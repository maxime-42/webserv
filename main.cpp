#include "Server.hpp"
#include "ParsingFile.hpp"
#include "Service.hpp"
#include "header.hpp"

typedef std::list < std::list < std::map < std::string, std::string > > > t_nested_list;

bool		getInfo(std::list < std::list < std::map < std::string, std::string > > >nestedList, int port, std::string reponseToFind, std::string &reponse);
bool		getInfo(int port, std::string elemToFind, void *reponse, bool(*callback)(t_single_list & secondList, std::string &elemToFind, void *ptrReponse));
bool		find_directive(t_single_list & secondList, std::string &elemToFind, void *ptrReponse);
bool		find_location(t_single_list & secondList, std::string &elemToFind, void *ptrReponse);
void		print_string_dictionnary(std::map<std::string, std::string> &first);
void		test_getInfo();


int	 main(int argc, const char** argv)
{
	ParsingFile  & instance1 = ParsingFile::getInstance("./configFile/default.conf");
	ParsingFile  & instance2 = ParsingFile::getInstance("./configFile/default.conf");
	std::cout << "instance ParsingFile 1 addr = " << &instance1 << std::endl; 
	std::cout << "instance ParsingFile 2 addr = " << &instance2 << std::endl;
	if (argc > 2)
		std::cout << "Error: something go wrong about number arguments" << std::endl;
	else if (argc == 2)
	{
		Service svc(argv[1]);
	}
	else
	{
		test_getInfo();
		Service svc;
	}
	(void)argc;
	(void)argv;
	return (0);
}