#include "Cgi.hpp"
#include <string.h>
#include "Request.hpp"

Cgi::Cgi() {}

Cgi::~Cgi(){}

void		displayDirectionary(std::map<std::string, std::string> &map);
void		print_arg(char **array);

Cgi::Cgi(std::string script, void *ptr_void, std::map<std::string, std::string> &cgi_head): _script(script), _cgi_path(CGI_PATH), _env(NULL), _args( NULL)
{
	_data = std::string();
	_has_error = false;
	try
	{
		complete_the_name_of_script();
		set_args();
		// print_arg(_args);
		set_env_map(ptr_void);
		set_env();
		exec_Cgi();
		// print_arg(_env);
		remove_headers(cgi_head);
	}
	catch(const char *e)
	{
		std::cerr << e << '\n';
	}
//	std::cout << "data [" << _data << "]" << std::endl;
	clear_2D_array(_env);
	clear_2D_array(_args);
}

/*
** free memory what as early alloc to "_arg"
** this function free memory which was early alloc to "_arg"
*/
void	Cgi::clear_args()
{
	for (size_t i = 0; i < NUMBER_ARGUMENTS ; i++)
	{
		free(_args[i]);
		_args[i] = NULL;
	}
	free(_args);
	_args = NULL;
}

void	Cgi::clear_2D_array(char **array)
{
	int i = 0;
	while (array[i])
	{
		free(array[i]);
		i++;
	}
	free(array);
}

/*
** this function add  slash to the name of script
** add likewise name of the current working directory to script
*/

void	Cgi::								complete_the_name_of_script()
{
	char 									*pwd = NULL;
	_script = "/" + _script;
	pwd = getcwd(NULL, 0);
	_pwd = pwd;//stock pwd 
	if (pwd == NULL)
	{
		throw("error while getcwd");
		_has_error = true;
	}
	else
	{
		_script = pwd + _script;
		free(pwd);
	}
}

/*
** recover the cgi binary file name and script name, store it in array "_args"
** _args meaning argument
*/

void	Cgi::								set_args()
{
	 _args = (char**)malloc(sizeof(char *) * (NUMBER_ARGUMENTS + 1));
	char *name = strdup((char*)_script.c_str());
	char *path =  strdup((char*)_cgi_path.c_str());
	if (name == NULL)
		throw("error alloc for name");
	if (path == NULL)
		throw("error mal for path");
	_args[0] = path;
	_args[1] = name;
	_args[2] = NULL;
}

/**
 * @brief this function set a set of variable environement in map  
 * 
 * @param ptr_void it is a pointer of Request which has  some functions to  values of variable environnement
 */
void	Cgi::set_env_map(void *ptr_void)
{
	/*
		Set cgi body to empty string to check when we execve to know if there is something to send in STDIN
	*/
	_cgi_body = "";

	Request *ptr_request = (Request *)ptr_void;
	_env_map["SERVER_PROTOCOL"] = "HTTP/1.1";
	_env_map["REDIRECT_STATUS"] = "200";
	_env_map["SCRIPT_NAME"] = _args[1];
	_env_map["HTTP_RAW_POST_DATA"] = ptr_request->header["body"]; 
	_env_map["PATH_INFO"] = _pwd + "/usr/bin/php-cgi";

/*
	DEBUG :
	std::cout << "       methode = " << ptr_request->header["method"] << std::endl;
	std::cout << "       url = " << ptr_request->header["url"] << std::endl;
	std::cout << "       arg = " << ptr_request->header["args"] << std::endl;
*/

	if (ptr_request->header["method"] == "GET" && ptr_request->header["args"] != "")
	{
		_env_map["QUERY_STRING"] = ptr_request->header["args"];
		_cgi_body = ptr_request->header["args"];
	}
	if (ptr_request->header["method"] == "POST")
	{
		_cgi_body = ptr_request->header["body"];
	}
}

/*
 * recover the HTTP_META_VARIABLES and store them in a malloc pointer of ch
 * strings (char **).
 *
*/
void	Cgi::set_env()
{
	_env = (char **)malloc(sizeof(char *) * (_env_map.size() + 1));
	if (_env == NULL)
		throw("error happened while mallo in set_env() to cgi");
	int i = 0;
	for(std::map<std::string, std::string>::iterator it = _env_map.begin(); it != _env_map.end(); it++)
	{
		_env[i] = strdup((it->first + "=" + it->second).c_str());
		if (_env[i] == NULL)
			throw("error happened while mallo in set_env() to cgi");
		i++;
	}
	_env[i] = NULL;
}

