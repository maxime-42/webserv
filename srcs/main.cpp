#include "Server.hpp"
#include "ParsingFile.hpp"
#include "Service.hpp"
#include "header.hpp"
#include "Cgi.hpp"
	   
int	 main(int argc, const char** argv, char **env)
{
	if (argc > 2)
		std::cout << "Error: something go wrong about number arguments" << std::endl;
	else if (argc == 2)
	{
		Service svc(argv[1]);
	}
	else
	{
		//Cgi	cgi("root/myphp.php", "/usr/bin/php-cgi", env);
		Cgi	cgi("/usr/bin/php-cgi", env);
		Service svc;
	}
	(void)env;
	(void)argc;
	(void)argv;
	return (0);
}
