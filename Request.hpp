#ifndef REQUEST_HPP
#define REQUEST_HPP

#include "Server.hpp"
#include <iostream>
#include <fstream>

# define SUCCESS 0
# define FAILURE 1

class Request
{

	private:

        void                    _process_GET();
        void                    _process_POST();
        void                    _process_DELETE();

	public:

		std::map<std::string, std::string> header;
		std::map<std::string, std::string> reponse;

		Request();
		~Request();

        void                    parse(std::string request_str);
        void                    process();
        void                    send_reponse(int socket);
        int                     create_file(std::string const file_type);


};

#endif
