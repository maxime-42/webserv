
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
	size_t																_line;
	size_t																_column;
	std::list<std::map < std::string, std::string > >					_singleList;

	std::string															_configFile;
	size_t																_nbParenthese;
	token_type															_previousToken;//this function always stock the previous token
	std::vector<std::string> 											_keyWords;		//this vectore stock all keys word

	std::list < std::list < std::map < std::string, std::string > > >	_serverList;		//this vectore stock all keys word

public:
	ParsingFile();
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

	void						hasBracketOpen();
	void 						hasName(std::string &directiveName, std::string & pieceOfString, size_t i);

	void						hasValue(std::string &directiveValue, std::string & pieceOfString);

	void						hasSemicolon();
	void						HasBracketClose();
	void						hasLocation(std::string &directiveName, std::string & pieceOfString);
	void						displayServerTable();
	void						addListInNestedList(std::map<std::string, std::string>	&dictionary);
	void						addDictionaryInList(std::map<std::string, std::string>	&dictionary);
	void						displaySingleList(std::list<std::map < std::string, std::string > > &linkedList);
	void						insertInDictionary(std::map<std::string, std::string> &dictionary, std::string &directiveName, std::string &directiveValue);
	bool						isNumber(std::string &str);
	std::string					getElem(size_t lineServer, std::string elem);


};

#endif 
