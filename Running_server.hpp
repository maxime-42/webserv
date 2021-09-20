#ifndef RUNNING_SERVER
#define RUNNING_SERVER

#include "Server.hpp"

namespace ft
{
	class Running_server : public Server
	{
	private:
		int			_timeout;
		// Server			_server;
		struct pollfd		_pollFd[SIZE_POLLFD];
		int			_nb_fd_poll;
		bool			_end_server;

	public:
		Running_server();
  		~Running_server();
		int			initialise_poll();
// 		void			setUp_listeningEvent();
// 		void			check_event();
// 		void			event_is_listening();
// 		void			accept_event();
// 		void			adding_fd_in_pollFd();
	};

}

#endif
