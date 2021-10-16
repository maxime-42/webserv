
#ifndef PARSINGFILE
#define PARSINGFILE
#define ERROR -1
#define CONTENT_LOCATION 14
#define CONTENT_SERVER 15
#define DIRECT 3

#define DIRECTIVE_NAME 0
#define SEPARATOR "\t\f\r\n\v {} ;"


#define INIT 0
#define CONTENTE_SERVER 1
#define CONTENTE_LOCATION 2
#define DIRECTIVE_NAME 3
#define DIRECTIVE_VALUE 4

#define NOT_FOUND -1


#include <bitset>
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
	std::string _configFile;
	size_t		_nbParenthese;
	std::bitset<8> _prevElem;
	bool		_syntaxError;
	std::vector<std::string> _keyWords;
	std::vector<std::string> _vectFiles;
	std::vector<std::string> _tokens;				   
	std::vector<std::map<std::string, std::string> > _arrayOfSite; //two-dimensional map  https://www.geeksforgeeks.org/implementing-multidimensional-map-in-c/
public:
	ParsingFile(/* args */);
	~ParsingFile();
	void				testError(int code); 
	void				getFile(std::string fileName);
	void				createKeyWord();
	void				displayFile();
	void				displayToken(std::vector<std::string> &tokenVector);

	void				handleCommentes(std::string &line);
	void				parsingFile();

	void				skipWhiteSpace(size_t &i);
	void				checkServerSyntaxe(size_t &i);

	std::string			pieceOfConfigFile(size_t &i);
	void				checkServerSyntaxe(size_t &i, std::string &pieceOfString);
	void				syntaxError(char const *msgError);
	void				displayDirectionary(std::map<std::string, std::string> &map);
	int					ParsingFile::wordsRecognizer(std::string &pieceOfString);
	int					wordsRecognizer(std::string &pieceOfString);

};

#endif 
