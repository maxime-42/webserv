#include "Server.hpp"
#include "ParsingFile.hpp"
#include "Service.hpp"

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
	if (argc > 2)
		std::cout << "Error: something go wrong about number arguments" << std::endl;
	else if (argc == 2)
	{
		Service svc(argv[1]);
	}
	else
	{
	std::map<char, int> first;
	first['a']=10;
	first['b']=30;
  	first['c']=50;
  	first['d']=70;
	print(first);
	test(&first);
	print(first);
	
		// ParsingFile parsing;
		// std::string reponse = std::string();
		// bool ret = getInfo(parsing.getList(), 3300, "server_name", reponse);
		// if (ret == true)
		// {
		// 	std::cout << ">>>FOUND<<<" << std::endl;
		// 	std::cout << "[" << reponse << "]" << std::endl;
		// }
		// else
		// {
		// 	std::cout << ">>>Nooot FOUND<<<" << std::endl;
		// 	std::cout << "[" << reponse << "]" << std::endl;
		// }
		// Service svc;
		// (void)ret;
	}
	(void)argc;
	(void)argv;
	return (0);
}