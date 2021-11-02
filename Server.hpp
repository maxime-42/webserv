#ifndef	SERVER_HPP
#define	SERVER_HPP
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <poll.h>
#include <iostream>
#include <netinet/in.h>
#include <sys/ioctl.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>

#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <algorithm>
#include <cstring>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <map>

#define ERROR		-1
#define PORT	    8080
#define SIZE_POLLFD	300
#define BUFFER_SIZE	30000
#define TIMEOUT		(3 * 60 * 1000)
#define NB_PENDING	SIZE_POLLFD

class Server
{
    private:
        int						_server_fd;
        int						_listen_fd;
        char					_buffer[BUFFER_SIZE];
        struct	sockaddr_in		_address;
        int						_port;
        int						_timeout;
        bool					_close_connexion;
        bool					_compress_array;

        struct pollfd			_tab_poll[SIZE_POLLFD];
        int						_nfds; //number file descriptor sever in _tab_poll


    public:
        Server();
        Server(int port);
        ~Server();
        void					setup();
        void					test_error(int error_code, char const *msg);
        int						get_server_fd();
        int						get_listen_fd();
        void					setup_poll(); 
        void					start_svc();
        void					set_tab_poll(int fd);
        void					handle_existing_connections(struct pollfd	*_tab_poll);
        void					squeeze_tab_poll();
        void					accept_all_incoming_connections();
        bool					receive_data(struct pollfd	*_tab_poll);
        void					clean_all_socket();



        void                    parse_request(std::string r, std::map<std::string, std::string> & request,
                                                        std::map<std::string, std::string> & reponse);
        void                    process_request(std::map<std::string, std::string> request,
                                                        std::map<std::string, std::string> & reponse);
        void                    send_reponse(int socket, std::map<std::string, std::string> & reponse);
        void                    process_GET(std::map<std::string, std::string> request, std::map<std::string, std::string> & reponse);
        void                    process_POST(std::map<std::string, std::string> request, std::map<std::string, std::string> & reponse);
        void                    process_DELETE(std::map<std::string, std::string> request, std::map<std::string, std::string> & reponse);

};

#endif