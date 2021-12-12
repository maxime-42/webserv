#include "Server.hpp"
#include "Parse_config_file.hpp"
#include "Service.hpp"
#include "header.hpp"
#include "Cgi.hpp"
#include "utile.hpp"

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
		// Service svc;
		Parse_config_file &parse = Parse_config_file::getInstance("./configFile/default.conf");
		// displayServerTable();
		(void)parse;
	}
	(void)argc;
	(void)argv;
	return (0);
}
