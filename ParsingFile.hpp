
#ifndef PARSINGFILE
#define PARSINGFILE
#define ERROR -1
#define CONTENT_LOCATION 0
#define CONTENT_SERVER 1
#define DIRECT 3
#define SEPARATOR "\t\f\r\n\v {} ;"

#include <iostream>
#include <fstream>
#include <vector>
#include <string.h>
#include <map>
#include <sstream>
#include <string>
#include <stdlib.h>
#include <algorithm>

class ParsingFile
{
private:
	std::string _file;
	bool		_syntaxError;
	std::vector<std::string> _keyWords;
	std::vector<std::string> _vectFiles;
	std::vector<std::string> _tokens;				   
	std::vector<std::map<std::string, std::string> > _arrayOfSite; //two-dimensional map  https://www.geeksforgeeks.org/implementing-multidimensional-map-in-c/
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
	void	checkFileSyntax(char const **str);
	void	directiveaName(char const **ptr);
	// std::vector<std::string> tokenizeString(std::string &line, char const sep);
	// void	displayToken(std::string tokenVector);
// void	 displayToken(std::string &tokenVector);
	void	displayToken(std::vector<std::string> &tokenVector);
	void	addSiteTable();
	void insertNameValueInDictionary(std::vector<std::string> &token, std::map<std::string, std::string> &dictionary);
	std::vector<std::string>	 formatToken(std::string &line, int content);

	std::string		getValueFromString(std::string &str);
	void	pushInArrayOfSite(std::vector<std::string> &tokenFile);
	void	displayDirectionary(std::map<std::string, std::string> &map);
	// std::vector<std::string> tokenizeString(std::string &line, char * separator);
	std::vector<std::string> tokenizeString(std::string &line, char *seperator);


};

#endif 
