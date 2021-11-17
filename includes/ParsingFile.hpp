
#ifndef PARSINGFILE
#define PARSINGFILE

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
#include "header.hpp"	
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

/*
** THIS IS A SINGLETON CLASS : Singleton design pattern is a software design principle that is used to restrict the instantiation of a class to one object
*/


typedef std::list < std::list < std::map < std::string, std::string > > > t_nested_list;
typedef std::list<std::map < std::string, std::string > > t_single_list;

class ParsingFile
{

private:
	std::string															_fileName;
	t_single_list														_singleList;

	std::string															_configFile;

	token_type															_previousToken;//this function always stock the previous token
	std::vector<std::string> 											_keyWords;		//this vectore stock all keys word

	t_nested_list														_serverList;		//this vectore stock all keys word
	bool																_errorHappened;
	ParsingFile();
	ParsingFile(std::string fileName);
	void operator=(ParsingFile &other);
  	ParsingFile (const ParsingFile &other);
	size_t																interface_numberOfServer();
	t_nested_list	&													 interface_getList();


public:
	static	ParsingFile													&getInstance();
	static	ParsingFile 												&getInstance(std::string fileName);
	~ParsingFile();

	bool																getErrorHappened();
	static size_t														numberOfServer();

	static t_nested_list	&													getList();

	/****************this four function it is to debug********************/
	void						displayServerTable();
	void						displayToken(std::vector<std::string> &tokenVector);
	void						displayDirectionary(std::map<std::string, std::string> &map);
	void						displaySingleList(std::list<std::map < std::string, std::string > > &linkedList);

private:
	void						getFile();
	void						createKeyWord();
	void						handleCommentes(std::string &line);
	void						parsingProcess();
	void						checkServerSyntaxe(size_t &i);

	std::string					getPieceOfstring(size_t &i);
	void						hasServer();
	bool						checkIfSecretWord(std::string &pieceOfString);

	/****************those function operare to a specific token********************/
	void						hasBracketOpen(int &nbParenthese);
	void						hasBracketClose(int &nbParenthese);	
	void 						hasName(std::string &directiveName, std::string & pieceOfString, size_t i);
	void						hasValue(std::string &directiveValue, std::string & pieceOfString);
	void						hasSemicolon();
	void						hasLocation(std::string &directiveName, std::string & pieceOfString);

	void						addListInNestedList(std::map<std::string, std::string>	&dictionary);
	void						addDictionaryInList(std::map<std::string, std::string>	&dictionary);
	void						insertInDictionary(std::map<std::string, std::string> &dictionary, std::string &directiveName, std::string &directiveValue);
	bool						isNumber(std::string &str);
	int							getStartProcess();
};

#endif 
