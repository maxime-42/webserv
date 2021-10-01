// #include "Running_server.hpp"
#include "Create_server.hpp"


int	 main(int argc, const char** argv) 
{
	ft::Create_server apiRest(PORT);
	apiRest.start_svc();
	(void)argc;
	(void)argv;
	return (0);
}