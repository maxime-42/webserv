#include "Parse_config_file.hpp"

/*
** THIS IS A SINGLETON CLASS : Singleton design pattern is a software design principle that is used to restrict the instantiation of a class to one object
*/

 void Parse_config_file:: operator=(Parse_config_file &other) {(void)other;}// Singletons should prevent copy or assign

Parse_config_file ::Parse_config_file  (const Parse_config_file & other) {(void)other;};// Singletons should not be cloneable, so it private.


static  std::string  s_fileName;

Parse_config_file & Parse_config_file::								getInstance(std::string fileName)
{
	// s_fileName = fileName;
	static  Parse_config_file  _instance(fileName);
	return (_instance);
}

/**
 * @brief 
 * this construct it called when there is no one given parameter to the program
 * @return Parse_config_file:: 
 */

Parse_config_file::											Parse_config_file(): _fileName("./configFile/default.conf"), _configFile(std::string()), _errorHappened(false)
{
	std::cout << "*******************************\tTAKING \tDEFAULT\t FILE\t***********************" << std::endl;

	set_defaut_config();
	int result = getStartProcess();
	if (result == ERROR)
	{
		_errorHappened = true;
	}
}

/**
 * @brief 
 *	this construct it called when given parameter to the program
 * @param fileName the config file
 */
Parse_config_file::											Parse_config_file(std::string fileName): _fileName(fileName), _configFile(std::string()), _errorHappened(false)
{
	s_fileName = fileName;
	std::cout << "****************GET FILE\tFROM\tPARAMETER****************" << std::endl;
	set_defaut_config();

	int result = getStartProcess();
	if (result == ERROR)
	{
		_errorHappened = true;
	}
}

bool	Parse_config_file::									getErrorHappened(){return (_errorHappened);}

/*
**	step one 	:	get file in std::string "configFile"
**	step two 	:	create table of keyword
**	step three	:	parsing file, file is located in std::string _configFile;
*/
int	Parse_config_file::										getStartProcess()
{
	try
	{
		getFile();
		createKeyWord();
		_previousToken = initialized;
		// parsingProcess();
		_bracket_counter = 0;
		_previousToken = initialized;
		_indexConfigFile = 0;
		_hisLocation = false;
		parse();
		std::cout << ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>SUCCESSFULLY PARSING<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<\n" << std::endl;
	}
	catch(char const *  msg_error)
	{
		std::cerr << msg_error << std::endl;
		return (ERROR);
	}
	return (SUCCESS);
}

Parse_config_file::~Parse_config_file(){ }

/***************************************************************alll get function ****************************************/
size_t	Parse_config_file::									numberOfServer()
{
	return (getInstance(s_fileName).interface_numberOfServer());
}

size_t	Parse_config_file::									interface_numberOfServer()
{
	return (_serverList.size());
}


t_nested_list	&										Parse_config_file:: getList()
{
	return(getInstance(s_fileName).interface_getList());
}

t_nested_list	&	Parse_config_file::						interface_getList(){	return(_serverList);}

std::map<std::string, std::string> & Parse_config_file::		get_globalConfig()
{
	return (getInstance(s_fileName).interface_get_globalConfig());
}

std::map<std::string, std::string> & Parse_config_file::		interface_get_globalConfig(){	return (_globalConfig);}


std::vector<int> & Parse_config_file:: 						interface_get_ports () {return (_ports);}

std::vector<int> & Parse_config_file:: 						get_ports () 
{
	return (getInstance(s_fileName).interface_get_ports());
}

void	Parse_config_file::									set_defaut_config()
{
	_defautConfig["allow"] = "PUT GET DELETE POST";
	_defautConfig["listen"] = "8080";
	_defautConfig["cli_max_size"] = "5000000";
	_defautConfig["error page"] = " 404 error.html";
	_defautConfig["server_name"] = "tebi2poney";
	_defautConfig["index"] = "index.html";
	// _defautConfig["cgi_pass"] = CGI_PATH;
	_defautConfig["autoindex"] = "on";
	char *pwd = pwd = getcwd(NULL, 0);
	if (pwd)
	{
		_defautConfig["root"] = pwd;
		_defautConfig["root"] += "/www";
		free(pwd);
	}

}

