#include "Running_server.hpp"
x
namespace ft
{
	Running_server()
	{
		_pollFd[0].fd = _server.get_network_socket();
		_pollFd[0].events = POLLIN;
		 /* Initialize the timeout to 3 minutes.*/
		_timeout = (3 * 60 * 1000);

	}
}
