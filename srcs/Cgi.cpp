#include "Cgi.hpp"
#include <string.h>

Cgi::Cgi(/* args */){}

Cgi::~Cgi(){}


Cgi::Cgi(std::string script, std::string  path_script, char **env_main):_script(script), _path_script(path_script), _args(NULL), _env_main(env_main), _data(std::string()),_has_error(false)
{
	try
	{
		adjust_name_script();
		set_args();
		execCgi();
	}
	catch(const char *e)
	{
		std::cerr << e << '\n';
	}
	(void)_env_main;
	clear_args();
	
}

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

void	Cgi::	adjust_name_script()
{
	char 		*pwd = NULL;
	_script = "/" + _script;
	pwd = getcwd(NULL, 0);
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


void	Cgi::set_args()
{

	 _args = (char**)malloc(sizeof(char *) * (NUMBER_ARGUMENTS + 1));
	char *name = strdup((char*)_script.c_str());
	char *path =  strdup((char*)_path_script.c_str());
	if (name == NULL)
		throw("error alloc for name");
	if (path == NULL)
		throw("error mal for path");
	_args[0] = path;
	_args[1] = name;
	_args[2] = NULL;
}

void	Cgi::set_env()
{

}

void	Cgi::check_error(int code, const char *error_msg)
{
	if (code < 0)
		throw(error_msg);
}



void		Cgi::	execCgi()
{
 	int			pipeFd[2];
	int			child_status;
	
	pipe(pipeFd);
	int			pid = fork();
	check_error(pid, "error cgi fork faile\n");
	if (pid == 0)
	{
		close(pipeFd[TO_READ]);
		dup2(pipeFd[TO_WRITE], 1);
		if (execve(_args[0], _args, _env_main) == -1) 
			exit(ERROR);
		exit(ERROR);
	}
	else
	{
		close(pipeFd[TO_WRITE]);
		int ret = wait(&child_status);
		check_error(ret, "error wait");
		check_error(child_status, "error execve");
		char		c;
		while (read(pipeFd[TO_READ], &c, 1) > 0)
		{
			_data += c;
		}
	}
	std::cout << "data=[" << _data << "]" << std::endl;
}
