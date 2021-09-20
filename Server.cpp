#include "Server.hpp"
#include <cstring>


namespace ft
{
	void			Server::test_error(int error_code, char const *msg)
	{
		if (error_code < 0)
		{
			std::cout  << msg << std::endl;
			//close(_network_socket);
			exit (ERROR);
		}
	}

	void Server::			create_network_socket()
	{
		_network_socket = socket(AF_INET, SOCK_STREAM, 0);
		test_error(_network_socket, "error create socket _network_socket_");
		// Forcefully attaching socket to the port 
		int ret = setsockopt(_network_socket, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &_opt, sizeof(_opt));
		test_error(ret, "foce to reuse a port");
		// Set socket to be nonblocking
		ret = ioctl(_network_socket, FIONBIO, (char *)&_opt);
		test_error(ret, "set socket to be nonblocking");

	}

	Server::			Server():_port(PORT), _nb_pending(NB_PENDING),_opt(1){std::cout << "default constructor calling...\n" << std::endl;}

	Server::			Server(int port):_port(port), _nb_pending(NB_PENDING),_opt(1){}

	Server::			~Server(){}

	void Server::			init_struct_sockaddr()
	{
		_address.sin_family = AF_INET;
		_address.sin_addr.s_addr = INADDR_ANY;
		_address.sin_port = htons(_port);
		 memset(_address.sin_zero, '\0', sizeof (_address.sin_zero));
	}


	void    Server::		bind_and_listen()
	{
		int ret;

		_addrlen = sizeof(_address);
		ret = bind(_network_socket, (struct sockaddr *)&_address, _addrlen);
		test_error(ret, "error while the bind");
		_listen_fd = listen(_network_socket, _nb_pending);
		test_error(_listen_fd, "error while the listening");
	}

	int	 Server::		get_network_socket() {return (_network_socket);}
}
