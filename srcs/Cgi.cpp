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
		print_arg(_args);
		set_env_map(ptr_void);
		displayDirectionary(_env_map);
		set_env();
		print_arg(_env);
		exec_Cgi();
		// print_arg(_env);
		remove_headers(cgi_head);
	}
	catch(const char *e)
	{
		std::cerr << e << '\n';
	}
	clear_2D_array(_env);
	clear_2D_array(_args);
}

/**
 * @brief 
 * free memory what as early alloc to the 2D array
 * @param array  is a pointer which has the location to free memory
 */
void	Cgi::clear_2D_array(char **array)
{
	int i = 0;
	if (array == NULL)
		return ;
	while (array[i])
	{
		free(array[i]);
		i++;
	}
	free(array);
}

/**
 * @brief 
 *	get the current working directory and concatenate that to the  script, it let to have absolute path of script
 *	first add back a slash to the name of "_script"
 *	save the  current working directory given by function getcwd
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
 * Create a 2D array it is a char ** _args
 * this 2D array will contain the cgi absolute path  and script absolute path
 * _args meaning argument
 */
void	Cgi::								set_args()
{
	
	 _args = (char**)malloc(sizeof(char *) * (NUMBER_ARGUMENTS + 1));
	// char *name = strdup((char*)_script.c_str());
	char *name = strdup("/home/lenox/webserv/www/cgi/post-method.php");
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
 * @brief 
 * this function set a set of variable environement in map
 * @param ptr_void it is a pointer of Request object which has  some functions to values of variable environnement
 */
void	Cgi::set_env_map(void *ptr_void)
{
	/*
		Set cgi body to empty string to check when we execve to know if there is something to send in STDIN
	*/
	// _cgi_body = "";
	Request *ptr_request = (Request *)ptr_void;
	_env_map["SERVER_PROTOCOL"] = "HTTP/1.1";

	// _env_map["AUTH_TYPE"] = "";
	// _env_map["CONTENT-LENGTH"] = "0";
	// _env_map["GATEWAY_INTERFACE"] = "CGI/1.1";
	// _env_map["RAW_POST_DATA"] = ptr_request->header["body"]; 
	_env_map["PATH_TRANSLATED"] = _args[0];
	_env_map["REDIRECT_STATUS"] = "200";
	_env_map["SCRIPT_FILENAME"] = _args[0];
	_env_map["SCRIPT_NAME"] = _args[1];
	_env_map["SCRIPT_PORT"] = ptr_request->header["port"];
	if (ptr_request->header["method"] == "GET" && ptr_request->header["args"] != "")
	{
		_env_map["QUERY_STRING"] = ptr_request->header["args"];
		_cgi_body = ptr_request->header["args"];
	}
	if (ptr_request->header["method"] == "POST")
	{
		_env_map["CONTENT_LENGTH"] = ptr_request->header["CONTENT-LENGTH"];
		_env_map["CONTENT_TYPE"] = ptr_request->header["CONTENT-TYPE"];
		_cgi_body = ptr_request->header["body"];
		_env_map["ACCEPT_ENCODING"] = "gzip, deflate, br";
		_env_map["USER_AGENT"] = "Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/94.0.4606.81 Safari/537.36";
		_env_map["REFERER"] = "http://localhost:8080/cgi/post-method.php";
		_env_map["ACCEPT_LANGUAGE"] = "fr-FR,fr;q=0.9,en-US;q=0.8,en;q=0.7";
		_env_map["ACCEPT"] = "text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.9";
		_env_map["CACHE_CONTROL"] = "max-age=0";
		_env_map["ORIGIN"] = "http://localhost:8080";
		// _env_map["SERVER_SOFTWARE"] = "webserv/0.1.0";

		// _env_map["PATH_INFO"] = "http://localhost:8080/cgi/post-method.php";
		// _env_map["PRAGMA"] = "no-cache";
		// _env_map["REQUEST_METHOD"] = ptr_request->header["method"];



	}
}

/**
 * @brief
 * recover the variables environment in map "_env_map" then store them in a char ** _env
 * skim the map "_env" map to get each variables, concatenate value and key with "=" then put the string concat in _env 
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
		if (_env[i] == NULL)
			throw("error happened while mallo in set_env() to cgi");
		i++;
	}
	_env[i] = NULL;
}

/**
 * @brief 
 * this function throw error if ever the variable "code" is less than zero
 * @param code if is less than zero an error will throw
 * @param error_msg text message to display
 */
void	Cgi::								check_error(int code, const char *error_msg)
{
	if (code < 0)
		throw(error_msg);
}

/**
 * @brief
 * the behind this function is to get the stdout of process child in other file descriptor like pipeFd[TO_READ]
 * it mean there for to write the stdout of process child in  pipeFd[TO_WRITE]
 * "child_status" gonna have the code of exite child process, if the exit code  is less than zero an error will throw
 * afterward the contain in pipeFd[TO_READ] will be copy in string "_data"
 */
void		Cgi::	exec_Cgi()
{
 	int			pipeFd[2];
	int			child_status;

	pipe(pipeFd);
	std::cout << "cgi body  ==" << _cgi_body << "==\n";
	int			pid = fork();
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

		if (execve(_args[0], _args, _env) == ERROR) 
		//if (execv(_args[0], _args) == ERROR) 
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
		// set_data(pipeFd[TO_READ]);
			std::cout << "\ndata==" << _data << "==" << std::endl;
	}
}

/**
 * @brief Get the data object
 * std::string contains  data  has recovery through a file descriptor
 * @return std::string 
 */
std::string		Cgi::	get_data() {return (_data);}

/**
 * @brief Set data
 * data recovery  inside file descriptor to copy that in string "_data"
 * _data contains data has recovery through file descriptor
 * @param fd is the file descriptor where to recovery data
 */
void	Cgi::	set_data(int fd)
{
	char	buf[READ_SIZE + 1];
	int		nbByte = 0;
	while ( (nbByte = read(fd, buf, READ_SIZE)) > 0)/*from, here contains in pipeFd[TO_READ] gonna be copy in string "_data"*/
	{
		buf[nbByte] = '\0';
		_data += buf; 
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



/**
 * @brief Get the query string object
 * this function recovery all variable in _url it meant everything after ? 
 * @return std::string contains all data after ?
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

