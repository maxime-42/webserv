#include "Create_server.hpp"
#include <cstring>


namespace ft
{
	Create_server::					~Create_server(){}

 	Create_server::					Create_server(int port):_port(port)
	{
		setup();

	}
	
	void	Create_server::			setup()
	{
		int ret;//return value of functions
		int	option = 1;

		_server_fd = socket(AF_INET, SOCK_STREAM, 0);//  Creating socket file descriptor, SOCK_STREAM: TCP(reliable, connection oriented)
		test_error(_server_fd, "error create socket");
		std::cout << "server_fd = " << _server_fd << std::endl;
		ret = setsockopt(_server_fd, SOL_SOCKET,  SO_REUSEADDR, (char *)&option, sizeof(option));// Forcefully attaching socket to the port
		test_error(ret, "Error force to reuse a port function setsockopt");
		ret = fcntl(_server_fd, F_SETFL, O_NONBLOCK);// Set socket to be nonblocking
		test_error(ret, "Error function fcnt");
		_address.sin_family = AF_INET;// IPv4 protocol
		memset(&_address.sin_zero, 0, sizeof (_address.sin_zero));
		_address.sin_addr.s_addr = INADDR_ANY;
		_address.sin_port = htons(_port);
		ret = bind(_server_fd, (struct sockaddr *)&_address, sizeof(_address));    // Forcefully attaching socket to the port
		test_error(ret, "Error while the binding");
		_listen_fd = listen(_server_fd, NB_PENDING);
		std::cout << "listen_fd = " << _listen_fd<< std::endl;
		test_error(_listen_fd, "Error function  the listening");
		/* Set up the initial listening socket                        */
		_pollFd[0].fd = _server_fd;
		_pollFd[0].events = POLLIN;
	}
	
	// void	Create_server::			receive()
	// void	Create_server::			response()

	int	 	Create_server::			get_server_fd() {return (_server_fd);}
	int	 	Create_server::			get_listen_fd() {return (_listen_fd);}
	// int		Create_server::			get_nb_fd_poll() {return (_nb_fd_poll);}
	// void	Create_server::			set_nb_fd_poll(int n) {_nb_fd_poll = n;}

}