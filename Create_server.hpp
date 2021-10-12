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
#include <unistd.h>
#include "define.hpp"
#include <sys/types.h>
#include <sys/socket.h>

namespace ft
{
	class Create_server
	{
	private:
		int						_server_fd;
		int						_listen_fd;
		char					_buffer[BUFFER_SIZE];
		struct	sockaddr_in		_address;
		int						_port;
		int						_timeout;
		bool					_close_connexion;
		bool					_compress_array;

		struct pollfd			_tab_poll[SIZE_POLLFD];
		int						_nfds; //number file descriptor sever in _tab_poll


	public:
		Create_server();
		Create_server(int port);
		~Create_server();
		void					setup();
 		void					test_error(int error_code, char const *msg);
		int						get_server_fd();
		int						get_listen_fd();
		void					setup_poll(); 
		void					start_svc();
		void					set_tab_poll(int fd);
		void					existing_connection(struct pollfd	*_tab_poll);
		void					squeeze_tab_poll();
		void					accept_all_incoming_connections();
		bool					receive_data(struct pollfd	*_tab_poll);
		bool					send_data(struct pollfd	*_tab_poll);
		void					clean_all_socket();

	};
}


#endif
