#ifndef REQUEST_HPP
#define REQUEST_HPP

#include "Server.hpp"

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

		Request();
		~Request();

        void                    parse(std::string request_str);
        void                    process();
        void                    send_reponse(int socket);


};

#endif