std::map<std::string, std::string> & Parse_config_file::		get_defaut_config(){return(getInstance(s_fileName).interface_get_defaut_config());}
// std::map<std::string, std::string> & Parse_config_file::		get_defaut_config(){return(_defautConfig);}

std::map<std::string, std::string> & Parse_config_file:: 		interface_get_defaut_config () {return (_defautConfig);}

/*********************************************************************************OPEN  FILE* *********************************************************************************/


/**
 * @brief 
 * this function remove comments line in give  string 
 * comment line start with "#"
 * @param line which has  #
 */
void	Parse_config_file::									handleCommentes(std::string &line)
{
	size_t  begin = line.find("#");
	if (begin != std::string::npos)
	{
		line.erase(begin);
	}
}

/**
 * @brief Get the File object
 * get file and store it in std::string _configFile
 * get file line by line and then concated each line got previously
 */
void	Parse_config_file:: getFile()
{
	std::string line;
	std::ifstream MyReadFile(_fileName.c_str());
	if (MyReadFile.is_open())
	{
		while (getline (MyReadFile, line)) 
		{
			_configFile.append(line);
			handleCommentes(_configFile);
			_configFile.append("\n");
		}
		MyReadFile.close();
	}
	else
	{
		throw("error open file");
	}
}


/*********************************************************************************ANALYSE SYNTAXE CONFIG FILE*********************************************************************************/



/**
 * @brief 
 * Returns true if given string in parameter is a number else return false
 * @param str the given string to parse each character if is a digit chacracter
 * @return true  if given string in parameter is a number
 * @return false if given string in parameter is not a number
 */
bool	 		Parse_config_file::							isNumber(std::string &str)
{
	int result;
	for (size_t i = 0; i < str.size(); i++)
	{
		result = isdigit(str[i]);
		if (result == 0)
		{
			return (false);
		}
	}
	return (true);
}



/**
 * @brief 
 * check if the port is a intger
 * if str_port is not a integer in string format throw error
 * @param str_port port in string format
 */
void	Parse_config_file:: 									checkPort(std::string &str_port)
{
	int int_port ;
	bool ret = isNumber(str_port);
	if (ret == false)
		throw("error : port must be a integer");
	std:: stringstream(str_port) >> int_port; // convert string to integer
	_ports.push_back(int_port);
}



void	Parse_config_file:: hasSemicolon()
{
	if (_previousToken == value)
	{
		_previousToken = semicolon;
	}
	else
	{
		throw("error syntaxe: insertMap");
	}
}

void	Parse_config_file:: 									hasName(std::string &directiveName, std::string & pieceOfString, size_t i)
{
	int result = 0;
	result = !isspace(_configFile[i]);
	if (result != 0)
	{
		throw("error syntaxe: hasName");
	}
	// if (_previousToken == semicolon || _previousToken == brackets_open || _previousToken == brackets_close)
	if (_previousToken == semicolon || _previousToken == brackets_open || _previousToken == brackets_close || _previousToken == initialized)
	{
		directiveName = pieceOfString;
		_previousToken = name;
	}
	else
		throw("error syntaxe: hasName");

}

void	Parse_config_file:: 									hasValue(std::string &directiveValue, std::string & pieceOfString)
{
	if (_previousToken == name || _previousToken == location || _previousToken == value)
	{
		if (_previousToken == value)
		{
			directiveValue.append(" ");
			directiveValue += pieceOfString;
		}
		else
				directiveValue = pieceOfString;
		_previousToken = value;
	}
	else
	{
		throw("error syntaxe: hasValue");
	}
}

