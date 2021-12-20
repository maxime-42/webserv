#ifndef	SERVER_HPP
#define	SERVER_HPP
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <poll.h>
#include <iostream>
#include <netinet/in.h>
#include <sys/ioctl.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>

#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <algorithm>
#include <cstring>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <map>
#include <vector>
#include "header.hpp"

/**
 * @brief 
 * it going to have as much Server object inside linked list than it will have server block in config file
 * it will help to have a program multiport
 */
class Server
{
	private:
		int						_server_fd;
		int						_listen_fd;
		char					_buffer[BUFFER_SIZE + 1];
		int						_port;
		bool					_close_connexion;
		bool					_compress_array;// any time a clients disconnected  this variable will be set to true to squeeze poll array
		bool					_hasError;
		std::vector<int>		 _sockect_clients;//this vector will store  socket clients which reached server 
		struct	sockaddr_in		_address;

	public:
		Server();
		Server(int port);
		~Server();
		void					setup();

		int						get_server_fd();
		void					setup_poll(); 
		std::vector<int> &		get_sockect_clients();
		void					accept_all_incoming_connections();
		bool					handle_existing_connections(struct pollfd	*_tab_poll);
		void					set_port(int port);
		int						getPort();
		int	 					get_hasError();


	private:
		void					test_error(int error_code, const char  * msg);
		void					set_tab_poll(int fd);
		void					squeeze_tab_poll();
		int						receive_data(struct pollfd	*_tab_poll);
		void					setPort(int port);
		
		void					parse_request(std::string r, std::map<std::string, std::string> & request,  std::map<std::string, std::string> & reponse);
		void					process_request(std::map<std::string, std::string> request, std::map<std::string, std::string> & reponse);
		void					send_reponse(int socket, std::map<std::string, std::string> & reponse);
		void					process_GET(std::map<std::string, std::string> request, std::map<std::string, std::string> & reponse);
		void					process_POST(std::map<std::string, std::string> request, std::map<std::string, std::string> & reponse);
		void					process_DELETE(std::map<std::string, std::string> request, std::map<std::string, std::string> & reponse);
		void					Squeze_vect_sockect_fd(int to_find);

};

#endif
