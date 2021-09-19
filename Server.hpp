#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <poll.h>
#include <iostream>
#include <netinet/in.h>

#define ERROR -1


namespace ft
{
	class Server
	{
	private:
		int			_network_socket;
		int			_listen_fd;
		struct  sockaddr_in	_address;
		int			_addrlen;
		int			_port;
		int			_nb_pending;
		int			_opt = 1;



	public:
		Server();
		Server(int port, int nb_pending);
		~Server();
		void			create_network_socket();
		void			listen_fd();   
		void			test_error(int error_code, char const *msg);
		void			bind_and_listen();

	private:	  
		void			init_struct_sockaddr();
    };
}
