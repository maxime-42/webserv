#include "Server.hpp"
#include "ParsingFile.hpp"
#include "Service.hpp"
#include "header.hpp"
void							displayServerTable();
bool			get_location_url(int port, std::string url, void *ptrReponse);
void		displayDirectionary(std::map<std::string, std::string> &map);

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
		// displayServerTable();
		// Service svc;
		// (void)map;
		std::map<std::string, std::string> reponse;
		bool ret = get_location_url(8080, "/ww/upload/tmp/newfile", &reponse);
		if (ret == true)
		{
			std::cout << ">>>>Trouver<<<<" << std::endl;
			displayDirectionary(reponse);
		}
		else
		{
			std::cout << "Echec" << std::endl;

		}
		(void)parse;

	}
	(void)argc;
	(void)argv;
	return (0);
}