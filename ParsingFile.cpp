#include "ParsingFile.hpp"

ParsingFile::ParsingFile(/* args */):_configFile(""), _syntaxError(false)
{
	_nbParenthese = 0;
	std::vector<std::string>tokenArray;
	// std::cout << "construteur" << std::endl;
	getFile("./configFile/default.conf");
	std::cout << "**********ConfigFile***********\n"<< _configFile << std::endl;
	createKeyWord();
	parsingFile();
	std::cout << _configFile << std::endl;

}

ParsingFile::~ParsingFile()
{
	// std::cout << "Bye" << std::endl;
}


void	ParsingFile::displayDirectionary(std::map<std::string, std::string> &map)
{
	std::cout << "***************DISPLAY DICTIONNARY***************" << std::endl;
	for (std::map<std::string, std::string>::iterator it = map.begin(); it != map.end(); it++)
	{
		std::cout << "name = " << it->first << "\t\tvalue = " << it->second<< std::endl;
	}
}


/******************************************** OPEN  FILE*******************************************/

/*
*	this function remove comments
*/

void	ParsingFile::handleCommentes(std::string &line)
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


void	ParsingFile::displayFile()
{
	std::cout << "\n************DISPLAY FILE************" << std::endl;
	std::cout << _configFile << std::endl;
}


void	ParsingFile:: displayToken(std::vector<std::string> &tokenVector)
{
	std::cout << "************DISPLAY TOKEN VECTORE************" << std::endl;
	for (std::vector<std::string>::iterator  it = tokenVector.begin(); it != tokenVector.end(); it++)
	{
		std::cout << *it << std::endl;
	}
}


void	ParsingFile::syntaxError(char const *msgError)
{
	std::cout << msgError<< std::endl;
	_syntaxError = true;
	this->~ParsingFile();
	exit(1);
}

/***************************ANALYSE SYNTAXE CONFIG FILE***************************/


void	ParsingFile::	createKeyWord()
{
	_keyWords.push_back("listen");
	_keyWords.push_back("server_name");
	_keyWords.push_back("root");
	_keyWords.push_back("error_page");
	_keyWords.push_back("location");
	_keyWords.push_back("autoindex");
	_keyWords.push_back("client_max_body_size");
	_keyWords.push_back("index");
	_keyWords.push_back(";");
	_keyWords.push_back("allow_methods");
	_keyWords.push_back("cgi");
	_keyWords.push_back("fastcgi_pass");
	_keyWords.push_back("fastcgi_param");
	_keyWords.push_back("return");
	_keyWords.push_back("location");
	_keyWords.push_back("server");
	_keyWords.push_back("{");
	_keyWords.push_back("}");
	_keyWords.push_back(";");


}

/*
**	the goal is to get a piece of word inside a string
**		get a piece of a string, between start and nbCharacterTocopy
**		this piece came from configFile
*/
std::string	ParsingFile::pieceOfConfigFile(size_t &i)
{
	size_t	nbCharacterTocopy = 0;
	size_t start = i;

	while (!isspace(_configFile[i++]) && (_configFile[i] != '{' && _configFile[i] != '}') && _configFile[i] != ';')
	{
		nbCharacterTocopy++;
	}
	std::string pieceOfString = _configFile.substr(start, nbCharacterTocopy);
	return (pieceOfString);
}

void	ParsingFile::checkServerSyntaxe(size_t &i, std::string &pieceOfString)
{
	if (_prevElem == INIT)
		_prevElem = CONTENTE_SERVER;
	else if (pieceOfString.compare("{") == 0 && _prevElem == CONTENTE_SERVER)
	{
		_nbParenthese++;
	}
	else
		syntaxError("Syntaxe error : Server expected open parenthese");
}

/*
** this function do three thing , the goal is to get a piece of word inside a string
** first step:
**		get a piece of a string, between start and nbCharacterTocopy
**		this piece came from configFile
** 
**second step:
**		trying to find this piece string in _keyWords vector
**
** last step :
**	return this piece if is exist in keyWords otherwise return empty string
*/
int	ParsingFile::wordsRecognizer(std::string &pieceOfString)
{
	int resultCompar = -1;

	for (size_t j = 0; j < _keyWords.size()&& resultCompar != 0 ; j++)
	{
		resultCompar = _keyWords[j].compare(pieceOfString);
	}
	std::cout << "word =["<< pieceOfString <<"]\nresultCompar = " << resultCompar << std::endl;
	return (resultCompar == 0 ? resultCompar : NOT_FOUND);
}

void	ParsingFile::parsingFile()
{
	_prevElem = INIT;
	for (size_t i = 0; i < _configFile.size(); i++)
	{
		if (!isspace(_configFile[i]))
		{
			std::string pieceOfString = pieceOfConfigFile(i);
			if (pieceOfString.compare("server") == 0)
			{
				std::cout << "found = " << _keyWords[CONTENT_SERVER] << std::endl;
				// checkServerSyntaxe(i);
				std::cout << "word = [" << pieceOfString << "]" << std::endl;
			}
			else if (pieceOfString.compare("location") == 0)
			{
				std::cout << "found location = " << _keyWords[CONTENT_LOCATION] << std::endl;
				// std::cout << "found = " << _keyWords[CONTENT_SERVER] << std::endl;
			}
			else if (wordsRecognizer(pieceOfString) == 0)
			{
				;
			}
		}
	}
}
