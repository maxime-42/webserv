
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

class ParsingFile
{
private:
	std::string					_configFile;
	size_t						_nbParenthese;
	std::bitset<8> 				_prevElem;
	token_type					_previousToken;
	bool						_syntaxError;
	std::vector<std::string> 	_keyWords;
	std::vector<std::string> 	_vectFiles;
	std::vector<std::string> 	_tokens;				   
	std::vector<std::map<std::string, std::string> > _arrayOfSite;

public:
	ParsingFile(/* args */);
	~ParsingFile();
	void						testError(int code); 
	void						getFile(std::string fileName);
	void						createKeyWord();
	void						displayFile();
	void						displayToken(std::vector<std::string> &tokenVector);

	void						handleCommentes(std::string &line);
	void						parsingFile();

	void						skipWhiteSpace(size_t &i);
	void						checkServerSyntaxe(size_t &i);

	std::string					getPieceOfstring(size_t &i);
	void						hasServer();
	void						syntaxError(char const *msgError);
	void						displayDirectionary(std::map<std::string, std::string> &map);
	bool						checkIfSecretWord(std::string &pieceOfString);

	void						HasBracketOpen();
	void 						hasName(std::string &directiveName, std::string & pieceOfString, size_t i);

	void						hasValue(std::string &directiveValue, std::string & pieceOfString);

	void						insertMap(std::map<std::string, std::string> &dictionary, std::string &directiveName, std::string & directiveValue);
	void						HasBracketClose();
	void						hasLocation(std::string &directiveName, std::string & pieceOfString);

};

#endif 
