#include "Server.hpp"
#include "ParsingFile.hpp"
#include "Service.hpp"
#include "header.hpp"

typedef std::list < std::list < std::map < std::string, std::string > > > t_nested_list;

bool  getInfo(std::list < std::list < std::map < std::string, std::string > > >nestedList, int port, std::string reponseToFind, std::string &reponse);

void	test(void *ptr)
{
	std::map<char, int> *Mymap = (std::map<char, int> *) ptr;
	(*Mymap)['a'] = 800;
}

void	print(std::map<std::string, std::string> &first)
{
	for (std::map<std::string, std::string>::iterator  itr = first.begin(); itr != first.end(); ++itr) 
	{
        std::cout << '\t' << itr->first << '\t' << itr->second << '\n';
	}
	std::cout << "\n" << std::endl;
}

bool  getInfo(t_nested_list firstList, int port, std::string elemToFind, void *reponse, bool(*callback)(t_single_list & secondList, std::string &elemToFind, void *ptrReponse));
bool		find_location(t_single_list & secondList, std::string &elemToFind, void *ptrReponse);
bool		find_directive(t_single_list & secondList, std::string &elemToFind, void *ptrReponse);

int	 main(int argc, const char** argv)
{
	// ParsingFile  *instance = ParsingFile::getInstance("./configFile/default.conf");
	// std::cout << "size = " << instance1->numberOfServer() << std::endl; 
	// std::cout << "addresse instace 1 	= " << instance1 << std::endl; 
	if (argc > 2)
		std::cout << "Error: something go wrong about number arguments" << std::endl;
	else if (argc == 2)
	{
		// Service svc(argv[1]);
	}
	else
	{
		// std::map<std::string, std::string> reponse;
		// std::string reponse;
		// // bool ret = getInfo(instance->getList(), 3300, "/", &reponse, find_location);
		// bool ret = getInfo(instance->getList(), 800, "allow", &reponse, find_directive);
		// if (ret)
		// {
		// 	std::cout << ">>>>>Exit<<<<" << std::endl;
		// 	std::cout << "reponse == " << reponse << std::endl;
		// 	// print(reponse);
		// }
		// else
		// {
		// 	std::cout << "<<< Nooot exit<<<<" << std::endl;
		// }
		Service svc;
	}
	// delete instance;
	(void)argc;
	(void)argv;
	return (0);
}