void	Parse_config_file::	createKeyWord()
{
	_keyWords.push_back("listen");
	_keyWords.push_back("server_name");
	_keyWords.push_back("root");
	_keyWords.push_back("error_page");
	_keyWords.push_back("autoindex");
	_keyWords.push_back("client_max_body_size");
	_keyWords.push_back("index");
	_keyWords.push_back("allow_methods");
	_keyWords.push_back("allow");
	_keyWords.push_back("cgi");
	_keyWords.push_back("fastcgi_pass");
	_keyWords.push_back("fastcgi_param");
	_keyWords.push_back("return");
	_keyWords.push_back("cgi_pass");
	_keyWords.push_back("cli_max_size");
}

bool	is_secret_word(std::string &word)
{
	if (word.compare("listen") == 0)
		return (true);
	if (word.compare("server_name") == 0)
		return (true);
	if (word.compare("root") == 0)
		return (true);
	if (word.compare("error_page") == 0)
		return (true);
	if (word.compare("autoindex") == 0)
		return (true);
	if (word.compare("client_max_body_size") == 0)
		return (true);
	if (word.compare("index") == 0)
		return (true);
	if (word.compare("allow_methods") == 0)
		return (true);
	if (word.compare("allow") == 0)
		return (true);
	if (word.compare("cgi") == 0)
		return (true);
	if (word.compare("fastcgi_pass") == 0)
		return (true);
	if (word.compare("fastcgi_param") == 0)
		return (true);
	if (word.compare("return") == 0)
		return (true);
	if (word.compare("cli_max_size") == 0)
		return (true);
	if (word.compare("cgi_pass") == 0)
		return (true);
	return (false);
}
/*
**	the goal is to get a piece of word inside a string
**		get a piece of a string, between start and nbCharacterTocopy
**		this piece came from configFile
** 		paramter i is index of string _configFile
** 		i it is increment depending  numbers characters to copy
*/
std::string	Parse_config_file::								getPieceOfstring(size_t &i)
{
	size_t	nbCharacterTocopy = 0;
	size_t start = i;

	if (_configFile[i] == '{' || _configFile[i] == '}' || _configFile[i] == ';')//if { or { or ; is the first character i going to catch that piece Of String
	{
		i++;
		nbCharacterTocopy++;
	}
	else
	{
		while (!isspace(_configFile[i]) && (_configFile[i] != '{' && _configFile[i] != '}') && _configFile[i] != ';') // { or } or ; and whitespace character are delimiter
		{
			nbCharacterTocopy++;
			i++;
		}
	}
	std::string pieceOfString = _configFile.substr(start, nbCharacterTocopy);
	return (pieceOfString);
}


void	Parse_config_file::									hasLocation(std::string &directiveName, std::string & pieceOfString)
{
	if (_previousToken == semicolon || _previousToken == brackets_open || _previousToken == brackets_close)
	{
		_previousToken = location;
		directiveName = pieceOfString;
	}
	else
	{
		throw("Syntaxe error : hasLocation");
	}
}

void	Parse_config_file::									hasBracketOpen(int &nbParenthese)
{
	if ( _previousToken == server || _previousToken == location || _previousToken == value)
	{
		nbParenthese++;
		_previousToken = brackets_open;
	}
	else
	{
		throw("Syntaxe error : Server expected open bracket");
	}
}

void	Parse_config_file::									hasBracketClose(int &nbParenthese)
{
	if ( _previousToken == semicolon || _previousToken == brackets_close || _previousToken == brackets_open)
	{
		nbParenthese--;
		_previousToken = brackets_close;
	}
	else
	{
		throw("Syntaxe error :  HasBracketClose");
	}
}

/*
** check if given string in paramter of function is secret word
** secret word is inside vector _keyWords
*/
bool	Parse_config_file::									checkIfSecretWord(std::string &pieceOfString)
{
	int resultCompar = -1;

	for (size_t j = 0; j < _keyWords.size()&& resultCompar != 0 ; j++)
	{
		resultCompar = _keyWords[j].compare(pieceOfString);
	}
	return (resultCompar == 0 ? true : false);
}

