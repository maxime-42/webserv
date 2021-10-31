#include "Service.hpp"

Service::Service(/* args */):_hasError(false)
{
	std::cout << "Default constructor Service" << std::endl;
	setUpService();
	
	(void)_parsing;
}

Service:: 										Service(std::string FileName):_parsing(FileName){}

Service::~Service(){}

void	Service::								checkError(int error_code,  const char *  msg)
{
	if (error_code < 0)
	{
		throw(msg);
	}
}

void	Service::								squeeze_tab_poll()
{

	if (_compress_pollFds)
	{
		_compress_pollFds = false;
		for (size_t i = 0; i < _nfds; i++)
		{
			if (_pollFds[i].fd == ERROR)
			{
				for(size_t j = i; j < _nfds - 1; j++)
				{
					_pollFds[j].fd = _pollFds[j+1].fd;
				}
				i--;
				_nfds--;
			}
		}
	}
}

/*
**	return true if an error happened somewhere
*/
bool	Service::								getHasError	(){return (_hasError);}


/*
**	setup each  server 
** _parsing that object contains files configuration
*/

void	Service::								setUpService()
{
	int 										port;
	std::string 								elem;
	for (_nfds = 0; _nfds < _parsing.numberOfServer(); _nfds++)
	{
		elem = _parsing.getElem(_nfds, "listen");
		std:: stringstream ss(elem);
		ss >> port;
		std::cout << "port = [" << port << "]" << std::endl;
		Server server(port);
		server.setup();
		_pollFds[_nfds].fd = server.get_server_fd();
		_pollFds[_nfds].events = POLLIN; //Tell me when ready to read
		_listServer.push_back(server);
	}
}

void	Service::								addFdsToPollFds(std::vector<int> &vect_socket_fd, size_t from)
{
	for (size_t i = from; i < vect_socket_fd.size(); i++)
	{
		_pollFds[_nfds].fd = vect_socket_fd[i];
		_pollFds[_nfds].events = POLLIN;
		_nfds++;
	}
}

void	Service::								handlerServer(size_t &index)
{
	size_t	from;
	for (std::list<Server>::iterator it  = _listServer.begin(); it != _listServer.end(); it++)
	{
		Server	&currentServer = *it;
		std::vector<int> & vect_socket_fd = currentServer.get_vect_socket_fd();
		from = vect_socket_fd.size();
		if (_pollFds[index].fd == currentServer.get_server_fd())
		{
			currentServer.accept_all_incoming_connections();
			addFdsToPollFds(vect_socket_fd, from);
		}
		else if ((std::find(vect_socket_fd.begin(), vect_socket_fd.end(), _pollFds[index].fd)) != vect_socket_fd.end())
		{
			std::cout << "=========connexion existing===============" << std::endl;
			_close_connexion = currentServer.handle_existing_connections(&_pollFds[index]);
			if (_close_connexion)
			{
				_compress_pollFds = true;
			}
		}
	}
}

void	Service::								runService()
{
	int											ret;
	try
	{
		while (true)
		{
			ret = poll(_pollFds, _nfds, TIMEOUT);
			checkError(ret, "poll() failed");
			if (ret == 0)
				throw("poll() timed out");
			for (size_t index = 0; index < _nfds; index++) //loop through the array of file descriptor to monitoring an event 
			{
				if (_pollFds[index].revents == 0)//loop as long the are not event happened
					continue;
				std::cout << "==========_pollFds[index].fd = " << _pollFds[index].fd << " index = " << index << "===========" << std::endl;
				handlerServer(index);
			}
		}
		squeeze_tab_poll();
	}
	catch (const char * msg_error)
	{
		_hasError = true;
		std::cerr << msg_error << '\n';
	}
}