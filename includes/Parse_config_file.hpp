
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
#include "utile.hpp"

#define SIZE_ARRAY_FUNC 6



typedef std::list < std::list < std::map < std::string, std::string > > > t_nested_list;
typedef std::list<std::map < std::string, std::string > > t_single_list;

/**
 * @brief 
 * THIS IS A SINGLETON CLASS : Singleton design pattern is a software design principle that is used to restrict the instantiation of a class to one object
 * this class  get config file and parse them and store it
 */
class Parse_config_file
{

private:
	std::string															_fileName;
	t_single_list														_singleList;

	std::string															_configFile;

	token_type															_previousToken;//this function always stock the previous token

	t_nested_list														_serverList;		//this vectore stock all keys word
	bool																_errorHappened;
	std::map<std::string, std::string>									_globalConfig;
	std::vector<int>													_ports;	//this vector going to containt all port of config file

	std::map<std::string, std::string>									_defautConfig;
	std::map<std::string, std::string>									_block_server;
	std::map<std::string, std::string>									_block_location;
	size_t																_indexConfigFile;
	int																	_bracket_counter;
	std::string															_directive_name;
	std::string															_directive_value;
	std::string															_current_word;
	bool																_hisLocation;
	std::string															_pwd;


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
	void																handleCommentes(std::string &line);
	bool	 															isNumber(std::string &str);
	void																parse();

	void																hasServer(void);


	void																push_back_dictionary_in_singleList(std::map<std::string, std::string>	&dictionary);
	void																push_back_in_singleList(std::map<std::string, std::string>	&dictionary);
	void																checkPort(std::string &str_port);
	void																push_front_in_singleList(std::map<std::string, std::string>	&dictionary);
	bool																checkIfSecretWord(std::string &pieceOfString);

	size_t																get_indexConfigFile();
	token_type															get_previousToken();
	std::string															get_directive_name();
	std::string 														get_directive_value();
	
//////////getter///////

	int																	get_bracket_counter();
	std::string															get_current_word();
	std::string															get_configFile();
	t_single_list														get_singleList();
	bool																get_hisLocation();
	std::map<std::string, std::string>									get_defaut_block_serve();
	std::map<std::string, std::string>									get_block_server();
	std::map<std::string, std::string>									get_block_location();	
	std::string															get_current_directory();
	void																getFile();
	std::string															create_token(size_t &i);
	int																	getStartProcess();


//////////setter///////
	void																set_previousToken(token_type newToken);
	void																set_directive_name(std::string name);
	void																set_directive_value(std::string value);
	void 																set_block_server(std::string name, std::string value);
	void																set_block_location(std::map<std::string, std::string> block);
	void																set_block_location(std::string name, std::string value);

	void																set_bracket_counter(int value);
	void																set_current_word(std::string word);
	void																set_globalConfig(std::string directive_name, std::string directive_value);
	void																set_block_server(std::map<std::string, std::string> block_serve);
	void																push_in_neestedList(t_single_list singleList);
	void																set_singleList(t_single_list singleList);
	void																set_hisLocation(bool state);

/// parser//////////////////
 	bool										 						has_Semicolon(Parse_config_file *ptr);
 	bool	 															has_Value(Parse_config_file *ptr);
 	bool 																has_DirectName(Parse_config_file *ptr);
 	bool																curl_bracket_open(Parse_config_file *ptr);
	void																push_someWhere(Parse_config_file *ptr);
	bool																curl_bracket_close(Parse_config_file *ptr);
	bool																has_location_block(Parse_config_file *ptr);

	void																modify_name_error_page(std::string &directive_name, std::string &directive_value);
	void																not_allow(Parse_config_file *ptr);
	bool																block_Server(Parse_config_file *ptr);
	void																method_lookup();


public:
	static size_t														numberOfServer();
	~Parse_config_file();

	static std::map<std::string, std::string> & 						get_defaut_config();
	bool																getErrorHappened();
	static	Parse_config_file 											&getInstance(std::string fileName);
	static	Parse_config_file											&getInstance();
	static	t_nested_list	&											getList();
	static	std::map<std::string, std::string> & 						get_globalConfig();
	static std::vector<int> &											get_ports();

};

#endif 
