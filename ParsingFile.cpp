#include "ParsingFile.hpp"

ParsingFile::								ParsingFile(/* args */):_configFile("")
{	
	_line = 0;
	_column = 0;
	_previousToken = initialized;
	_nbParenthese = 0;
	std::vector<std::string>tokenArray;
	// std::cout << "construteur" << std::endl;
	getFile("./configFile/default.conf");
	std::cout << "**********ConfigFile***********\n"<< _configFile << std::endl;
	createKeyWord();
	parsingFile();
	displayServerTable();
	std::cout << "\n>>>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<<<<<<<<<<\n" << std::endl;
	// displayDirectionary(*_singleList.begin());

	// std::cout << _configFile << std::endl;

}

ParsingFile::~ParsingFile()
{
	// std::cout << "Bye" << std::endl;
}

/*********************************************************************************DISPLAY*********************************************************************************/


void	ParsingFile::						displayToken(std::vector<std::string> &tokenVector)
{
	std::cout << "************DISPLAY TOKEN VECTORE************" << std::endl;
	for (std::vector<std::string>::iterator  it = tokenVector.begin(); it != tokenVector.end(); it++)
	{
		std::cout << *it << std::endl;
	}
}

void	ParsingFile::displayDirectionary(std::map<std::string, std::string> &map)
{
	std::cout << "***************DISPLAY DICTIONNARY***************" << std::endl;
	for (std::map<std::string, std::string>::iterator it = map.begin(); it != map.end(); it++)
	{
		std::cout << "name = " << it->first << "\t\tvalue = " << it->second<< std::endl;
	}
}

void	ParsingFile::						displaySingleList(std::list<std::map < std::string, std::string > > &linkedList)
{
	std::list<std::map < std::string, std::string > >::iterator  itrSingle_list_pointer  = linkedList.begin();
	for (;  itrSingle_list_pointer != linkedList.end(); itrSingle_list_pointer++)
	{
		std::cout << "<<<<<<<<<<SINGLE LIST>>>>>>>>>>" << std::endl;
		displayDirectionary(*itrSingle_list_pointer);
	}
}

void	ParsingFile::						displayServerTable()
{
	std::list < std::list < std::map < std::string, std::string > > >::iterator first = _serverList.begin() ;
	std::list < std::map < std::string, std::string > >::iterator itrSingle_list_pointer;
	size_t i = 0;
	for (; first != _serverList.end(); first++)
	{
		i++;
		std::list<std::map < std::string, std::string > > & single_list_pointer  = *first;
		std::cout << "<<<<<<<<<<<<<<<<<<<<SINGLE LIST N° = "<< i <<">>>>>>>>>>>>>>>>>>>>\n" << std::endl;
		for (itrSingle_list_pointer = single_list_pointer.begin();  itrSingle_list_pointer != single_list_pointer.end(); itrSingle_list_pointer++)
		{
			displayDirectionary(*itrSingle_list_pointer);
		}
	
	}	
}

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

void	ParsingFile:: getFile(std::string fileName)
{
	std::string line;
	std::ifstream MyReadFile(fileName.c_str());
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
		std::cout << "error open file" << std::endl;
		this->~ParsingFile();
		exit(ERROR);
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
void	ParsingFile::						syntaxError(char const *msgError)
{
	std::cout << msgError<< std::endl;
	this->~ParsingFile();
	exit(1);
}

void	ParsingFile:: hasSemicolon()
{
	if (_previousToken == value)
	{
		_previousToken = semicolon;
	}
	else
	{
		syntaxError("error syntaxe: insertMap");
	}
}

void	ParsingFile:: 						hasName(std::string &directiveName, std::string & pieceOfString, size_t i)
{
	int result = !isspace(_configFile[i]);
	if (result != 0)
	{
		syntaxError("error syntaxe: hasName");
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
		syntaxError("error syntaxe: hasValue");
	}
}

void	ParsingFile::	createKeyWord()
{
	_keyWords.push_back("listen");
	_keyWords.push_back("server_name");
	_keyWords.push_back("root");
	_keyWords.push_back("error_page");
	// _keyWords.push_back("location");
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
*/
std::string	ParsingFile::					getPieceOfstring(size_t &i)
{
	size_t	nbCharacterTocopy = 0;
	size_t start = i;

	if (_configFile[i] == '{' || _configFile[i] == '}' || _configFile[i] == ';')
	{
		i++;
		nbCharacterTocopy++;
	}
	else
	{
		while (!isspace(_configFile[i]) && (_configFile[i] != '{' && _configFile[i] != '}') && _configFile[i] != ';')
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
		syntaxError("Syntaxe error : hasServer");
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
		syntaxError("Syntaxe error : hasLocation");
	}
}

void	ParsingFile::						hasBracketOpen()
{
	if ( _previousToken == server || _previousToken == location || _previousToken == value)
	{
		_nbParenthese++;
		_previousToken = brackets_open;
	}
	else
	{
		syntaxError("Syntaxe error : Server expected open bracket");
	}
}

void	ParsingFile::						HasBracketClose()
{
	if ( _previousToken == semicolon || _previousToken == brackets_close || _previousToken == brackets_open)
	{
		_nbParenthese++;
		_previousToken = brackets_close;
	}
	else
		syntaxError("Syntaxe error :  HasBracketClose");
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

void										ParsingFile::parsingFile()
{
	std::string 							directiveName;
	std::string 							directiveValue;
	std::map<std::string, std::string>		dictionary;
	std::cout << "*********************************** STARTING P A R S I N G...******************************************" << std::endl;
	for (size_t i = 0; i < _configFile.size(); )
	{
		if (!isspace(_configFile[i]))
		{
			std::string pieceOfString = getPieceOfstring(i);
			// std::cout << "pieceOfString [" << pieceOfString << "]"<< std::endl;
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
				hasBracketOpen();
				if (directiveName.compare("location") == 0)
				{
					addDictionaryInList(dictionary);
					insertInDictionary(dictionary, directiveName, directiveValue);
				}
			}
			else if (pieceOfString.compare("}") == 0)
			{
				HasBracketClose();
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
					syntaxError("error syntaxe: listen have to be decimal numer");
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
	if ((_nbParenthese%2) )
		syntaxError("error syntaxe: missing parenthe");
	std::cout << "***********SUCCESSFULLY PARSING***********" << std::endl;
	addListInNestedList(dictionary);
}

/*********************************************************************************get value*********************************************************************************/



/*
single linked list containt a map in each node.
each node  represente a location.
map containt all directive
*/