#ifndef CGI
#define CGI // !1
#define NUMBER_ARGUMENTS 2
#define	TO_READ 0
#define	TO_WRITE 1
#define	ERROR -1
#define	SUCCESS 0

#include <string>
#include <unistd.h>
#include <iostream>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
class Cgi
{
//private:
	public:

	static std::string						_script;
	static std::string 						_path_script;
	static char								**_env_main;
	static char								**_args;

	std::string								_data;
	std::string								_pwd;
	bool									_has_error;

	void		complete_the_name_of_script();
	void		set_args();
	void		set_env();
	void		check_error(int code, const char *error_msg);
	void		retrieve_data();
	void		clear_args();
	void		exec_Cgi();

public:
	Cgi(std::string script);
//	Cgi(string script, std::string  path_script, char **env);
	Cgi(std::string  path_script, char **env);
	std::string			get_data();
	~Cgi();

};

#endif
