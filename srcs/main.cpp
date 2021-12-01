#include "Server.hpp"
#include "ParsingFile.hpp"
#include "Service.hpp"
#include "header.hpp"
#include "Cgi.hpp"
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
		ParsingFile &parse = ParsingFile::getInstance("./configFile/default.conf");
		displayServerTable();
		// Cgi cgi("myphp.php", 8080);

		(void)parse;
	}
	(void)argc;
	(void)argv;
	return (0);
}
