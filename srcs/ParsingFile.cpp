#include "ParsingFile.hpp"

/*
** THIS IS A SINGLETON CLASS : Singleton design pattern is a software design principle that is used to restrict the instantiation of a class to one object
*/


 void ParsingFile:: operator=(ParsingFile &other) {(void)other;}// Singletons should prevent copy or assign

ParsingFile ::ParsingFile  (const ParsingFile & other) {(void)other;};// Singletons should not be cloneable, so it private.


static  std::string  s_fileName;

ParsingFile & ParsingFile::								getInstance(std::string fileName)
{
	// s_fileName = fileName;
	static  ParsingFile  _instance(fileName);
	return (_instance);
}



/*
**	this construct it called when there is no one given parameter to the program
*/
ParsingFile::											ParsingFile(): _fileName("./configFile/default.conf"), _configFile(std::string()), _errorHappened(false)
{
	std::cout << "*******************************\tTAKING \tDEFAULT\t FILE\t***********************" << std::endl;
	set_defaut_config();
	int result = getStartProcess();
	if (result == ERROR)
	{
		_errorHappened = true;
	}
}

/*
**	this construct it called when given parameter to the program
*/
ParsingFile::											ParsingFile(std::string fileName): _fileName(fileName), _configFile(std::string()), _errorHappened(false)
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

bool	ParsingFile::									getErrorHappened(){return (_errorHappened);}

/*
**	step one 	:	get file in std::string "configFile"
**	step two 	:	create table of keyword
**	step three	:	parsing file, file is located in std::string _configFile;
*/
int	ParsingFile::										getStartProcess()
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

ParsingFile::~ParsingFile(){ }

/***************************************************************alll get function ****************************************/
size_t	ParsingFile::									numberOfServer()
{
	return (getInstance(s_fileName).interface_numberOfServer());
}

size_t	ParsingFile::									interface_numberOfServer()
{
	return (_serverList.size());
}


t_nested_list	&										ParsingFile:: getList()
{
	return(getInstance(s_fileName).interface_getList());
}

t_nested_list	&	ParsingFile::						interface_getList(){	return(_serverList);}

std::map<std::string, std::string> & ParsingFile::		get_globalConfig()
{
	return (getInstance(s_fileName).interface_get_globalConfig());
}

std::map<std::string, std::string> & ParsingFile::		interface_get_globalConfig(){	return (_globalConfig);}


std::vector<int> & ParsingFile:: 						interface_get_ports () {return (_ports);}

std::vector<int> & ParsingFile:: 						get_ports () 
{
	return (getInstance(s_fileName).interface_get_ports());
}

void	ParsingFile::									set_defaut_config()
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

std::map<std::string, std::string> & ParsingFile::		get_defaut_config(){return(getInstance(s_fileName).interface_get_defaut_config());}
// std::map<std::string, std::string> & ParsingFile::		get_defaut_config(){return(_defautConfig);}

std::map<std::string, std::string> & ParsingFile:: 		interface_get_defaut_config () {return (_defautConfig);}

/*********************************************************************************OPEN  FILE* *********************************************************************************/

/*
**	this function remove comments line
** comment line start with "#"
*/
void	ParsingFile::									handleCommentes(std::string &line)
{
	size_t  begin = line.find("#");
	if (begin != std::string::npos)
	{
		line.erase(begin);
	}
}
/*
** get file and store it in std::string _configFile
** get file line by line and then concated each line got previously
*/
void	ParsingFile:: getFile()
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


/*
** Returns true if given string in parameter is a number else false
*/
bool	 		ParsingFile::							isNumber(std::string &str)
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


/*
** to be sure about unity of port
** first check if
*/
void	ParsingFile:: 									checkPort(std::string &str_port)
{
	int int_port ;
	bool ret = isNumber(str_port);
	if (ret == false)
		throw("error : port must be a integer");
	std:: stringstream(str_port) >> int_port; // convert string to integer
	_ports.push_back(int_port);
}



void	ParsingFile:: hasSemicolon()
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

void	ParsingFile:: 									hasName(std::string &directiveName, std::string & pieceOfString, size_t i)
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

void	ParsingFile:: 									hasValue(std::string &directiveValue, std::string & pieceOfString)
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

