#include "Create_server.hpp"
#include <cstring>


namespace ft
{
	void	Create_server::			test_error(int error_code, char const *msg)
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
		test_error(ret, "bfrforce to reuse a port function setsockopt");
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
		_tab_poll[0].fd = _server_fd;/* Set up the initial listening socket                        */
		_tab_poll[0].events = POLLIN;
	}

	void	Create_server::			set_tab_poll(int fd)
	{
		_tab_poll[_nfds].fd = fd;
		_tab_poll[_nfds].events = POLLIN;
		_nfds++;
	}

	void	Create_server::			accept_all_incoming_connections(int index)
	{
		int new_sd = 0;
		while (new_sd != ERROR)
		{
			if (_tab_poll[index].fd == _server_fd)
			{
				int addrlen = sizeof(_address);
				new_sd = accept(_server_fd, (sockaddr *)&_address, (socklen_t*)&addrlen);
				if (new_sd < 0)
				{
					if (errno != EWOULDBLOCK)
						test_error(ERROR, "accept() failed");
					set_tab_poll(new_sd);
				}
			}
		}
	}


	void	Create_server::			start_svc()
	{
		int ret;
		int	current_size;
		int	timeout = (3 * 60 * 1000);
		while (true)
		{
			ret = poll(_tab_poll, _nfds, timeout);
			test_error(ret, "poll() failed");
			if (ret == 0)
				test_error(ERROR, "poll() timed out.  End program.");
			 current_size = _nfds;
			for (int i = 0; i < current_size; i++)
			{
				if(_tab_poll[i].revents == 0)//loop as long the are not event happened
					continue;
				if(_tab_poll[i].revents != POLLIN)// i will probably change it to handle the responses
					test_error(ERROR, "Error! revents");
				accept_all_incoming_connections(i);
			}
		}
	}


	int	 	Create_server::			get_server_fd() {return (_server_fd);}
	int	 	Create_server::			get_listen_fd() {return (_listen_fd);}


	// int		Create_server::			get_nb_fd_poll() {return (_nb_fd_poll);}
	// void	Create_server::			set_nb_fd_poll(int n) {_nb_fd_poll = n;}

}