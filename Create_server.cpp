#include "Create_server.hpp"
#include <cstring>


namespace ft
{
	Create_server::					~Create_server()
	{
		for (int i = 0; i < _nfds; i++)
		{
			if(_tab_poll[i].fd >= 0)
				close(_tab_poll[i].fd);
		}
	}

	void	Create_server::			test_error(int error_code, char const *msg)
	{
		if (error_code < 0)
		{
			std::cout  << msg << std::endl;
			close(_server_fd);
			this->~Create_server();
			exit (ERROR);
		}
	}
	

 	Create_server::					Create_server(int port):_port(port)
	{
		_close_connexion = false;
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
		_tab_poll[0].fd = _server_fd;/* Set up the initial listening socket to monitori */
		_tab_poll[0].events = POLLIN;
	}

	void	Create_server::			set_tab_poll(int fd)
	{
		_tab_poll[_nfds].fd = fd;
		_tab_poll[_nfds].events = POLLIN;
		_nfds++;
	}

	void	Create_server::			accept_all_incoming_connections(struct pollfd	*_tab_poll)
	{
		int new_sd = 0;
		int addrlen = sizeof(_address);
		while (new_sd != ERROR)
		{
			if (_tab_poll->fd == _server_fd)
			{
				new_sd = accept(_server_fd, (sockaddr *)&_address, (socklen_t*)&addrlen);
				if (new_sd < 0)
				{
					if (errno != EWOULDBLOCK)
						test_error(ERROR, "accept() failed");
					else
						_nfds = ERROR;
				}
				else
				{
					_tab_poll[_nfds].fd = new_sd;
					_tab_poll[_nfds++].events = POLLIN;
				}
			}
		}
	}

	void	Create_server::  is_readable(struct pollfd	*_tab_poll)
	{
		int	ret;
		std::cout << "Descriptor " << _tab_poll->fd << " is readable" << std::endl;
		while (true)
		{
			ret = recv(_tab_poll->fd, _buffer, sizeof(_buffer), 0);
			if (ret < 0)
			{
				if (errno != EWOULDBLOCK)
				{
					test_error(ERROR, "  recv() failed");
					_close_connexion = true;
				}
				return ;
			}
			if (ret == 0)
			{
				std::cout << "Connection closed" << std::endl;
				_close_connexion = true;
				return ;
			}
			if (_close_connexion)
			{
				close(_tab_poll->fd);
				_tab_poll->fd = -1;
				_compress_array = true;
			}
		}
	}

	void	Create_server:: squeeze_tab_poll()
	{
		
		if (_compress_array)
		{
			_compress_array = false;
			for (int i = 0; i < _nfds; i++)
			{
				if (_tab_poll[i].fd == -1)
				{
					for(int j = i; j < _nfds - 1; j++)
					{
						_tab_poll[j].fd = _tab_poll[j+1].fd;
					}
					i--;
					_nfds--;
				}
			}
		}
	}

	void	Create_server::			start_svc()
	{
		int ret;
		int	current_size;
		while (true)
		{
			ret = poll(_tab_poll, _nfds, (3 * 60 * 1000));
			test_error(ret, "poll() failed");
			if (ret == 0)
				test_error(ERROR, "poll() timed out.  End program.");
			 current_size = _nfds;
			for (int index = 0; index < current_size; index++)
			{
				if(_tab_poll[index].revents == 0)//loop as long the are not event happened
					continue;
				if(_tab_poll[index].revents != POLLIN)// i will probably change it to handle the responses
					test_error(ERROR, "Error! revents");
				if (_tab_poll[index].fd == _server_fd)
					accept_all_incoming_connections(&_tab_poll[index]);
				else
					is_readable(&_tab_poll[index]);
				squeeze_tab_poll();
			}
		}
	}


	int	 	Create_server::			get_server_fd() {return (_server_fd);}
	int	 	Create_server::			get_listen_fd() {return (_listen_fd);}


	// int		Create_server::			get_nb_fd_poll() {return (_nb_fd_poll);}
	// void	Create_server::			set_nb_fd_poll(int n) {_nb_fd_poll = n;}

}