
#ifndef PARSINGFILE
#define PARSINGFILE
#define ERROR -1
#define CONTENT_SERVER 1
#define CONTENT_ 1
#define DIRECT 3
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
	std::vector<std::string> _keyWords;
	std::vector<std::string> _vectFiles;
	std::vector<std::string> _tokens;
	std::map<int, std::map<std::string, std::string> > _siteTable;//two-dimensional map  https://www.geeksforgeeks.org/implementing-multidimensional-map-in-c/
public:
	ParsingFile(/* args */);
	~ParsingFile();
	void	testError(int code); 
	void	getFile(std::string fileName);
	void	movePointer(char const **ptr);
	void	createKeyWord();
	void	displayFile();
	void	syntaxError(char const *msgError);
	void	directive(char const **ptr);
	void	directiveValue(char const **ptr);
	void	contentLocation(char const **ptr);
	void	contentServer(char const **str);
	void	directiveaName(char const **ptr);
	std::vector<std::string> tokenizeString(std::string &line, char const sep);
	// void	displayToken(std::string tokenVector);
// void	 displayToken(std::string &tokenVector);
	void	displayToken(std::vector<std::string> &tokenVector);
	void	addSiteTable();
	void	addSiteTable(std::string &str, int is);
	void	checkTokenFile(std::vector<std::string> &tokenFile);




};

#endif 
