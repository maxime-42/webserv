#ifndef	DEFINE_HPP
#define DEFINE_HPP

/*this file contain all define of projet*/

#define ERROR		-1
#define PORT		12345
#define NB_PENDING	30
#define SIZE_POLLFD	300

void							test_error(int error_code, char const *msg)
{
	if (error_code < 0)
	{
		std::cout  << msg << std::endl;
		// close(_server_fd);
		exit (ERROR);
	}
}
#endif
