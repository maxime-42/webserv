
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
#include <unistd.h>


/*
** THIS IS A SINGLETON CLASS : Singleton design pattern is a software design principle that is used to restrict the instantiation of a class to one object
*/
#include "utile.hpp"


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
	std::map<std::string, std::string>									_globalConfig;
	std::vector<int>													_ports;	//this vector going to containt all port of config file

	std::map<std::string, std::string>									_defautConfig;


	ParsingFile();
	ParsingFile(std::string fileName);
	void operator=(ParsingFile &other);
  	ParsingFile (const ParsingFile &other);
	size_t																interface_numberOfServer();
	t_nested_list	&													interface_getList();
	std::map<std::string, std::string> & 								interface_get_globalConfig();
	std::vector<int> & 													interface_get_ports();
	std::map<std::string, std::string> & 			 					interface_get_defaut_config();

	void 																set_defaut_config();

public:
	static	ParsingFile													&getInstance();
	static	ParsingFile 												&getInstance(std::string fileName);
	static size_t														numberOfServer();
	static	t_nested_list	&											getList();
	static	std::map<std::string, std::string> & 						get_globalConfig();
	static std::vector<int> &											get_ports();
	static std::map<std::string, std::string> & 						get_defaut_config();

	bool																getErrorHappened();


	~ParsingFile();

	/****************this four function it is to debug********************/
	void						displayServerTable();
	void						displayToken(std::vector<std::string> &tokenVector);
	void						displayDirectionary(std::map<std::string, std::string> &map);
	// void						displaySingleList(std::list<std::map < std::string, std::string > > &linkedList);

private:
	void						getFile();
	void						createKeyWord();
	void						handleCommentes(std::string &line);
	void						parsingProcess();
	void						checkServerSyntaxe(size_t &i);
	bool	 					isNumber(std::string &str);

	std::string					getPieceOfstring(size_t &i);
	void						hasServer();
	bool						checkIfSecretWord(std::string &pieceOfString);
	void						checkPort(std::string &str_port);

	/****************those function operare to a specific token********************/
	void						hasBracketOpen(int &nbParenthese);
	void						hasBracketClose(int &nbParenthese);	
	void 						hasName(std::string &directiveName, std::string & pieceOfString, size_t i);
	void						hasValue(std::string &directiveValue, std::string & pieceOfString);
	void						hasSemicolon();
	void						hasLocation(std::string &directiveName, std::string & pieceOfString);

	void						push_singleList_in_neestedList(std::map<std::string, std::string>	&dictionary);
	// void						addDictionaryInList(std::map<std::string, std::string>	&dictionary);
	void						insertInDictionary(std::map<std::string, std::string> &dictionary, std::string &directiveName, std::string &directiveValue);
	int							getStartProcess();
	void						push_front_dictionary_in_singleList(std::map<std::string, std::string>	&dictionary);
	void						push_back_dictionary_in_singleList(std::map<std::string, std::string>	&dictionary);
	void						block_return(std::map<std::string, std::string>	&dictionary);

};

#endif 
