#ifndef	CREATE_SERVER_HPP
#define	CREATE_SERVER_HPP
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <poll.h>
#include <iostream>
#include <netinet/in.h>
#include <sys/ioctl.h>
#include <stdio.h>
#include <fcntl.h>
#include "define.hpp"

namespace ft
{
	class Create_server
	{
	private:
		int						_server_fd;
		int						_listen_fd;
		struct	sockaddr_in		_address;
		int						_port;

	public:
		Create_server();
		Create_server(int port);
		~Create_server();
		void					setup();
 		void					test_error(int error_code, char const *msg);
		int						get_server_fd();
		int						get_listen_fd();
	};
}


#endif
