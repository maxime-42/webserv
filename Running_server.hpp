#include "server.hpp"

#define SIZE_POLLFD 300

namespace ft
{
	class Running_Server
	{
	private:
		int			_timeout;
		Server			_server;
  		struct pollfd		_pollFd[SIZE_POLLFD];
	public:
		Running_Server();
  		~Running_Server();
		void			setUp_listeningEvent();
		void			check_event();
		void			event_is_listening();
		void			accept_event();
		void			adding_fd_in_pollFd();

	}

}
