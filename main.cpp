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

void	print(std::map<char, int> &first)
{
	for (std::map<char, int>::iterator  itr = first.begin(); itr != first.end(); ++itr) 
	{
        std::cout << '\t' << itr->first << '\t' << itr->second << '\n';
	}
	std::cout << "\n" << std::endl;
}

int	 main(int argc, const char** argv)
{
	// ParsingFile  *instance1 = ParsingFile::getInstance("./configFile/default.conf");
	// std::cout << "size = " << instance1->numberOfServer() << std::endl; 
	// std::cout << "addresse instace 1 	= " << instance1 << std::endl; 
	// if (argc > 2)
	// 	std::cout << "Error: something go wrong about number arguments" << std::endl;
	// else if (argc == 2)
	// {
	// 	Service svc(argv[1]);
	// }
	// else
	// {
		Service svc;
	// 	// (void)ret;
	// }
	// delete instance;
	(void)argc;
	(void)argv;
	return (0);
}