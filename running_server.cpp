#include "server.hpp"

#define SIZE_POLLFD 200

class running : public server
{
	private:
  		struct pollfd		_pollFd[SIZE_POLLFD];
	public:
		running();
  		~running();
		void			setUp_listeningEvent();
		void			check_event();
		void			event_is_listening();
		void			accept_event();
		void			adding_fd_in_pollFd();

}

namespace ft
{
	void	ruuning_server()
	{
		struct pollfd		_tab_fd[200];
		while (true)
		{
		}
	}
}
