#include "Service.hpp"

/** If you really want to deeply understand everything what i did here, i recommend you to check out Parsing class
** this class it very tie to class "Parsing " and "Server"
*/

/*
** If the are some problem while parsing file, nothing going to happen here, the program will shut down
** because the class Parsing will run first 
*/


Service::Service(/* args */):_hasError(false)
{
	_compress_pollFds = false;
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

/*
** to every closed connection, the array of poll must be squeeze
*/
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
** size of _listServer depend size of _parsing.numberOfServer()
**	first you should understand idea of Parsing class
**	each port is tie in a server object, it mean we create a server object for each port 
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
/*
** add new file descriptor to poll array, 
*/
void	Service::								addFdsToPollFds(std::vector<int> &vect_socket_fd, size_t tmpSize)
{
	for (size_t i = tmpSize; i < vect_socket_fd.size(); i++)
	{
		_pollFds[_nfds].fd = vect_socket_fd[i];
		_pollFds[_nfds].events = POLLIN;
		_nfds++;
	}
}

/*
**	the idea behind this function is to check the file descriptor _pollFds[index].fd is already existing or if it is new connection 
**	each server have own vector file descriptor which are connected to it
**	the loop allow to loop through each Server inside list
**
** 	variable "tmp_size" let me to update poll array :
**	if the size of socket array of server change, i can copy the new file descriptor to array poll
**	if the size of socket array of server grow, variable "tmp_size" contains the old size, 
**	with the difference between new size and old size let to update poll array with addFdsToPollFds(..))
*/
void	Service::								handlerServer(size_t &index)
{
	size_t	tmpSize;
	for (std::list<Server>::iterator it  = _listServer.begin(); it != _listServer.end(); it++)
	{
		Server	&currentServer = *it;
		std::vector<int> & vect_socket_fd = currentServer.get_vect_socket_fd();
		tmpSize = vect_socket_fd.size();
		if (_pollFds[index].fd == currentServer.get_server_fd())
		{
			currentServer.accept_all_incoming_connections();
			addFdsToPollFds(vect_socket_fd, tmpSize);
		}
		else if ((std::find(vect_socket_fd.begin(), vect_socket_fd.end(), _pollFds[index].fd)) != vect_socket_fd.end())
		{
			std::cout << "=========connexion existing===============" << std::endl;
			_close_connexion = currentServer.handle_existing_connections(&_pollFds[index]); // return true if the connection is closed
			if (_close_connexion == true)
			{
				_compress_pollFds = true;// In this case the array  poll will be  squeeze
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