void	Parse_config_file::									push_front_in_singleList(std::map<std::string, std::string>	&dictionary)
{
	if (dictionary.size())
	{
		_singleList.push_front(dictionary);
	}
}

void	Parse_config_file::									push_back_in_singleList(std::map<std::string, std::string>	&dictionary)
{
	if (dictionary.size())
	{
		_singleList.push_back(dictionary);
	}
}

void		verify_the_path(std::string path)
{
	bool ret = is_a_directory(path);
	if (ret == false)
	{
		std::cout << "paht = [" << path <<"]" << std::endl;
		throw("error : root have to be a existing directory");
	}
}

/*
** this function insert name and value in dictionary
** afterward initialize  name and value
*/
void	Parse_config_file::									insertInDictionary(std::map<std::string, std::string>	&dictionary, std::string &directiveName, std::string &directiveValue)
{
	if (directiveName.compare("root") == 0)
	{
		verify_the_path(directiveValue);
	}
	dictionary[directiveName] = directiveValue;
	directiveName = std::string();
	directiveValue = std::string();	
}

void		Parse_config_file::								block_return(std::map<std::string, std::string>	&dictionary)
{
	std::map < std::string, std::string >::iterator itr_dictionary;
	itr_dictionary = dictionary.find("return");
	if (itr_dictionary != dictionary.end())
	{
		throw("error return");
	}
}
void	Parse_config_file::									hasServer()
{
	// if (_previousToken == initialized || _previousToken == brackets_close)
	if (_previousToken == initialized || _previousToken == brackets_close || _previousToken == semicolon)
	{
		_previousToken = server;
	}
	else
	{
		throw("Syntaxe error : hasServer");
	}
}

/*
**	si (location)
		cout++
		tmp = dictionary;
		dicyionary.clear()
	....
	apres ajout du dictionnary dans liste
	si (tmp.size())
		dictionary = tmp;
		tmp.clear()

	si (nb_parenthese == 0)
		push_front dictionary dans liste

*/
/*
**to understand pretty good this function you should glance on the diagram of parsing
** this function try to identify token, then act to depending token  
** token is pieceOfString
*/

/////////////getter////////////////
token_type	Parse_config_file::	get_previousToken(){return (_previousToken);}
std::string Parse_config_file::	get_directive_name(){return (_directive_name);}
std::string Parse_config_file::	get_directive_value(){return (_directive_value);}
std::map<std::string, std::string> Parse_config_file::	get_block_server(){return (_block_server);}
std::map<std::string, std::string> Parse_config_file::	get_block_location(){return (_block_location);}
int	Parse_config_file::	get_bracket_counter(){return (_bracket_counter);}
std::string	Parse_config_file::	get_current_word(){return (_current_word);}
std::string	Parse_config_file::	get_configFile(){return (_configFile);}
size_t		Parse_config_file::	get_indexConfigFile(){return (_indexConfigFile);}
t_single_list	Parse_config_file::get_singleList(){return (_singleList);}
bool		Parse_config_file:: get_hisLocation(){ return(_hisLocation);}


///////////setter//////////////////////
void		Parse_config_file::	set_previousToken(token_type newToken){_previousToken = newToken;}
void		Parse_config_file::	set_directive_name(std::string name){_directive_name = name;}
void		Parse_config_file::	set_directive_value(std::string value){_directive_value = value;}
void		Parse_config_file::	set_block_server(std::string name, std::string value){_block_server[name] = value;}
void		Parse_config_file::	set_block_server(std::map<std::string, std::string> block_serve){_block_server = block_serve;}
void		Parse_config_file::	set_block_location(std::map<std::string, std::string> block){_block_location = block ;}
void		Parse_config_file::	set_block_location(std::string name, std::string value){_block_location[name] = value;}
void		Parse_config_file::	set_bracket_counter(int value){_bracket_counter = value;}
void		Parse_config_file::	set_current_word(std::string word){_current_word = word;}
void		Parse_config_file::	set_singleList(t_single_list singleList){_singleList = singleList;}
void		Parse_config_file:: set_hisLocation(bool state){_hisLocation = state;}

