#include "Service.hpp"

/** 
 * If the are some problem while parsing congile file, the program will shut down
 * because the class Parss config file going to run first 
 */


/**
 * @brief 
 * this do nothing else than display available server socket and the port which it is tie
 * each server has own port, server object are stored in  linked list 
 * linked list contain a server object at each node 
 * this function skim the linked list and display  ports of server of each node
 * it mean each iteration is tie at a server object
 */
void	Service::								displayAvailableServer(/* args */)
{
	std::cout << "\nAvailable servers:" << std::endl << std::endl;
	for (std::list<Server>::iterator it = _listServer.begin(); it != _listServer.end(); it++)
	{
		std::cout << "server =" << it->get_server_fd() << "= port =" << it->getPort() << "="  << std::endl;
	}
	std::cout << "\n" << std::endl;
}

/**
 * @brief 
 * this constructor is called when none parameter was given to the program
 * first get instance of Parsing class, 
 * check if any error happened while parsing file 
 * if there had not error setup,  displayAvailableServer, and run service
 * i display available port during the setup 
 */
Service::Service():_instance(Parse_config_file::getInstance("./configFile/default.conf")), _compress_pollFds(false)
{
	if (_instance.getErrorHappened() == true)//glance if the parsing has detected an error
	{
		std::cout << "EXIT PROGRAM" << std::endl;
		return ;
	}
	int ret = setUpService();
	if (ret == SUCCESS)
	{
		displayAvailableServer();
		runService();
	}
}

/**
 * @brief 
 * this constructor is called when a parameter was given to the program
 * first get instance of Parsing class, 
 * check if any error happened while parsing file 
 * if there had not error setup,  displayAvailableServer, and run service
 * i display available port during the setup 
 * @param FileName configle file
 */
Service:: 										Service(std::string FileName):_instance(Parse_config_file::getInstance(FileName)), _compress_pollFds(false)
{
	if (_instance.getErrorHappened() == true)//glance if the parsing has detected an error
	{
		std::cout << "EXIT PROGRAM" << std::endl;
		return ;
	}
	int ret = setUpService();
	if (ret == SUCCESS)
	{
		displayAvailableServer();
		runService();
	}

}

Service::~Service(){}

/***************************************************************************************/

/**
 * @brief 
 * throw a exeception if error_code is less than 0
 * @param error_code 
 * @param msg the text message to display when teh error is throw
 */
void	Service::								checkError(int error_code,  const char *  msg)
{
	if (error_code < 0)
	{
		throw(msg);
	}
}


/**
 * @brief 
 * to every closed connection, the array of poll must be squeeze too
 * at each time the connection, closed the poll array have to be squeeze
 * when connection has closed, the value of poll array is set to "SQUEEZE" an _compress_pollFds equal to true
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


/**
 * @brief Set the Up Service object
 *  @all_ports : this vector contains all port of config file
 * the goal of this function it is to create a linked list which has an instance of server class in each node
 * there will been as much node than the number of port
 * each port is tie in a server object, it mean we create a server object for each port 
 * @return int 0 for a success or error for an -1 error happened some where
 */
int	Service::								setUpService()
{
	std::vector<int> &all_ports = Parse_config_file::get_ports();

	for (_nfds = 0; _nfds < _instance.numberOfServer(); _nfds++)
	{
		Server server;/* instance a server over each iteration*/
		if (all_ports.size() > _nfds)
		{
			server.set_port(all_ports[_nfds]);
		}
		server.setup();
		if (server.get_hasError() == true)/*check if a error happened while setup of server it stop the loop*/
			return (ERROR); /* return this when an error occurred while the setup of server */
		_pollFds[_nfds].fd = server.get_server_fd();
		_pollFds[_nfds].events = POLLIN; //Tell me when ready to read
		_listServer.push_back(server);
	}
	return (SUCCESS);
}

