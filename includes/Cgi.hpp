#ifndef CGI
# ifndef CGI_PATH
#  define CGI_PATH "/usr/bin/php-cgi"
# endif

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
#include <map>
#include <sstream>
#include "utile.hpp"
#define READ_SIZE 1000000

class Cgi
{
private:
	Cgi();
	std::string							_script;
	int									_port;
	std::string 						_cgi_path;
	char								**_env;
	char								**_args;

	std::string							_data;
	std::string							_pwd;
	std::string							_uri;
	bool								_has_error;
	std::map<std::string, std::string> 	_env_map;
	std::string							_cgi_body;



	void					complete_the_name_of_script();
	void					set_args();
	void					set_env();
	void					check_error(int code, const char *error_msg);
	void					retrieve_data();
	void					clear_args();
	void					exec_Cgi();
	std::string				get_query_string();
	void					clear_2D_array(char **array);
	void					remove_headers(std::map<std::string, std::string> &cgi_head);
	void					set_env_map(void *ptr_void);
	void					set_data(int fd);

public:
	Cgi(std::string script, void *ptr_void, std::map<std::string, std::string> &reponse);
	std::string			get_data();
	~Cgi();

};

#endif
