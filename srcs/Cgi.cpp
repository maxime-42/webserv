#include "Cgi.hpp"
#include <string.h>

Cgi::Cgi() {}

Cgi::~Cgi(){}

void		displayDirectionary(std::map<std::string, std::string> &map);
void	print_arg(char **array);

Cgi::Cgi(std::string script, int port): _script(script), _port(port), _cgi_path(CGI_PATH), _env(NULL), _args( NULL)
{
	_data = std::string();
	_has_error = false;
	try
	{
		complete_the_name_of_script();
		set_args();
		set_env();
		print_arg(_args);
		exec_Cgi();
	}
	catch(const char *e)
	{
		std::cerr << e << '\n';
	}
	std::cout << "data [" << _data << "]" << std::endl;
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

/*
 * recover the HTTP_META_VARIABLES and store them in a malloc'd pointer to
 * strings (char **).
 * */
void	Cgi::set_env()
{

	std::map<std::string, std::string> env_map;
	env_map["SERVER_PROTOCOL"] = "HTTP/1.1";
	env_map["SERVER_SOFTWARE"] = "webserv/1.0";
	env_map["GATEWAY_INTERFACE"] = "CGI/1.1";
	env_map["REDIRECT_STATUS"] = "200";
	env_map["STATUS_CODE"] = "200 OK";

	env_map["SCRIPT_NAME"] = _args[1];
	getInfo(8080, "listen", &env_map["SERVER_PORT"], find_directive);
	getInfo(8080, "server_name", &env_map["SERVER_NAME"], find_directive);
	// std::cout << "_args[1]; == " << _args[1] << std::endl;
	// env_map["REQUEST_URI"] = ;
	// env_map["PATH_INFO"] = ;
	// env["PATH_TRANSLATED"] =;
	// env_map["CONTENT_TYPE"] = ;
	// env_map["CONTENT_LENGTH"] = ;
	// env_map["REQUEST_METHOD"] = ";
	//env['QUERY_STRING'] = get_query_string();
	// displayDirectionary(env_map);
	int i = 0;
	_env = (char **)malloc(sizeof(char *) * (env_map.size() + 1));
	if (_env == NULL)
		throw("error happened while mallo in set_env() to cgi");
	for(std::map<std::string, std::string>::iterator it = env_map.begin(); it != env_map.end(); it++)
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
	if (pid == 0)
	{
		close(pipeFd[TO_READ]);/*closing of read side of pipe because it gonna write*/
		dup2(pipeFd[TO_WRITE], 1);  /* connect the write side with stdout */
		if (execve(_args[0], _args, _env) == ERROR) 
		// if (execv(_args[0], _args) == ERROR) 
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

std::string		Cgi::	get_data(){return (_data);}

/*
**	this function get all variable in _url
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

