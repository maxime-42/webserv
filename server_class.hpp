#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <poll.h>
#include <iostream>
#include <netinet/in.h>

#define ERROR -1
#define PORT 80


namespace ft
{
    class server
    {
    private:
        int                 _network_socket;
        int                 _listen_fd;
        struct  sockaddr_in _address;
        int                 _addrlen;
        int                 _nb_pending;
        int                 _port;
        struct pollfd       _tab_fd[200];

        void                init_struct_sockaddr();

        /* data */
    public:
        void                create_network_socket();
        void                listen_fd();   
        server(/* args */);
        server(int port, int nb_pending);

        ~server();
        void                 test_error(int error_code, char const *msg);

        void    bind_and_listen();

    };
 
}