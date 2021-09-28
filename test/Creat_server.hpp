#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <poll.h>
#include <iostream>
#include <netinet/in.h>
#include <sys/ioctl.h>
#include <stdio.h>

namespace ft
{
	class Create_server
	{
	private:
		int												_server_fd;
		int												_listen_fd;
		struct  sockaddr_in								_address;
		int												_port;
		int												_opt;

	public:
		Create_server();
		Create_server(int port);
		~Create_server();
		void											create_socket_fd();
		void											listen_fd();
		void											test_error(int error_code, char const *msg);
		void											to_bind();
		int												get_server_fd();
		int												get_listen_fd();
	};
}