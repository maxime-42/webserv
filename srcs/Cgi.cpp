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

/**
 * @brief free memory what as early alloc to the 2D array
 * 
 * @param array  is a pointer which has the location to free memory
 */
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

/**
 * @brief 
 *	get the current working directory and concatene that to the  _script
 *	first add back a slash to the name of "_script"
 *	save the  current working directory given by getcwd
 *	if neither error   appear then the concatenation going to done between   current working directory and script
 */
void	Cgi::								complete_the_name_of_script()
{
	char 									*current_working_directory = NULL;
	_script = "/" + _script;
	current_working_directory = getcwd(NULL, 0);
	_pwd = current_working_directory;//stock current_working_directory 
	if (current_working_directory == NULL)
	{
		throw("error while getcwd");
		_has_error = true;
	}
	else
	{
		_script = current_working_directory + _script;
		free(current_working_directory);
	}
}

/**
 * @brief 
 * Set the 2D array it is a char ** _args
 * recover the cgi binary file name and script name, store it in array "_args"
 * _args meaning argument
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
	Request *ptr_request = (Request *)ptr_void;
	_env_map["SERVER_PROTOCOL"] = "HTTP/1.1";
	_env_map["REDIRECT_STATUS"] = "200";
	_env_map["SCRIPT_NAME"] = _args[1];
	_env_map["HTTP_RAW_POST_DATA"] = ptr_request->header["body"]; 
	_env_map["PATH_INFO"] = _pwd + "/usr/bin/php-cgi";
}

/**
 * @brief 
 * recover the variables environment in _env_map then store them in _env
* skim the _env map to get each variables,  concatenate value and key with "=" then put the string concat in _env 
*/
void	Cgi::set_env()
{
	_env = (char **)malloc(sizeof(char *) * (_env_map.size() + 1));
	if (_env == NULL)
		throw("error happened while mallo in set_env() to cgi");
	int i = 0;
	for(std::map<std::string, std::string>::iterator it = _env_map.begin(); it != _env_map.end(); it++)
	{
		std::string var_env = (it->first + "=" + it->second);
		_env[i] = strdup(var_env.c_str());
		// _env[i] = strdup((it->first + "=" + it->second).c_str());
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
	if (pid == 0)
	{
		close(pipeFd[TO_READ]);/*closing of read side of pipe because it gonna write*/
		dup2(pipeFd[TO_WRITE], 1);  /* connect the write side with stdout */
/*
		int pipe2[2];
		pipe(pipe2);

		dup2(pipe2[TO_READ], 0);

		write(pipe2[TO_WRITE], POST_BODY, POST_BODY_LENGH);
*/		
		
		//if (execve(_args[0], _args, _env) == ERROR) 
		 if (execv(_args[0], _args) == ERROR) 
			exit(ERROR);
		exit(SUCCESS);
	}
	else
	{
		close(pipeFd[TO_WRITE]);/*closing of write side of pipe because it read*/
		int ret = wait(&child_status);
		check_error(ret, "error wait");
		check_error(child_status, "error execve");
		char		c; /* this variable will skim to each character in pipeFd[TO_READ] */
		while (read(pipeFd[TO_READ], &c, 1) > 0)/*from, here contains in pipeFd[TO_READ] gonna be copy in string "_data"*/
		{
			_data += c;
		}
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
**	this function recovery all variable in _url
** it meant everything after ? 
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