#include "Server.hpp"
#include "Request.hpp"


Server::								~Server(){	}

Server::								Server():_port(PORT), _hasError(false){}

Server::								Server(int port):_port(port), _hasError(false){}

void	Server::						test_error(int error_code,  const char *  msg)
{
	if (error_code < 0)
	{
		close(_server_fd);
		throw(msg);
	}
}

void	Server::			setup()
{
	int ret;//will contain the return value of functions
	int	option = 1;
	try
	{
		_server_fd = socket(AF_INET, SOCK_STREAM, 0);//  Creating socket file descriptor, SOCK_STREAM: TCP(reliable, connection oriented)
		test_error(_server_fd, "error create socket");
		ret = setsockopt(_server_fd, SOL_SOCKET,  SO_REUSEADDR, (char *)&option, sizeof(option));// Forcefully attaching socket to the port
		test_error(ret, "bfrforce to reuse a port function setsockopt");
		ret = fcntl(_server_fd, F_SETFL, O_NONBLOCK);// Set socket to be nonblocking
		test_error(ret, "Error function fcnt");
		_address.sin_family = AF_INET;// IPv4 protocol
		memset(&_address.sin_zero, 0, sizeof (_address.sin_zero));
		_address.sin_addr.s_addr = INADDR_ANY;
		_address.sin_port = htons(_port);
		ret = bind(_server_fd, (struct sockaddr *)&_address, sizeof(_address));// Forcefully attaching socket to the port
		test_error(ret, "Error while the binding");
		_listen_fd = listen(_server_fd, NB_PENDING);
		test_error(_listen_fd, "Error function  the listening");
	}
	catch( const char  * msg_error)
	{
		std::cerr << msg_error << std::endl;
		_hasError = true;
	}
}

/*
** this function remove socke given in parameter, it meaning this client is disconnected 
** it useless to keeping it in "_sockect_clients" 
*/
void	Server::						Squeze_vect_sockect_fd(int to_find)
{
	for (std::vector<int>::iterator it = _sockect_clients.begin(); it !=  _sockect_clients.end() ; it++)
	{
		if (*it == to_find)
		{
//			std::cout << ">>>>>>>>>>>>>>>>>> erase file descriptor == " << *it << " from server : " << _server_fd << "<<<<<<<<<<<<<<<<<<" << std::endl;
			_sockect_clients.erase(it);
			return ;
		}
	}
}

int		Server:: 						receive_data(struct pollfd	*ptr_tab_poll)
{	
	int ret = recv(ptr_tab_poll->fd, _buffer, BUFFER_SIZE, 0);
	if (ret < 0)
	{
		// if (errno != EWOULDBLOCK)
		// {
			_close_connexion = true;
		// 	std::cout << "recv() failed" << std::endl;
		// }
		std::cout << "recv() failed" << std::endl;
		return (ret);
	}
	if (ret == 0)
	{
		std::cout << "connection closed from remote side" << std::endl;
		_close_connexion = true;
		return(ret);
	}
	_buffer[ret] = '\0';

	std::cout << "\n\n" << "===============   "  << ret << " BYTES  RECEIVED   ===============\n";
	std::cout << _buffer;
	std::cout << "\n======================================================" << std::endl;

	return (ret);	
}



bool	Server::						handle_existing_connections(struct pollfd	*ptr_tab_poll)
{
	Request		request;
	int			ret;

	_close_connexion = false;
	if (ptr_tab_poll->revents & POLLOUT) {

		ret =  request.send_reponse(ptr_tab_poll);

		if (ret < 0)
			std::cout << "send() failed!" << std::endl;
		else if (ret == 0) {
			close(ptr_tab_poll->fd);
			_close_connexion = true;
		}

	}
	else if ((ret = receive_data(ptr_tab_poll)) > 0)
	{
		request.store(_buffer, ptr_tab_poll, ret);
		if (request.end_reached(ptr_tab_poll))
		{
			request.parse(ptr_tab_poll, _port);
			request.process();
			request.compose_reponse(ptr_tab_poll);
		}
	}
	if (_close_connexion)
	{
		std::cout << "close connection pollFd  = " << ptr_tab_poll->fd << "\n" <<std::endl;
		close(ptr_tab_poll->fd);
		Squeze_vect_sockect_fd(ptr_tab_poll->fd);
		ptr_tab_poll->fd = SQUEEZE;
		_compress_array = true;
	}
	return (_close_connexion);
}

void	Server::						accept_all_incoming_connections()
{
	int new_sockect = 0;
	int addrlen = sizeof(_address);
	while (new_sockect != ERROR)
	{
		new_sockect = accept(_server_fd, (sockaddr *)&_address, (socklen_t*)&addrlen);
		if (new_sockect < 0)
		{
 			if (errno != EWOULDBLOCK)
				test_error(ERROR, "accept() failed");
			new_sockect = ERROR;
		}
		else
		{
			_sockect_clients.push_back(new_sockect);//add new socket in vector socket, it has all  client of own port
			//std::cout << "new incoming connection new_sockect = " << new_sockect << std::endl;
		}
	}
}

void		Server::						set_port(int port){_port = port;}


int		Server::						getPort(){	return (_port);}
int	 	Server::						get_server_fd() {return (_server_fd);}
int	 	Server::						get_hasError() {return (_hasError);}
std::vector<int> &	Server::			get_sockect_clients(){return (_sockect_clients);}
