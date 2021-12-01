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
		parsingProcess();
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
	_keyWords.push_back("alias");
	_keyWords.push_back("cgi_pass");
	_keyWords.push_back("cli_max_size");
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

void	Parse_config_file::									push_singleList_in_neestedList(std::map<std::string, std::string>	&dictionary)
{
	if (_previousToken == brackets_close)
	{
		_serverList.push_back(_singleList);
		t_single_list::iterator itr_secondList = _singleList.begin();
		std::map < std::string, std::string > dico = *itr_secondList;
		// std::map < std::string, std::string >::iterator itr_dictionary= dictionary.find("return");
		block_return(dico);
		dictionary.clear();
		dictionary = _defautConfig;
		_singleList.clear();
	}
}

void	Parse_config_file::									push_back_dictionary_in_singleList(std::map<std::string, std::string>	&dictionary)
{
	if (dictionary.size())
	{
		_singleList.push_back(dictionary);
		dictionary.clear();
	}
}

void	Parse_config_file::									push_front_dictionary_in_singleList(std::map<std::string, std::string>	&dictionary)
{
	if (dictionary.size())
	{
		_singleList.push_front(dictionary);
		dictionary.clear();
	}
}

/*
** this function insert name and value in dictionary
** afterward initialize  name and value
*/
void	Parse_config_file::									insertInDictionary(std::map<std::string, std::string>	&dictionary, std::string &directiveName, std::string &directiveValue)
{
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
void													Parse_config_file::parsingProcess()
{
	std::string 							directiveName;
	std::string 							directiveValue;
	std::map<std::string, std::string>		dictionary = _defautConfig; 
	int										nbParenthese = 0;//it increment when it meet open brack an decrement to bracket closed 
	int										count = 0;
	std::map<std::string, std::string>		tmp; 

	for (size_t i = 0; i < _configFile.size(); )
	{
		if (!isspace(_configFile[i]))
		{
			std::string pieceOfString = getPieceOfstring(i);
			if (pieceOfString.compare("server") == 0)
			{
				hasServer();
			}
			else if (pieceOfString.compare("location") == 0)
			{
				hasLocation(directiveName, pieceOfString);
			}
			else if (pieceOfString.compare("{") == 0)
			{
				hasBracketOpen(nbParenthese);
				if (directiveName.compare("location") == 0)
				{
					count++;
					tmp = dictionary;
					dictionary.clear();
					insertInDictionary(dictionary, directiveName, directiveValue);
				}
			}
			else if (pieceOfString.compare("}") == 0)
			{
				hasBracketClose(nbParenthese);
				if (nbParenthese == 0)
				{
					if (_singleList.size() == 0)
						push_back_dictionary_in_singleList(dictionary);
					if (count)
					{
						push_front_dictionary_in_singleList(dictionary);
						count--;
					}
					push_singleList_in_neestedList(dictionary);
					count = 0;
				}
				else
				{
					push_back_dictionary_in_singleList(dictionary);
					if (tmp.size())
					{
						dictionary = tmp;
						tmp.clear();
					}
				}
			}
			else if (checkIfSecretWord(pieceOfString) == true)
			{
				hasName(directiveName, pieceOfString, i);
			}
			else if (pieceOfString.compare(";") == 0)
			{
				hasSemicolon();
				if (directiveName.compare("listen") == 0 )
					checkPort(directiveValue);
				if (nbParenthese == 0)
					_globalConfig[directiveName] = directiveValue;
				else
					insertInDictionary(dictionary, directiveName, directiveValue);
			}
			else
			{
				hasValue(directiveValue, pieceOfString);
			}
		}
		else
			i++;
	}
	if (nbParenthese != 0 )
	{
		throw("error syntaxe: missing parenthe");
	}
}
