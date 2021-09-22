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
#include "define.hpp"

namespace ft
{
	class Server
	{
	private:
		int			_server_fd;
		int			_listen_fd;
		struct  sockaddr_in6	_address;
		int			_port;
		int			_opt;



	public:
		Server();
		Server(int port);
		~Server();
		void			create_server_fd();
		void			listen_fd();   
 		void			test_error(int error_code, char const *msg);
		void			bind_socket();
		int			get_server_fd();
		int			get_listen_fd();
		void			start_server();

    };
}

#endif