void				Parse_config_file::	set_globalConfig(std::string directive_name, std::string directive_value)
{
	int bracket_counter = get_bracket_counter();
	if ( bracket_counter == 0)
	{
		_globalConfig[directive_name] = directive_value;
	}
}

bool				 has_Semicolon(Parse_config_file *ptr)
{
	std::string current_word = ptr->get_current_word();
	if (current_word.compare(";") == 0)
	{
		token_type previousToken = ptr->get_previousToken();
		if (previousToken == value)
		{
			ptr->set_previousToken(semicolon);
			if (ptr->get_bracket_counter() == 0)
				ptr->set_globalConfig(ptr->get_directive_name(), ptr->get_directive_value());
			else if (ptr->get_bracket_counter() == 2 && ptr->get_hisLocation() == true)
			{
				ptr->set_block_location(ptr->get_directive_name(), ptr->get_directive_value());//test
			}
			else
				ptr->set_block_server(ptr->get_directive_name(), ptr->get_directive_value());
			return (true);
		}
		else
		{
			throw("error syntaxe: insertMap");
		}
	}
	return (false);
}

bool	 									has_Value(Parse_config_file *ptr)
{
	token_type previousToken = ptr->get_previousToken();
	if (previousToken == name || previousToken == location || previousToken == value)
	{
		std::string current_word = ptr->get_current_word();
		std::string directiveValue = ptr->get_directive_value();
		if (previousToken == value)
		{
			// directiveValue.append(" ");
			directiveValue += " " + current_word;
			// ptr->set_directive_value(directiveValue);
		}
		else
			directiveValue = current_word;
		ptr->set_directive_value(directiveValue);
		ptr->set_previousToken(value);
		return (true);
	}
	else
	{
		std::cout << "ptr->get_current_word() == [" << ptr->get_current_word() << "]" << std::endl;
		throw("error syntaxe: hasValue");
	}
	return (false);
}

bool 									has_DirectName(Parse_config_file *ptr)
{
	std::string current_word = ptr->get_current_word();
	if (is_secret_word(current_word) == true)
	{
		std::string configFile = ptr->get_configFile();
		int result = !isspace(configFile[ptr->get_indexConfigFile()]);/**/
		if (result != 0)
		{
			throw("error syntaxe: hasName");
		}
		token_type previousToken = ptr->get_previousToken();
		if (previousToken == semicolon || previousToken == brackets_open || previousToken == brackets_close || previousToken == initialized)
		{
			ptr->set_directive_name(current_word);
			ptr->set_previousToken(name);
			return (true);
		}
		else
			throw("error syntaxe: hasName");
	}
	return (false);
}

bool										curlOpen(Parse_config_file *ptr)
{
	std::string word = ptr->get_current_word();
	if (word.compare("{") == 0)
	{
		token_type previousToken = ptr->get_previousToken();
		if (previousToken == server || previousToken == location || previousToken == value)
		{
			int n = ptr->get_bracket_counter();
			n++;
			ptr->set_bracket_counter(n);
			previousToken = brackets_open;
			ptr->set_previousToken(previousToken);
			return (true);
		}
		else
		{
			throw("Syntaxe error : Server expected on curl open");
		}
	}
	return (false);
}

void	Parse_config_file::									push_in_neestedList(t_single_list singleList)
{
		_serverList.push_back(singleList);
}

