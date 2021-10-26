#include "ParsingFile.hpp"

/*
**	this construct it called when there are not any argument to the object
*/
ParsingFile::								ParsingFile():_fileName("./configFile/default.conf"), _configFile(std::string())
{
	int result = getStartProcess();
	if (result == ERROR)
	{
		this->~ParsingFile();
		exit(ERROR);
	}
}

/*
**	this construct it called when there is argument to the object
*/
ParsingFile::								ParsingFile(std::string fileName):_fileName(fileName), _configFile(std::string())
{	
	getStartProcess();
	
}

/*
**	step one 	:	get file in std::string "configFile"
**	step two 	:	create table of keyword
**	step three	:	parsing _configFile;
*/
int	ParsingFile::						getStartProcess()
{
	
	try
	{
		getFile();
		createKeyWord();
		_previousToken = initialized;
		parsingProcess();
		std::cout << ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>SUCCESSFULLY PARSING<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<" << std::endl;
	}
	catch(char const *  msg_error)
	{
		std::cerr << msg_error << std::endl;
		return (ERROR);
	}
	return (SUCCESS);
}

ParsingFile::~ParsingFile(){}

size_t	ParsingFile::						numberOfSite()
{
	return (_singleList.size());
}

/********************************************************************************ACCESS TO ELEM it _serverList it is a neested list************************************************************************************/

std::string	ParsingFile::				getElem(size_t lineServer, std::string elem)
{
	std::map < std::string, std::string > dictionary;
	std::map < std::string, std::string >::iterator it;
	std::list < std::list < std::map < std::string, std::string > > >::iterator first = _serverList.begin() ;
	std::list < std::map < std::string, std::string > >::iterator itrSingle_list_pointer;
	for (; first != _serverList.end(); first++)
	{
		lineServer--;
		if (lineServer == 0)
		{
			std::list<std::map < std::string, std::string > > & single_list_pointer  = *first;
			for (itrSingle_list_pointer = single_list_pointer.begin();  itrSingle_list_pointer != single_list_pointer.end(); itrSingle_list_pointer++)
			{
				dictionary = *itrSingle_list_pointer;
				it = dictionary.find(elem);
				if (it != dictionary.end())
					return (it->second);
			}
		}
	}
	return (std::string());	
}

/*********************************************************************************OPEN  FILE* *********************************************************************************/

/*
*	this function remove comments line
*/

void	ParsingFile::						handleCommentes(std::string &line)
{
	size_t  begin = line.find("#");
	if (begin != std::string::npos)
	{
		line.erase(begin);
	}
}

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
bool	ParsingFile:: 						isNumber(std::string &str)
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

void	ParsingFile:: 						hasName(std::string &directiveName, std::string & pieceOfString, size_t i)
{
	int result = !isspace(_configFile[i]);
	if (result != 0)
	{
		throw("error syntaxe: hasName");
	}
	if (_previousToken == semicolon || _previousToken == brackets_open || _previousToken == brackets_close)
	{
		directiveName = pieceOfString;
		_previousToken = name;
	}
}

void	ParsingFile:: hasValue(std::string &directiveValue, std::string & pieceOfString)
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
std::string	ParsingFile::					getPieceOfstring(size_t &i)
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

void	ParsingFile::						hasServer()
{
	if (_previousToken == initialized || _previousToken == brackets_close)
	{
		_previousToken = server;
	}
	else
	{
		throw("Syntaxe error : hasServer");
	}
}

void	ParsingFile::						hasLocation(std::string &directiveName, std::string & pieceOfString)
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

void	ParsingFile::						hasBracketOpen(int &nbParenthese)
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

void	ParsingFile::						hasBracketClose(int &nbParenthese)
{
	if ( _previousToken == semicolon || _previousToken == brackets_close || _previousToken == brackets_open)
	{
		nbParenthese--;
		_previousToken = brackets_close;
	}
	else
		throw("Syntaxe error :  HasBracketClose");
}

/*
** check if given string in paramter of function is secret word
** secret word is inside vector _keyWords
*/
bool	ParsingFile::						checkIfSecretWord(std::string &pieceOfString)
{
	int resultCompar = -1;

	for (size_t j = 0; j < _keyWords.size()&& resultCompar != 0 ; j++)
	{
		resultCompar = _keyWords[j].compare(pieceOfString);
	}
	return (resultCompar == 0 ? true : false);
}

void	ParsingFile::						addListInNestedList(std::map<std::string, std::string>	&dictionary)
{
	if (_previousToken == brackets_close)
	{
		_serverList.push_back(_singleList);
		dictionary.clear();
		_singleList.clear();
	}
}

void	ParsingFile::						addDictionaryInList(std::map<std::string, std::string>	&dictionary)
{
	if (dictionary.size())
	{
		_singleList.push_back(dictionary);
		dictionary.clear();
	}
}

/*
** this function insert name and value in dictionary
** afterward initialize  name and value
*/
void	ParsingFile::						insertInDictionary(std::map<std::string, std::string>	&dictionary, std::string &directiveName, std::string &directiveValue)
{
	dictionary[directiveName] = directiveValue;
	directiveName = std::string();
	directiveValue = std::string();	
}

/*
** this function try to identify token, then act to depending token  
** token is pieceOfString
*/
void										ParsingFile::parsingProcess()
{
	std::string 							directiveName;
	std::string 							directiveValue;
	std::map<std::string, std::string>		dictionary; 
	int										nbParenthese = 0;//it increment when it meet open brack an decrement to bracket closed 
	std::cout << "*********************************** ST A R T I N G	P A R S I N G...******************************************" << std::endl;
	for (size_t i = 0; i < _configFile.size(); )
	{
		if (!isspace(_configFile[i]))
		{
			std::string pieceOfString = getPieceOfstring(i);
			if (pieceOfString.compare("server") == 0)
			{
				addListInNestedList(dictionary);
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
					addDictionaryInList(dictionary);
					insertInDictionary(dictionary, directiveName, directiveValue);
				}
			}
			else if (pieceOfString.compare("}") == 0)
			{
				hasBracketClose(nbParenthese);
				addDictionaryInList(dictionary);
			}
			else if (checkIfSecretWord(pieceOfString) == true)
			{
				hasName(directiveName, pieceOfString, i);
			}
			else if (pieceOfString.compare(";") == 0)
			{
				hasSemicolon();
				if (directiveName.compare("listen") == 0 && isNumber(directiveValue) == false)
					throw("error syntaxe: listen have to be decimal numer");
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
	addListInNestedList(dictionary);
}
