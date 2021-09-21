#include "Server.hpp"
#include <cstring>


namespace ft
{
	void			Server::test_error(int error_code, char const *msg)
	{
		if (error_code < 0)
		{
			std::cout  << msg << std::endl;
			//close(server_fd);
			exit (ERROR);
		}
	}

	void Server::			create_server_fd()
	{
		_server_fd = socket(AF_INET, SOCK_STREAM, 0);

		test_error(_server_fd, "error create socket _server_fd_");

		// Forcefully attaching socket to the port
		// setsockopt(listen_sd, SOL_SOCKET,  SO_REUSEADDR, (char *)&on, sizeof(on));
		int ret = setsockopt(_server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, (char *)&_opt, sizeof(_opt));
		test_error(ret, "foce to reuse a port");

		// Set socket to be nonblocking
		ret = ioctl(_server_fd, FIONBIO, (char *)&_opt);
		test_error(ret, "set socket to be nonblocking");
	}

	Server::			Server():_port(PORT), _nb_pending(NB_PENDING), _opt(1){std::cout << "default constructor server calling...\n" << std::endl;}

	Server::			Server(int port):_port(port), _nb_pending(NB_PENDING), _opt(1){}

	Server::			~Server(){}

	void    Server::		binding()
	{
		int ret = 0;


		memset(&_address, 0, sizeof (_address));		

		_address.sin6_family      = AF_INET6;

		memcpy(&_address.sin6_addr, &in6addr_any, sizeof(in6addr_any));

		_address.sin6_port        = htons(PORT);

		// memset(&_address.sin_zero, '\0', sizeof (_address.sin_zero));

		// _address.sin_addr.s_addr = INADDR_ANY;

		// _address.sin_port = htons(_port);

		_addrlen = sizeof(_address);
		// std::cout << " _addrlen =  " << _addrlen << std::endl;
		ret = bind(_server_fd, (struct sockaddr *)&_address, _addrlen);

		// std::cout << "ret =  " << ret << std::endl;		
		test_error(ret, "error while the binding");
	}
 
	// void    Server::		binding()
	// {
	// 	int ret = 0;


	// 	// memset(&_address, 0, sizeof (_address));		

	// 	_address.sin_family = AF_INET;

	// 	memset(&_address.sin_zero, '\0', sizeof (_address.sin_zero));

	// 	_address.sin_addr.s_addr = INADDR_ANY;

	// 	_address.sin_port = htons(_port);

	// 	_addrlen = sizeof(_address);
	// 	// std::cout << " _addrlen =  " << _addrlen << std::endl;
	// 	ret = bind(_server_fd, (struct sockaddr *)&_address, _addrlen);

	// 	// std::cout << "ret =  " << ret << std::endl;		
	// 	test_error(ret, "error while the binding");
	// }

	void	Server::		start_server()
	{
		create_server_fd();
		binding();
		_listen_fd = listen(_server_fd, _nb_pending);
		test_error(_listen_fd, "error while the listening");
		// std::cout << " _server_fd " << _server_fd << std::endl;
		// bind_and_listen();
		// std::cout << "_server_fd = " << _server_fd << std::endl;
	}

	int	 Server::		get_server_fd() {return (_server_fd);}
}
