#include "Server.hpp"
#include "Request.hpp"


Server::					~Server(){	}

void	Server:: clean_all_socket()
{

	for (int i = 0; i < _nfds; i++)
	{
		if(_tab_poll[i].fd >= 0)
			close(_tab_poll[i].fd);
	}
}

void	Server::			test_error(int error_code, char const *msg)
{
	if (error_code < 0)
	{
		std::cout  << msg << std::endl;
		close(_server_fd);
		throw(msg);
		this->~Server();
		exit (ERROR);
	}
}

Server::					Server(int port):_port(port), _nfds(1)
{
	try
	{
		setup();
	}
	catch(char *msg_error)
	{
		std::cerr << msg_error << '\n';
		this->~Server();
		exit (ERROR);
	}
}

void	Server::			setup()
{
	int ret;//return value of functions
	int	option = 1;

	_server_fd = socket(AF_INET, SOCK_STREAM, 0);//  Creating socket file descriptor, SOCK_STREAM: TCP(reliable, connection oriented)
	test_error(_server_fd, "error create socket");
	//std::cout << "server_fd = " << _server_fd << std::endl;
	ret = setsockopt(_server_fd, SOL_SOCKET,  SO_REUSEADDR, (char *)&option, sizeof(option));// Forcefully attaching socket to the port
	test_error(ret, "bfrforce to reuse a port function setsockopt");
	ret = fcntl(_server_fd, F_SETFL, O_NONBLOCK);// Set socket to be nonblocking
	test_error(ret, "Error function fcnt");
	_address.sin_family = AF_INET;// IPv4 protocol
	memset(&_address.sin_zero, 0, sizeof (_address.sin_zero));
	_address.sin_addr.s_addr = INADDR_ANY;
	//_address.sin_addr.s_addr = htons("127.0.0.1");
	_address.sin_port = htons(_port);
	ret = bind(_server_fd, (struct sockaddr *)&_address, sizeof(_address));    // Forcefully attaching socket to the port
	test_error(ret, "Error while the binding");
	_listen_fd = listen(_server_fd, NB_PENDING);
	test_error(_listen_fd, "Error function  the listening");
	_tab_poll[0].fd = _server_fd;/* Set up the initial listening socket to monitori */
	_tab_poll[0].events = POLLIN;
}

void	Server::			set_tab_poll(int fd)
{
    _tab_poll[_nfds].fd = fd;
    _tab_poll[_nfds].events = POLLIN;
    _nfds++;
}



bool	Server:: receive_data(struct pollfd	*ptr_tab_poll)
{	
	int ret;
	ret = recv(ptr_tab_poll->fd, _buffer, sizeof(_buffer), 0);
	if (ret < 0)
	{
		if (errno != EWOULDBLOCK)
		{
			_close_connexion = true;
			std::cout << "recv() failed" << std::endl;
		}
		return (false);
	}
	if (ret == 0)
	{
		std::cout << "connection closed from remote side" << std::endl;
		_close_connexion = true;
		return(false);
	}
	std::cout << ret << " bytes received:\n ===============\n" << _buffer << "\n===============\n"<< std::endl;
	return (true);
}


void	Server::  handle_existing_connections(struct pollfd	*ptr_tab_poll)
{
	Request request;
	_close_connexion = false;

	while (receive_data(ptr_tab_poll))
	{
		request.parse(std::string(_buffer));
		request.process();
		request.send_reponse(ptr_tab_poll->fd);
		close(ptr_tab_poll->fd);
	}

	if (_close_connexion)
	{
		std::cout << "close connection fd = " << ptr_tab_poll->fd << "\n" <<std::endl;
		close(ptr_tab_poll->fd);
		ptr_tab_poll->fd = ERROR;
	_compress_array = true;
    }

}

void	Server:: squeeze_tab_poll()
{

	if (_compress_array)
	{
		_compress_array = false;
		for (int i = 0; i < _nfds; i++)
		{
			if (_tab_poll[i].fd == ERROR)
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

void	Server::			accept_all_incoming_connections()
{
	int new_sd = 0;
	int addrlen = sizeof(_address);
	while (new_sd != ERROR)
	{
		new_sd = accept(_server_fd, (sockaddr *)&_address, (socklen_t*)&addrlen);
		if (new_sd < 0)
		{
 			if (errno != EWOULDBLOCK)
				test_error(ERROR, "accept() failed");
			new_sd = ERROR;
		}
		else
		{
			std::cout << "new incoming connection new_sd = " << new_sd << std::endl;
			//Add the new incoming connection to _tab_poll structure 
			_tab_poll[_nfds].fd = new_sd;
			_tab_poll[_nfds++].events = POLLIN;
		}
	}
}

void	Server::			start_svc()
{
	try
	{
		int ret;
		int	current_size;
		while (true)
		{
			ret = poll(_tab_poll, _nfds, TIMEOUT);
			test_error(ret, "poll() failed");
			if (ret == 0)
				test_error(ERROR, "poll() timed out.  End program.");
			current_size = _nfds;
			for (int index = 0; index < current_size; index++)
			{
				if(_tab_poll[index].revents == 0)//loop as long the are not event happened
					continue;
				if (_tab_poll[index].fd == _server_fd)
					accept_all_incoming_connections();
				else
					handle_existing_connections(&_tab_poll[index]);
			}
			squeeze_tab_poll();
		}
	}
	catch(std::string const  & msg_error)
	{
		std::cerr << msg_error << '\n';
	}
	
}


int	 	Server::			get_server_fd() {return (_server_fd);}
int	 	Server::			get_listen_fd() {return (_listen_fd);}