bool										curlClose(Parse_config_file *ptr)
{
	std::string word = ptr->get_current_word();
	if (word.compare("}") == 0)
	{
		token_type previousToken = ptr->get_previousToken();
		if ( previousToken == semicolon || previousToken == brackets_close || previousToken == brackets_open)
		{
			int n = ptr->get_bracket_counter();
			ptr->set_bracket_counter(--n);
			std::map <std::string, std::string> block_server = ptr->get_block_server();
			if (ptr->get_bracket_counter() == 0)
			{
				ptr->push_front_in_singleList(block_server);
				t_single_list singList = ptr->get_singleList();
				ptr->push_in_neestedList(singList);
				ptr->set_singleList(t_single_list());
				ptr->set_block_server(std::map <std::string, std::string>());
			}
			else if (ptr->get_bracket_counter() == 1 && ptr->get_hisLocation() == true)
			{
				std::map <std::string, std::string> block_location = ptr->get_block_location();
				ptr->set_hisLocation(false);
				ptr->push_back_in_singleList(block_location);
				std::cout << "keeeeeeeeeeLLLLLLLLL BYYYYYYYYYYYY" << std::endl;
			}
			previousToken = brackets_close;
			return (true);
		}
		else
		{
			throw("Syntaxe error :  HasBracketClose");
		}
	}
	return (false);
}


bool										block_Server(Parse_config_file *ptr)
{
	std::string word = ptr->get_current_word();
	if (word.compare("server") == 0)
	{	
		token_type previousToken = ptr->get_previousToken();
		if (previousToken == initialized || previousToken == brackets_close || previousToken == semicolon)
		{
			previousToken = server;
			ptr->set_previousToken(previousToken);
			return (true);
		}
		else
		{
			throw("Syntaxe error : hasServer");
		}
	}
	return (false);
}
void		displayDirectionary(std::map<std::string, std::string> &map);
void							displaySingleList(std::list<std::map < std::string, std::string > > &linkedList);

// void						Parse_config_file::handler_location()
// {
// 	bool (*ptr_func[SIZE_ARRAY_FUNC])(Parse_config_file *) = {&block_Server, &curlOpen, &curlClose, &has_DirectName, &has_Semicolon};
// 	while ( _configFile[_indexConfigFile] != '}')
// 	{
// 		if (!isspace(_configFile[_indexConfigFile]))
// 		{

// 		}
// 	}
// }

bool									has_location_block(Parse_config_file *ptr)
{
	std::string current_word = ptr->get_current_word();
	if (current_word.compare("location") == 0)
	{
		token_type	previousToken = ptr->get_previousToken();
		if (previousToken == semicolon || previousToken == brackets_open || previousToken == brackets_close)
		{
			ptr->set_previousToken(location);
			ptr->set_hisLocation(true);
			return (true);
		}
		else
		{
			throw("Syntaxe error : hasLocation");
		}
	}
	return (false);

}

void													Parse_config_file::parse()
{
	bool ret;
	bool (*ptr_func[SIZE_ARRAY_FUNC])(Parse_config_file *) = {&block_Server, &curlOpen, &curlClose, &has_DirectName, &has_Semicolon, &has_location_block};
	for (; _indexConfigFile < _configFile.size();)
	{
		if (!isspace(_configFile[_indexConfigFile]))
		{
			_current_word = getPieceOfstring(_indexConfigFile);
			for (size_t i = 0; i < SIZE_ARRAY_FUNC; i++)
			{
				ret = ptr_func[i](this);
				if (ret == true)
				{
					i = SIZE_ARRAY_FUNC;
				}
			}
			if (ret == false)
				has_Value(this);
		}
		else
 			_indexConfigFile++;
	}
	if (get_bracket_counter() != 0 )
		throw("error syntaxe: missing parenthe");
	// std::map <std::string, std::string> test = get_block_server();
	// displayDirectionary(test);
	std::cout << "======Location===" << std::endl;
	displayDirectionary(_block_location);
	// displaySingleList(_singleList);
	
}
