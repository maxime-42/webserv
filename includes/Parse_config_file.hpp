
#ifndef PARSE_CONFIG_FILE
#define PARSE_CONFIG_FILE

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

#define SIZE_ARRAY_FUNC 6
/*
** THIS IS A SINGLETON CLASS : Singleton design pattern is a software design principle that is used to restrict the instantiation of a class to one object
*/
#include "utile.hpp"


typedef std::list < std::list < std::map < std::string, std::string > > > t_nested_list;
typedef std::list<std::map < std::string, std::string > > t_single_list;

class Parse_config_file
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
	std::map<std::string, std::string>									_block_server;
	std::map<std::string, std::string>									_block_location;
	size_t																_indexConfigFile;
	int																	_bracket_counter;
	// int																	_index_file;
	std::string															_directive_name;
	std::string															_directive_value;
	std::string															_current_word;
	bool																_hisLocation;


	Parse_config_file();
	Parse_config_file(std::string fileName);
	void operator=(Parse_config_file &other);
  	Parse_config_file (const Parse_config_file &other);
	size_t																interface_numberOfServer();
	t_nested_list	&													interface_getList();
	std::map<std::string, std::string> & 								interface_get_globalConfig();
	std::vector<int> & 													interface_get_ports();
	std::map<std::string, std::string> & 			 					interface_get_defaut_config();

	void 																set_defaut_config();

public:
	static	Parse_config_file													&getInstance();
	static	Parse_config_file 												&getInstance(std::string fileName);
	static size_t														numberOfServer();
	static	t_nested_list	&											getList();
	static	std::map<std::string, std::string> & 						get_globalConfig();
	static std::vector<int> &											get_ports();
	static std::map<std::string, std::string> & 						get_defaut_config();
	void										push_front_in_singleList(std::map<std::string, std::string>	&dictionary);

	bool																getErrorHappened();
	bool																checkIfSecretWord(std::string &pieceOfString);

	std::string															_wsh;
	size_t																	get_indexConfigFile();
	~Parse_config_file();
	void																push_back_dictionary_in_singleList(std::map<std::string, std::string>	&dictionary);
	void										push_back_in_singleList(std::map<std::string, std::string>	&dictionary);

//////////getter///////
	token_type					get_previousToken();
	std::string					get_directive_name();
	std::string 				get_directive_value();
	std::map<std::string, std::string>	get_block_server();

	std::map<std::string, std::string>get_block_location();	
	int							get_bracket_counter();
	std::string					get_current_word();
	std::string					get_configFile();
	t_single_list				get_singleList();
	void						handler_location();
	bool						get_hisLocation();

//////////setter///////

	void						set_previousToken(token_type newToken);
	void						set_directive_name(std::string name);
	void						set_directive_value(std::string value);
	void 						set_block_server(std::string name, std::string value);
	void						set_block_location(std::map<std::string, std::string> block);
	void						set_block_location(std::string name, std::string value);

	void						set_bracket_counter(int value);
	void						set_current_word(std::string word);
	void						set_globalConfig(std::string directive_name, std::string directive_value);
	void						set_block_server(std::map<std::string, std::string> block_serve);
	void						push_in_neestedList(t_single_list singleList);
	void						set_singleList(t_single_list singleList);
	void						set_hisLocation(bool state);


private:
	void						getFile();
	void						createKeyWord();
	void						handleCommentes(std::string &line);
	void						parsingProcess();
	void						checkServerSyntaxe(size_t &i);
	bool	 					isNumber(std::string &str);

	std::string					getPieceOfstring(size_t &i);
	void						hasServer(void);
	void						checkPort(std::string &str_port);

	/****************those function operare to a specific token********************/
	void						hasBracketOpen(int &nbParenthese);
	void						hasBracketClose(int &nbParenthese);	
	void 						hasName(std::string &directiveName, std::string & pieceOfString, size_t i);
	void						hasValue(std::string &directiveValue, std::string & pieceOfString);
	void						hasSemicolon();
	void						hasLocation(std::string &directiveName, std::string & pieceOfString);

	// void						addDictionaryInList(std::map<std::string, std::string>	&dictionary);
	void						insertInDictionary(std::map<std::string, std::string> &dictionary, std::string &directiveName, std::string &directiveValue);
	int							getStartProcess();
	void						push_front_dictionary_in_singleList(std::map<std::string, std::string>	&dictionary);
	void						block_return(std::map<std::string, std::string>	&dictionary);
	void						parse();

};

#endif 
