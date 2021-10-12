#include "ParsingFile.hpp"

int main(int argc, char const *argv[])
{
	ParsingFile test;
	test.getFile("./configFile/default.conf");
	(void)argc;
	(void)argv;
	return 0;
}
