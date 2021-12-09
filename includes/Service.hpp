#ifndef SERVICE_HPP
#define SERVICE_HPP

#define DEFAULT_FILE "./configFile/default.conf"
#define	NONE_EVENT 0
#include "header.hpp"
#include "Server.hpp"
#include "Request.hpp"
#include "Parse_config_file.hpp"
#include <poll.h>
#include <list>
#include <algorithm>    // std::find
#include <signal.h>
#include <string>     // std::string, std::to_string

std::string										getElem(std::list < std::list < std::map < std::string, std::string > > > nestedList, size_t lineServer, std::string elem);
/**
 * @brief 
 * this class run the program it is the entry point with clients
 */
class Service
{
private:
	Parse_config_file 							&_instance;
	char										_buffer[BUFFER_SIZE];
	bool										_close_connexion;
	bool										_compress_pollFds;
	struct pollfd								_pollFds[SIZE_POLLFD];
	size_t										_nfds; //number file descriptor sever
	std::list<Server> 							_listServer;

	int											setUpService();
	void										handlerServer(size_t &index);

	void										checkError(int error_code,  const char *  msg);
	void										accept_all_incoming_connections(Server &server);
	void										handle_existing_connections(struct pollfd	*ptr_tab_poll);
	bool										receive_data(struct pollfd	*ptr_tab_poll);
	void										squeeze_tab_poll();
	void										addFdsToPollFds(std::vector<int> &vect_socket_fd, size_t from);
	void										displayAvailableServer(/* args */);
	int											getPort(int index);


public:
	Service();
	Service(std::string FileName);
	~Service();
	bool										getHasError();
	void										runService();
};

#endif // !