void	ParsingFile::	createKeyWord()
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
std::string	ParsingFile::								getPieceOfstring(size_t &i)
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

void	ParsingFile::									hasServer()
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

void	ParsingFile::									hasLocation(std::string &directiveName, std::string & pieceOfString)
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

void	ParsingFile::									hasBracketOpen(int &nbParenthese)
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

void	ParsingFile::									hasBracketClose(int &nbParenthese)
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
bool	ParsingFile::									checkIfSecretWord(std::string &pieceOfString)
{
	int resultCompar = -1;

	for (size_t j = 0; j < _keyWords.size()&& resultCompar != 0 ; j++)
	{
		resultCompar = _keyWords[j].compare(pieceOfString);
	}
	return (resultCompar == 0 ? true : false);
}

void	ParsingFile::									addListInNestedList(std::map<std::string, std::string>	&dictionary)
{
	if (_previousToken == brackets_close)
	{
		_serverList.push_back(_singleList);
		dictionary.clear();
		_singleList.clear();
	}
}

void	ParsingFile::									addDictionaryInList(std::map<std::string, std::string>	&dictionary)
{
	if (dictionary.size())
	{
		_singleList.push_back(dictionary);
		dictionary.clear();
	}
}

void	ParsingFile::									push_front_dictionary_in_singleList(std::map<std::string, std::string>	&dictionary)
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
void	ParsingFile::									insertInDictionary(std::map<std::string, std::string>	&dictionary, std::string &directiveName, std::string &directiveValue)
{
	dictionary[directiveName] = directiveValue;
	directiveName = std::string();
	directiveValue = std::string();	
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
void	print_string_dictionnary(std::map<std::string, std::string> &first);
void							displaySingleList(std::list<std::map < std::string, std::string > > &linkedList);

void													ParsingFile::parsingProcess()
{
	std::string 							directiveName;
	std::string 							directiveValue;
	// std::map<std::string, std::string>		dictionary = _defautConfig; 
	std::map<std::string, std::string>		dictionary; 
	int										nbParenthese = 0;//it increment when it meet open brack an decrement to bracket closed 
	/******************test******************/
	int										count = 0;
	std::map<std::string, std::string>		tmp; 

	std::cout << "******************************* ST A R T I N G	 P A R S I N G ******************************************" << std::endl;
	for (size_t i = 0; i < _configFile.size(); )
	{
		if (!isspace(_configFile[i]))
		{
			std::string pieceOfString = getPieceOfstring(i);
			// std::cout << "pieceOfString = [" << pieceOfString << "]" << std::endl;
			if (pieceOfString.compare("server") == 0)
			{
				hasServer();
			}
			else if (pieceOfString.compare("location") == 0)
			{
					std::cout << "count == [" << count << "]" << std::endl;

					// if (count > 1)
					// 	throw("error syntaxe: neested location");
				hasLocation(directiveName, pieceOfString);
			}
			else if (pieceOfString.compare("{") == 0)
			{
				hasBracketOpen(nbParenthese);
				if (directiveName.compare("location") == 0)
				{
					/******test*******/
					count++;

					tmp = dictionary;
					dictionary.clear();
					/****************/
					// addDictionaryInList(dictionary);
					insertInDictionary(dictionary, directiveName, directiveValue);

				}
			}
			else if (pieceOfString.compare("}") == 0)
			{
				hasBracketClose(nbParenthese);
				if (nbParenthese == 0)
				{
					if (_singleList.size() == 0)
						addDictionaryInList(dictionary);
					if (count)
					{
						// addDictionaryInList(dictionary);
						push_front_dictionary_in_singleList(dictionary);
						count--;
					}
					addListInNestedList(dictionary);
					// dictionary = _defautConfig; 
				}
				else
				{
					addDictionaryInList(dictionary);
					// if (count == 1)
					// 	count--;
					// print_string_dictionnary(dictionary);
					/*******test********/
					if (tmp.size())
					{
						// count--;
						// displaySingleList(_singleList);
						dictionary = tmp;
						tmp.clear();
						count--;

						// print_string_dictionnary(dictionary);
						
					}
					/****************/
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
				// 	throw("error syntaxe: listen have to be decimal numer");
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
