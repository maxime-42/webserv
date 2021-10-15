#include "ParsingFile.hpp"

ParsingFile::ParsingFile(/* args */):_file(""), _syntaxError(false)
{
	char const *ptr;
	std::vector<std::string>tokenArray;
	// std::cout << "construteur" << std::endl;
	getFile("./configFile/default.conf");
	createKeyWord();
	displayFile();
	ptr = _file.c_str();
	contentServer(&ptr);
	tokenArray = tokenizeString(_file, ';');
	displayToken(tokenArray);
	tokenArray = formatToken(tokenArray[0], CONTENT_SERVER);
	displayToken(tokenArray);

}

ParsingFile::~ParsingFile()
{
	// std::cout << "Bye" << std::endl;
}

std::vector<std::string> ParsingFile:: tokenizeString(std::string &line, char const sep)
{
	std::vector <std::string> tokens;
	// stringstream class check1
	std::stringstream check1(line);
	std::string intermediate;
	// Tokenizing w.r.t. space ' '
	while(getline(check1, intermediate, sep))
	{
		tokens.push_back(intermediate);
	}
	return (tokens);
}
/***************************Fill _siteTable ***************************/
/*
**	this function get value from given string
** exemple :
**	string = "  a  b " name is a value is b
**	this function copy b to return it and remove b from the string 
*/
std::string	ParsingFile::	getValueFromString(std::string &str)
{
	std::string value;
	int i = 0;
	while (isspace(str[i]))
		i++;
	while (!isspace(str[i]))
		i++;
	value = str.substr(i);
	str.erase(i);
	return (value);
}

std::vector<std::string>	ParsingFile:: formatToken(std::string &line, int content)
{
	std::vector<std::string> token;
	std::string directive;
	std::string value;
	if (content == CONTENT_SERVER || content == CONTENT_LOCATION)
	{
		token = tokenizeString(line, '{');
		directive = token[content];
		displayToken(token);
		value = getValueFromString(token[content]);
		token.push_back(value);
	}
	for (size_t i = 0; i < token.size(); i++)
	{
		// this line remove all whitspaces charactere around given string 
		token[i].erase(remove_if(token[i].begin(), token[i].end(), isspace), token[i].end());
	}
	return 	(token);
}

void	addSiteInTable(std::vector<std::string> &token, int i)
{
	std::string name;
	std::string value;
	if ((token.size() % 2))
	{
		for (size_t i = 0; i < token.size(); i++)
		{
			name = token[i];
			// _siteTable[0][listen] = 20;
			// _siteTable[0][server_name] = toto
			// _siteTable[1][server_name] = toto
		}
	}
	else
	{
			name = token[0];
			// _siteTable[][name] = token[i++];
	}
	


}

void	ParsingFile::prepareTokenToTable(std::vector<std::string> &tokenFile)
{
	int	i = 0;
	std::vector<std::string> token;

	for (std::vector<std::string>::iterator  it = tokenFile.begin(); it != tokenFile.end(); it++)
	{
		if (it->find("server") != std::string::npos )
			token = formatToken(*it, CONTENT_SERVER);
		if (it->find("location") != std::string::npos)
			token = formatToken(*it, CONTENT_LOCATION);
		if (token[0].compare("server")== 0)
		{
			i++;
		}
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
			_file.append(line);
			_file.append(" ");
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

}

void	ParsingFile::displayFile()
{
	std::cout << "\n************DISPLAY FILE************" << std::endl;
	std::cout << _file << std::endl;
}


void	ParsingFile:: displayToken(std::vector<std::string> &tokenVector)
{
	std::cout << "************DISPLAY TOKEN VECTORE************" << std::endl;
	for (std::vector<std::string>::iterator  it = tokenVector.begin(); it != tokenVector.end(); it++)
	{
		std::cout << *it << std::endl;
	}
}


/***************************ANALYSE SYNTAXE CONFIG FILE***************************/

void	ParsingFile::syntaxError(char const *msgError)
{
	std::cout << msgError<< std::endl;
	_syntaxError = true;
	this->~ParsingFile();
	exit(1);
}

void	ParsingFile:: movePointer(char const **ptr)
{
	while (isspace(**ptr))
	{
		(*ptr)++;
	}
}


void	ParsingFile::	directiveValue(char const **ptr)
{
	movePointer(ptr);
	while (std::isalnum(**ptr))
	{
		(*ptr)++;
	}
	movePointer(ptr);
	if (**ptr != ';')
	{
		syntaxError("error syntaxe: directive value");
	}
	std::cout << "directive value is successfull" << std::endl;
	(*ptr)++;
}

void	ParsingFile::directiveaName(char const **ptr)
{
	int 	ret = 1;
	for (std::vector<std::string>::iterator it = _keyWords.begin(); it !=  _keyWords.end() && ret != 0; it++)
	{
		ret = it->compare(0, strlen(*ptr), *ptr, it->length());
		if (ret == 0)
			*ptr += it->length();
	}
	if (ret != 0 || !isspace(**ptr) )
	{
		syntaxError("error syntaxe: directive name");
	}
	std::cout << "analyse Directive name is successfull " << std::endl;
}

void	ParsingFile::	directive(char const **ptr)
{
		movePointer(ptr);
		if (**ptr != '}')
		{
			directiveaName(ptr);
			directiveValue(ptr);
			movePointer(ptr);
		}

}

void	ParsingFile::	contentLocation(char const **ptr)
{
	bool openedParenthe = false;
	std::string str("location");
	movePointer(ptr);
	while (**ptr && **ptr != '}')
	{
		if (str.compare(0, strlen(*ptr), *ptr, str.length()) == 0)
		{
			std::cout << "****location detect****" << std::endl;
			while(*ptr && **ptr != '{')
				(*ptr)++;
			if (*ptr && **ptr != '{')
				syntaxError("error syntaxe: in location expected open parenthese");
			openedParenthe = true;
			(*ptr)++;
		}
		directive(ptr);
	}
	if (openedParenthe == true && **ptr != '}')
		syntaxError("error syntaxe: in location expected closed parenthe");
	if (openedParenthe == true )
		(*ptr)++;
}

void	ParsingFile::	contentServer(char const **ptr)
{
	std::string str("server");
	movePointer(ptr);
	while (**ptr)
	{
		if ( str.compare(0, strlen(*ptr), *ptr, str.length()) != 0)
		{
			syntaxError("error : expected server containt");
		}
		*ptr += str.length();
		movePointer(ptr);
		if (**ptr != '{')
		{
			syntaxError("error syntaxe: expected open parenthese");
		}
		std::cout << "analyse server open parenthese is successfully" << std::endl;
		*ptr += 1;
		contentLocation(ptr);
		movePointer(ptr);
		if (ptr[0][0] != '}')
			syntaxError("error syntaxe: server expected closed parenthese");
		(*ptr)++;
		movePointer(ptr);
	}
}