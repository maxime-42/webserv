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
private:
	std::string _script;
	std::string 							_path_script;
	char									**_args;
	char									**_env_main;
	std::string								_data;
	std::string								_pwd;
	bool									_has_error;

	Cgi(/* args */);
	void		complete_the_name_of_script();
	void		set_args();
	void		set_env();
	void		check_error(int code, const char *error_msg);
	void		retrieve_data();
	void		clear_args();
	void		exec_Cgi();

public:
	~Cgi();
	Cgi(std::string script, std::string  path_script, char **env);
	std::string			get_data();

};

#endif