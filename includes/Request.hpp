#ifndef REQUEST_HPP
# define REQUEST_HPP

#include "Server.hpp"
#include <vector>
#include "lookup_in_nested_list.hpp"

/*
	TODO: check if we need to implement here or if there already in Server.hpp
*/
#include <iostream>
#include <fstream>
#include <dirent.h>
#include <sys/stat.h>
#include <ctime>
//std::map<int, std::vector<unsigned char> >	g_request;

# define SUCCESS 0
# define FAILURE 1

class Request
{

	private:

        std::map<std::string, std::string>  http_table();

        void                                _process_GET();
        void                                _process_POST();
        void                                _process_DELETE();

        void                                http_code(std::string code);

	public:

		std::map<std::string, std::string> header;
		std::map<std::string, std::string> reponse;

		std::vector<char>			request;

		Request();
		~Request();

		int						store(char buffer[BUFFER_SIZE], struct pollfd *ptr_tab_poll, int bytes);
		int						sendall(int s, const char *buf, int len);
        void					parse(struct pollfd *ptr_tab_poll, int port);
        void                    process();
        int						send_reponse(int socket);
		bool					end_reached(struct pollfd *ptr_tab_poll);
        int                     create_file(std::string const file_type);
		std::string				find_url_and_name_from_file(std::string const file_type);


};

#endif
