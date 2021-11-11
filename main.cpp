#include "Server.hpp"
#include "ParsingFile.hpp"
#include "Service.hpp"


int	 main(int argc, const char** argv)
{
	if (argc > 2)
		std::cout << "Error: something go wrong about number arguments" << std::endl;
	else if (argc == 2)
	{
		Service svc(argv[1]);
	}
	else
	{
		Service svc;
	}
	(void)argc;
	(void)argv;
	return (0);
}