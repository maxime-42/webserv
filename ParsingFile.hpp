
#ifndef PARSINGFILE
#define PARSINGFILE
#define ERROR -1
#include <iostream>
#include <fstream>
#include <vector>
#include <string.h>
#include <map>
#include <sstream>
#include <string>
#include <stdlib.h>

class ParsingFile
{
private:
	std::string _file;
	bool		_syntaxError;
	std::vector<std::string> _tokenAvailable;

	std::vector<std::string> _vectFiles;
	std::vector<std::string> _tokens;
	//map< key_1_type, map< key_2_type, value_type> > object; source => https://www.geeksforgeeks.org/implementing-multidimensional-map-in-c/
	std::map<int, std::map<std::string, std::string> > _twoDimensionalMap;//two-dimensional map
public:
	ParsingFile(/* args */);
	~ParsingFile();
	void	testError(int code); 
	void	getFile(std::string fileName);
	void	tokenizer(std::string &line);
	void	parsingServer(char const **str);
	void	movePointer(char const **ptr);
	void	addtokenAvailable();
	void	displayTokenAvailable();
	void	displayFile();
	void	syntaxError(char const *msgError);
	void	directiveName(char const **ptr);
	void	directiveValue(char const **ptr);
	void	contentLocation(char const **ptr);

};

#endif 
