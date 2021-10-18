
#ifndef PARSINGFILE
#define PARSINGFILE
#define ERROR -1


enum token_type
{
	brackets_open = 0,
	brackets_close,
	semicolon,
	name,
	value,
	location,
	server,
	initialized
};

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
#include <list>

class ParsingFile
{
private:
	size_t						_line;
	size_t						_column;
	std::map<std::string, std::string> _dictionary;
	std::string					_configFile;
	size_t						_nbParenthese;
	char						_characterOfLimite;
	token_type					_previousToken;//this function always stock the previous token
	std::vector<std::string> 	_keyWords;		//this vectore stock all keys word
	std::list < std::list < std::map < std::string, std::string > > > _serverTable;		//this vectore stock all keys word

public:
	ParsingFile(/* args */);
	~ParsingFile();
	void						getFile(std::string fileName);
	void						createKeyWord();
	void						displayFile();
	void						displayToken(std::vector<std::string> &tokenVector);

	void						handleCommentes(std::string &line);
	void						parsingFile();

	void						checkServerSyntaxe(size_t &i);

	std::string					getPieceOfstring(size_t &i);
	void						hasServer();
	void						syntaxError(char const *msgError);
	void						displayDirectionary(std::map<std::string, std::string> &map);
	bool						checkIfSecretWord(std::string &pieceOfString);

	void						HasBracketOpen();
	void 						hasName(std::string &directiveName, std::string & pieceOfString, size_t i);

	void						hasValue(std::string &directiveValue, std::string & pieceOfString);

	void						hasSemicolon();
	void						HasBracketClose();
	void						hasLocation(std::string &directiveName, std::string & pieceOfString);
	void						displayServerTable();

};

#endif 
