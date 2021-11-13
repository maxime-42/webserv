#ifndef SERVICE_HPP
#define SERVICE_HPP

#include "header.hpp"
#include "Server.hpp"
#include "Request.hpp"
#include "ParsingFile.hpp"
#include <poll.h>
#include <list>
#include <algorithm>    // std::find
#include <signal.h>
#include <string>     // std::string, std::to_string

std::string  									getInfo(std::list < std::list < std::map < std::string, std::string > > > nestedList, int port, std::string toFind);
std::string										getElem(std::list < std::list < std::map < std::string, std::string > > > nestedList, size_t lineServer, std::string elem);

class Service
{
private:
	char										_buffer[BUFFER_SIZE];
	bool										_close_connexion;
	bool										_compress_pollFds;
	struct pollfd								_pollFds[SIZE_POLLFD];
	size_t										_nfds; //number file descriptor sever
	std::list<Server> 							_listServer;
	ParsingFile									_parsing;

private:
	void										setUpService();
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
	~Service();
	Service(std::string FileName);
	bool										getHasError();
	void										runService();


};


#endif // !