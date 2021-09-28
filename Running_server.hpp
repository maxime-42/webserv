#ifndef RUNNING_SERVER
#define RUNNING_SERVER

#include "Create_server.hpp"

namespace ft
{
	class Running_server : public Create_server
	{
	private:
		// int					_timeout;
		// // Server			_server;
		// struct pollfd		_pollFd[SIZE_POLLFD];
		// int					_nb_fd_poll;
		// bool					_end_server;

	public:
		Running_server();
  		~Running_server();
		int					initialise_poll();
	};

}

#endif
