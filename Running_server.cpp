#include "Running_server.hpp"

namespace ft
{
	int	Running_server:: initialise_poll()
	{
		int	ret;
		printf("Waiting on poll()...\n");
		/* Call poll() and wait 3 minutes for it to complete.      */
		ret = poll(_pollFd, _nb_fd_poll, _timeout);
		this->test_error(ret, "Error poll");
		if (ret == 0)
		{
			std::cout << "poll() timed out.  End program.\n" << std::endl;
			exit(0);
		}
		return (0);
	}

// 	Running_server():_pollFd[0].fd(_server.get_network_socket()), _pollFd[0].events(POLLIN), _timeout(3 * 60 * 1000), nb_fd_poll(SIZE_POLLFD), _end_server(false)
	Running_server::Running_server()
	{
		_pollFd[0].fd = this->get_network_socket();
		_pollFd[0].events = POLLIN;
		_timeout = (3 * 60 * 1000);
		_nb_fd_poll = SIZE_POLLFD;
		_end_server = false;
		while (_end_server == false)
		{
			initialise_poll();
		}
	}
}
