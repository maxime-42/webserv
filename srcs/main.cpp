#include "Server.hpp"
#include "ParsingFile.hpp"
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
		Service svc;
		// ParsingFile &parse = ParsingFile::getInstance("./configFile/default.conf");
		// std::string str;
		// bool ret = getInfo(8080, "index", &str, find_directive);
		// if (ret == true)
		// {
		// 	std::cout << "directive [" << str << "]" << std::endl;
		// }
		// else
		// {
		// 	std::cout << "ECHEC"  << std::endl;
		// }

		// // displayServerTable();
		// (void)parse;
	}
	(void)argc;
	(void)argv;
	return (0);
}