/**
 * @brief 
 * this function try to add new socket into  poll array if the size had changed
 * @param vect_socket_client , if  ever "vect_socket_client.size()" ist equal to "old_size"  nothing will happen
 * @param old_size is the previous size of @paramter vect_socket_client
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


/**
 * @brief 
 * allow to handle event
 * event can be on existed files descriptor or can be on  new files file descriptor which has to be accept by a server like  new file descriptor  
 * 
 * this function do two thing main:
 * 		one: if new connection came, add it in the vector "_socket_client" and poll array
 * 		two: if is existing socket go read request, and send reponse
 * 
 * it important to know than "_listServer"  this linked list contain a server on each node
 * this function loop through this list to look up the socket client which has triggered even
 *
 *	Each server have own vector of "sokect client" which are connected to it
 *
 *	the loop allow to loop through each Server inside linked list
 *
 * when event is  new on  new files file descriptor:
 * 		-> this file descriptor is add in an array  which belong server it has send request
 * 		-> likewise, this file descriptor is add in array of poll too
 * 
 *	when event is   on  existing file descriptor:
 *		-> go head, to recovery data inside
 *
 * 	variable "old_size" is initialized at size of array "vect_socket_client" this let me to update poll array :
 *	if the size of "_sockect_clients" has grown, variable "old_size" contain the old size of vect_socket_client, 
 *	with the difference between new size of "_sockect_clients"  and old size allow to update poll array with function addFdsToPollFds(..))
 * @param index on file descriptor of server to handler
 */
void	Service::								handlerServer(size_t &index)
{
	size_t	old_size;
	for (std::list<Server>::iterator it  = _listServer.begin(); it != _listServer.end(); it++)
	{
		Server	&currentServer = *it;
		std::vector<int> & vect_socket_client = currentServer.get_sockect_clients();
		old_size = vect_socket_client.size();
		if (_pollFds[index].fd == currentServer.get_server_fd())// meaning new incoming connection
		{
			currentServer.accept_all_incoming_connections();// new socket has been added to "_sockect_clients"
			addFdsToPollFds(vect_socket_client, old_size);// poll array should update to by adding the new socket on it
		}
		else if ((std::find(vect_socket_client.begin(), vect_socket_client.end(), _pollFds[index].fd)) != vect_socket_client.end())
		{
			//std::cout << "========= handle existing connection ===============" << std::endl;
			_close_connexion = currentServer.handle_existing_connections(&_pollFds[index]); // return true if the connection is closed
			if (_close_connexion == true)
			{
				_compress_pollFds = true;// In this case the array  poll will be  squeeze
			}
		}
	}
}

/**
 * @brief
 * this function triggered when a signal ctrl-c was sended
 * throw exception for stopping running program
 * @param sig value of signal
 */
void											handle_signal(int sig)
{
	std::cout << "Caught signal number = " << sig << std::endl;
}

/**
 * @brief 
 * this is the loop of program
 * function "signal" let to catch when a signal and stopping the program
 * function poll expect a event occurs
 * the loop "for" skim the array of poll to check if there a file descriptore has event
 * "handlerServer" it called only if an event happened to one of file descriptor of array poll 
 * _pollFds[index].revents == 0 if  there are none event
 */
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
				return ;//ici met throw("exit program");
			checkError(ret, "poll() failed");
			if (ret == 0)
				throw("poll() timed out");
			for (size_t index = 0; index < _nfds; index++) //loop through the array of sokect to monitoring an event 
			{
				//std::cout << "==========_pollFds[index].fd = " << _pollFds[index].fd << " index = " << index << "===========" << std::endl;
				if (_pollFds[index].revents == NONE_EVENT)//loop as long the are not event happened
					continue;
				handlerServer(index);// an event happened, handle it
			}
			squeeze_tab_poll();
		}
	}
	catch (const char * msg_error)
	{
		std::cerr << msg_error << '\n';
	}
}
