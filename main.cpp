#include "Server.hpp"
#include "ParsingFile.hpp"
#include "Service.hpp"


int	 main(int argc, const char** argv)
{
	Service svc;
	svc.runService();
	
	(void)argc;
	(void)argv;
	return (0);
}
// =======
// #include "ParsingFile.hpp"

// int main(int argc, char const *argv[])
// {
// 	// smallenum test = a;
// 	// std::cout << "value= " << test << std::endl;
// 	ParsingFile test;
// 	// std::string elem = test.getElem(1, "listen");
// 	// std::cout << "listen = [" << elem << "]"<< std::endl;
// 	(void)argc;
// 	(void)argv;
// 	return 0;
// }
// >>>>>>> origin/maxime