/*
**	this function throw error if ever the variable "code" is less than zero
*/
void	Cgi::								check_error(int code, const char *error_msg)
{
	if (code < 0)
		throw(error_msg);
}


/*
** the behind this function is to get the stdout of process child in other file descriptor like pipeFd[TO_READ]
** it mean there for to write the stdout of process child in  pipeFd[TO_WRITE]
**	"child_status" gonna have the code of exite child process, if the exit code  is less than zero an error will throw
** afterward the contain in pipeFd[TO_READ] will be copy in string "_data"
** afterward 
*/
void		Cgi::	exec_Cgi()
{
 	int			pipeFd[2];
	int			child_status;
	
	pipe(pipeFd);
	int			pid = fork();
	check_error(pid, "error cgi fork failed\n");
	/*
		TODO : Actuellement on remplit bien le cgi dans le cadre d'une requete POST avec des args.
		Mais on pid != 0 donc on rentre pas dans =====> <======
	*/
	if (pid == 0)
	{
		close(pipeFd[TO_READ]);/*closing read side of pipe because we're only going to write*/
		dup2(pipeFd[TO_WRITE], 1);  /* connect the write side with stdout */

		if (_cgi_body.length() > 0)
		{
			int pipe2[2];
			pipe(pipe2);

			dup2(pipe2[TO_READ], 0);

			write(pipe2[TO_WRITE], _cgi_body.c_str(), _cgi_body.length());
			//write(pipe2[TO_WRITE], "PIPIPIPiPi", 10);
			close(pipe2[TO_WRITE]); // send EOF
		}

		//if (execve(_args[0], _args, _env) == ERROR) 
		if (execv(_args[0], _args) == ERROR) 
			exit(ERROR);
		exit(SUCCESS);
	}
	else
	{
//		std::cout << "cgi PETITIIIs = [" << _cgi_body << "]\n";
		close(pipeFd[TO_WRITE]);/*closing of write side of pipe because it read*/
		int ret = wait(&child_status);
		check_error(ret, "error wait");
		check_error(child_status, "error execve");
		std::cout << "execv(" << _args[0] << ", " << _args[1] << ")" << std::endl;
		char		c; /* this variable will skim to each character in pipeFd[TO_READ] */
		while (read(pipeFd[TO_READ], &c, 1) > 0)/*from, here contains in pipeFd[TO_READ] gonna be copy in string "_data"*/
		{
			_data += c; 
		}
		std::cout << "data ->" << _data << "<-" << std::endl;
	}
}

void		Cgi::	remove_headers(std::map<std::string, std::string> &cgi_head) {

	std::string	header("");
	std::string key;
	std::string value;

	size_t pos2;
	size_t pos = _data.find("\r\n\r\n");
	if (pos != std::string::npos) {

		header = _data.substr(0, pos + 2);
		_data = _data.substr(pos + 4, std::string::npos);
	
	}
	while ((pos = header.find("\r\n")) != std::string::npos
			&& (pos2 = header.find(":")) != std::string::npos
 		 	&& pos2 < pos) {

		key = header.substr(0, pos2);
		value = header.substr(pos2 + 2, pos - pos2 - 2);

		cgi_head[key] = value;

//		std::cout << "key: " << key << "<-" << std::endl;
//		std::cout << "value: " << value << "<-" << std::endl;

		header.erase(0, pos + 2);
	}

}

std::string		Cgi::	get_data(){return (_data);}

/*
**	this function get all variable in _url
** it meant everything after ? 
*/

/*
	Debug : THIS FUNCTION IS USELESS FINALLY BECAUSE THE QUERY IS SPLIT FROM URL IN args IN THE REQUEST::PARSE
	Flemme de ré écrire en miniscule.............;
*/
std::string		Cgi::	get_query_string()
{
	std::string query_string;
	size_t pos =  _uri.find("?");
	if (pos != std::string::npos)/*check if "?" was there */
	{
		query_string = _uri.substr(pos, _uri.length()); /*copy from position of "?" until end string*/
	}
	return (query_string);
}

