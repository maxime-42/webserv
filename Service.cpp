#include "Service.hpp"

/** If you really want to deeply understand everything what i did here, i recommend you to check out Parsing class
** this class it very tie to class "Parsing " and "Server"
*/

/*
** If the are some problem while parsing file, the program will shut down
** because the class Parsing going to run first 
*/

/*
**	this do nothing else than display available serveur
*/
void	Service::								displayAvailableServer(/* args */)
{
	std::cout << "\nserveur available:" << std::endl;
	for (std::list<Server>::iterator it = _listServer.begin(); it != _listServer.end(); it++)
	{
		std::cout << "server [" << it->get_server_fd() << "]" << std::endl;
	}
	std::cout << "\n" << std::endl;
}

/************************************ constructore********************************/
/*
**	check if any error happened while parsing file 
**	if there had not error setup,  displayAvailableServer, and run service
**	i display available port during the setup 
*/
Service::Service()
{
	 _instance = ParsingFile::getInstance();
	if (_instance->getErrorHappened() == true)//glance if the parsing has detected an error
	{
		std::cout << "EXIT PROGRAME" << std::endl;
		return ;
	}
	_compress_pollFds = false;
	setUpService();
	displayAvailableServer();
	runService();
}

Service:: 										Service(std::string FileName)
{
	_instance = ParsingFile::getInstance(FileName); 
	if (_instance->getErrorHappened() == true)//glance if the parsing has detected an error
	{
		std::cout << "EXIT PROGRAME" << std::endl;
		return ;
	}
	_compress_pollFds = false;
	setUpService();
	displayAvailableServer();
	runService();
}

Service::~Service(){delete _instance;}

/***************************************************************************************/

void	Service::								checkError(int error_code,  const char *  msg)
{
	if (error_code < 0)
	{
		throw(msg);
	}
}


/*
** to every closed connection, the array of poll must be squeeze too
*/
void	Service::								squeeze_tab_poll()
{

	if (_compress_pollFds)
	{
		_compress_pollFds = false;
		for (size_t i = 0; i < _nfds; i++)
		{
			if (_pollFds[i].fd == SQUEEZE)
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
**	this function work related with "setUpService", it let to get port of server
** "getElem" return data under string format so i convert this string to integer then return that integer
** if "elem" is empty string it going to be assigned to 8080 by default
*/
int		Service::								getPort(int index)
{
	int 										port;
	std::string 								elem;
	// elem = _parsing.getElem(index, "listen");
	elem = getElem(_instance->getList(), (size_t)index, "listen");

	if (elem.empty())
		elem = "8080";
	std:: stringstream ss(elem);
	ss >> port;
	std::cout << "port = [" << port << "]" << std::endl;
	return (port);
}

/*
** size of _listServer depend size of _parsing.numberOfServer()
**	first you should understand idea of Parsing class
**	each port is tie in a server object, it mean we create a server object for each port 
*/

void	Service::								setUpService()
{
	int 										port;

	std::cout << "Port available:" << std::endl;
	for (_nfds = 0; _nfds < _instance->numberOfServer(); _nfds++)
	{
		port = getPort(_nfds);
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
		_pollFds[_nfds].events = POLLIN;//Tell me when ready to read
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
/*
**g_loopback let the program loop when it assigned to true otherwise program going stop
*/
static bool										g_loopback = true;

/*
**when any signal arrived, it  stop loopback by set "_loopback" to false 
*/
void											handle_signal(int sig)
{
	g_loopback = false;
	std::cout << "Caught signal number = " << sig << std::endl;
}

void	Service::								runService()
{
	int											ret;
	try
	{
		while (g_loopback)
		{
			// for (int i = 1; i <= 64; i++) //handler any signal
				signal(SIGINT, handle_signal);
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
		std::cerr << msg_error << '\n';
	}
}