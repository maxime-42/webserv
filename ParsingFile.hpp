
#ifndef PARSINGFILE
#define PARSINGFILE
#include <iostream>
#include <fstream>
#include <vector>

class ParsingFile
{
private:
	std::vector<std::string> vectFiles;
	/* data */
public:
	ParsingFile(/* args */);
	~ParsingFile();
	void	testError(int code);
	void	getFile(std::string fileName);

};


#endif 
