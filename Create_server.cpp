#include "Create_server.hpp"
#include <cstring>


namespace ft
{
	void							Create_server::test_error(int error_code, char const *msg)
	{
		if (error_code < 0)
		{
			std::cout  << msg << std::endl;
			close(_server_fd);
			exit (ERROR);
		}
	}

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
	}
	
	void	Create_server:: setup_poll() 
	{
		/* Set up the initial listening socket                        */
		_pollFd[0].fd = this->get_server_fd();
		_pollFd[0].events = POLLIN;
		_timeout = (3 * 60 * 1000);//Initialize the timeout to 3 minutes
		int ret = poll(_pollFd, _nb_fd_poll, _timeout);
		test_error(ret, " poll() failed");
		if (ret == 0)
			test_error(ERROR, " timed out.  End program.");

	}

	int	 Create_server::			get_server_fd() {return (_server_fd);}
	int	 Create_server::			get_listen_fd() {return (_listen_fd);}
}