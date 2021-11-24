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
	std::cout << "\nServeur available:" << std::endl;
	for (std::list<Server>::iterator it = _listServer.begin(); it != _listServer.end(); it++)
	{
		std::cout << "server [" << it->get_server_fd() << "] port [" << it->getPort() << "]"  << std::endl;
		
	}
	std::cout << "\n" << std::endl;
}

/************************************ constructore********************************/
/*
**	check if any error happened while parsing file 
**	if there had not error setup,  displayAvailableServer, and run service
**	i display available port during the setup 
*/
Service::Service():_instance(ParsingFile::getInstance("./configFile/default.conf"))
{
	if (_instance.getErrorHappened() == true)//glance if the parsing has detected an error
	{
		std::cout << "EXIT PROGRAME" << std::endl;
		return ;
	}
	_compress_pollFds = false;
	setUpService();
	displayAvailableServer();
	runService();
}

Service:: 										Service(std::string FileName):_instance(ParsingFile::getInstance(FileName))
{
	if (_instance.getErrorHappened() == true)//glance if the parsing has detected an error
	{
		std::cout << "EXIT PROGRAME" << std::endl;
		return ;
	}
	_compress_pollFds = false;
	setUpService();
	displayAvailableServer();
	runService();
}

Service::~Service(){}

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
** size of _listServer depend size of _parsing.numberOfServer()
**	first you should understand idea of Parsing class
**	each port is tie in a server object, it mean we create a server object for each port 
*/

void	Service::								setUpService()
{
	std::vector<int> &all_ports = ParsingFile::get_ports();

	for (_nfds = 0; _nfds < _instance.numberOfServer(); _nfds++)
	{
		Server server;
		if (all_ports.size() > _nfds)
		{
			server.set_port(all_ports[_nfds]);
		}
		server.setup();
		_pollFds[_nfds].fd = server.get_server_fd();
		_pollFds[_nfds].events = POLLIN; //Tell me when ready to read
		_listServer.push_back(server);
	}
}

/*
** this function try to add new socket into  poll array if the size had changed
**	@paramter old_size is the previous size of @paramter vect_socket_client
**	if  ever "vect_socket_client.size()" ist equal to "old_size"  nothing will happen
*/
void	Service::								addFdsToPollFds(std::vector<int> &vect_socket_client, size_t old_size)
{
	for (size_t i = old_size; i < vect_socket_client.size(); i++)
	{
		_pollFds[_nfds].fd = vect_socket_client[i];
		_pollFds[_nfds].events = POLLIN;//Tell me when ready to read
		_pollFds[_nfds].revents = NONE_EVENT;//set to none event, like that if value  changed it mean event a happened
		_nfds++;
	}
}

/*

** this function two thing main:
** one: if new connection came , add it in the vector "_socket_client" and poll array
** two: if is existing socket go read request, and send reponse
** 
**	it important to know than "_listServer"  this linked list contain a server on each node
** this function loop through this list to look up the socket client which has triggered even
**
**	Each server have own vector of "sokect client" which are connected to it
**
**	the loop allow to loop through each Server inside linked list
**
** 	variable "old_size" let me to update poll array :
**	if the size of "_sockect_clients" has grown, variable "old_size" contain the old size of vect_socket_client, 
**	with the difference between new size of "_sockect_clients"  and old size allow to update poll array with function addFdsToPollFds(..))
*/
void	Service::								handlerServer(size_t &index)
{
	size_t	old_size;
	for (std::list<Server>::iterator it  = _listServer.begin(); it != _listServer.end(); it++)
	{
		Server	&currentServer = *it;
		std::vector<int> & vect_socket_client = currentServer.get_sockect_clients();
		old_size = vect_socket_client.size();
		if (_pollFds[index].fd == currentServer.get_server_fd())// meaning new caming connection
		{
			currentServer.accept_all_incoming_connections();// new socket has been add to "_sockect_clients"
			addFdsToPollFds(vect_socket_client, old_size);// poll array should update to by adding the new socket on it
		}
		else if ((std::find(vect_socket_client.begin(), vect_socket_client.end(), _pollFds[index].fd)) != vect_socket_client.end())
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
**when any signal arrived, it  stop loopback by set "_loopback" to false 
*/
void											handle_signal(int sig)
{
	std::cout << "Caught signal number = " << sig << std::endl;
	throw("stoooped by CTRL-C");

}


void	Service::								runService()
{
	int											ret;

	try
	{
		while (true)
		{
			signal(SIGINT, handle_signal);
			ret = poll(_pollFds, _nfds, TIMEOUT);
			if (ret == ERROR && errno == EINTR)// check if poll is termined by ctrl-c 
				return ;
			checkError(ret, "poll() failed");
			if (ret == 0)
				throw("poll() timed out");
			for (size_t index = 0; index < _nfds; index++) //loop through the array of sokect to monitoring an event 
			{
				std::cout << "==========_pollFds[index].fd = " << _pollFds[index].fd << " index = " << index << "===========" << std::endl;
				if (_pollFds[index].revents == NONE_EVENT)//loop as long the are not event happened
					continue;
				handlerServer(index);// like event happened go to handler this
			}
		}
		squeeze_tab_poll();
	}
	catch (const char * msg_error)
	{
		std::cerr << msg_error << '\n';
	}
}