#ifndef	DEFINE_HPP
#define DEFINE_HPP

/*this file contain all define of projet*/

#define ERROR		-1
#define PORT		12345
#define SIZE_POLLFD	300
#define BUFFER_SIZE	30000
#define TIMEOUT		(3 * 60 * 1000)
#define NB_PENDING	SIZE_POLLFD
#define DATA	"HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 12\n\nHello world!"


#endif
