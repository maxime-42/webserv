#include "Server.hpp"


int	 main(int argc, const char** argv) 
{
	Server apiRest(PORT);
	apiRest.start_svc();
	(void)argc;
	(void)argv;
	return (0);
}
