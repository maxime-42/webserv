#include "Server.hpp"
#include "ParsingFile.hpp"
#include "Service.hpp"
#include "header.hpp"
void							displayServerTable();

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
		ParsingFile &parse = ParsingFile::getInstance("/home/lenox/branch_maxime/configFile/default.conf");
		// test_getInfo();
		displayServerTable();
		// Service svc;
		// (void)map;
		(void)parse;
	}
	(void)argc;
	(void)argv;
	return (0);
}