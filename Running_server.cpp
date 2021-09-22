#include "Running_server.hpp"

namespace ft
{
	int	Running_server:: initialise_poll()
	{
		int	ret;

		std::cout << "Waiting on poll()...\n" << std::endl;
		/* Call poll() and wait 3 minutes for it to complete.      */

		ret = poll(_pollFd, _nb_fd_poll, _timeout);
		this->test_error(ret, "Error poll");
		if (ret == 0)
		{
			std::cout << "poll() timed out.  End program.\n" << std::endl;
			exit(0);
		}
		return (ret);
	}

	Running_server::Running_server()
	{
		this->start_server();
		_pollFd[0].fd = this->get_server_fd();
		_pollFd[0].events = POLLIN;
		_timeout = (1 * 60 * 1000);
		_nb_fd_poll = SIZE_POLLFD;
		std::cout << "_server_fd = " << this->get_server_fd() << std::endl;
		std::cout << "_listen_fd = " << this->get_listen_fd() << std::endl;		
		exit(0);
		int	ret;
		_end_server = false;
		while (_end_server == false)
		{
			std::cout << "Waiting on poll()...\n" << std::endl;
			ret = poll(_pollFd, _nb_fd_poll, _timeout);
			this->test_error(ret, "Error poll");
			if (ret == 0)
			{
				std::cout << "poll() timed out.  End program.\n" << std::endl;
				_end_server = true;
			}
		}
	}

	Running_server:: ~Running_server(){}
}
