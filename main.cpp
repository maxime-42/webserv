// #include "Running_server.hpp"
#include "Create_server.hpp"

namespace ft
{

	ft::Create_server apiRest(PORT);
	void						loop()
	{
		struct pollfd			tab_poll[SIZE_POLLFD];
		int						nfds = 1;
		int						ret = 1;
		ft:Create_server		apiRest;
		

		tab_poll[0].fd = apiRest.get_server_fd();//server_fd;
  		tab_poll[0].events = POLLIN;
		while (true)
		{
			ret = poll(tab_poll, nfds, (3 * 60 * 1000));//initialize the timeout to 3 minutes.
			test_error(ret, "poll() failed");
			if (ret == 0)
				test_error(ERROR, "poll() timed out.  End program.\n");
			int current_size = nfds;
			for (int i = 0; i < current_size; i++)
			{
				if(tab_poll[i].revents == 0)
					continue;
				if(tab_poll[i].revents != POLLIN)
					test_error(ERROR, "Error! revents");
				if (tab_poll[i].fd == apiRest.get_server_fd())
					// Accept_each_incoming_connection();
					;
				else
					// readable_data; 
					;
			}
		}
	}
}
int	 main(int argc, const char** argv) 
{
	ft::Create_server apiRest(PORT);
	(void)argc;
	(void)argv;
	return (0);
}