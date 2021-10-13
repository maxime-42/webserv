#include "ParsingFile.hpp"

ParsingFile::ParsingFile(/* args */):_file(""), _syntaxError(false)
{
	char const *ptr;
	std::cout << "construteur" << std::endl;
	getFile("./configFile/default.conf");
	addtokenAvailable();
	displayFile();
	ptr = _file.c_str();
	parsingServer(&ptr);
}

ParsingFile::~ParsingFile()
{
	// std::cout << "Bye" << std::endl;

}


// void	ParsingFile::tokenizer(std::string &line)
// {

// 	std::stringstream check1(line);
// 	std::string intermediate;
// 	while(getline(check1, intermediate, ' '))
// 	{
// 		_tokens.push_back(intermediate);
// 	}
// 	(void)line;
// }

// void	getSite(std::string &file)
// {
// 	int openParenthese = 0;
// 	int closeParenthese = 0;

// }

void	ParsingFile:: getFile(std::string fileName)
{
	std::string line;

	std::ifstream MyReadFile(fileName.c_str());
	if (MyReadFile.is_open())
	{
		while (getline (MyReadFile, line)) 
		{
			_file.append(line);
			_file.append("\n");
		}
		MyReadFile.close();
	}
	else
	{
		std::cout << "error open file" << std::endl;
		exit(ERROR);
	}
	(void)fileName;
}

// void	ParsingFile:: getFile(std::string fileName)
// {
// 	// std::ifstream MyReadFile(fileName.c_str());
// 	 _readFile = (fileName.c_str());

// 	if (MyReadFile.is_open())
// 	{
// 	}
// 	else
// 	{
// 		std::cout << "error open file" << std::endl;
// 		exit(ERROR);
// 	}
// 	std::cout << _file << std::endl;
// 	(void)fileName;
// }

/*
**	this allow to skip any whitespace characters.
*/

void	ParsingFile::displayFile()
{
	std::cout << "\n************DISPLAY FILE************" << std::endl;
	std::cout << _file << std::endl;
}

void	ParsingFile::	addtokenAvailable()
{
	_tokenAvailable.push_back("listen");
	_tokenAvailable.push_back("server_name");
	_tokenAvailable.push_back("root");
	_tokenAvailable.push_back("error_page");
	_tokenAvailable.push_back("location");
	_tokenAvailable.push_back("autoindex");
	_tokenAvailable.push_back("client_max_body_size");
	_tokenAvailable.push_back("index");
	_tokenAvailable.push_back(";");
	_tokenAvailable.push_back("allow_methods");
	_tokenAvailable.push_back("cgi");
	_tokenAvailable.push_back("fastcgi_pass");
	_tokenAvailable.push_back("fastcgi_param");

}

void	ParsingFile:: displayTokenAvailable()
{
	std::cout << "************TOKEN AVAILABLE************" << std::endl;
	for (std::vector<std::string>::iterator  it = _tokenAvailable.begin(); it != _tokenAvailable.end(); it++)
	{
		std::cout << *it << std::endl;
	}
}


void	ParsingFile::syntaxError(char const *msgError)
{
	std::cout << msgError<< std::endl;
	_syntaxError = true;
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
	if (**ptr != ';')
	{
		syntaxError("error syntaxe: directive value");
	}
	std::cout << "directive value is successfull" << std::endl;
	(*ptr)++;
	movePointer(ptr);
}

void	ParsingFile::	directiveName(char const **ptr)
{
	int 	ret;
	while (**ptr &&  **ptr != '}' )
	{ 
		movePointer(ptr);
		//directiveLocallocal(ptr)
		ret = 1;
		for (std::vector<std::string>::iterator it = _tokenAvailable.begin(); it !=  _tokenAvailable.end() && ret != 0; it++)
		{
			ret = it->compare(0, strlen(*ptr), *ptr, it->length());
			if (ret == 0)
				*ptr += it->length();
		}
		if (ret != 0 || !isspace(**ptr) )
		{
			syntaxError("error syntaxe: directive name");
		}
		else
		{
			std::cout << "analyse Directive name is successfull " << std::endl;
			directiveValue(ptr);
		}
	}
}

void	ParsingFile::	contentLocation(char const **ptr)
{
	std::string str("location");
	movePointer(ptr);
	int ret = str.compare(0, strlen(*ptr), *ptr, str.length());
	// int ret = str.compare(0, strlen(*ptr), *ptr, str.length());
	std::cout << "ret =========" << ret << std::endl;
	if (ret == 0)
	{
		std::cout << "**********************HELLOW**************************" << std::endl;
		movePointer(ptr);
		if (ptr[0][str.length()] != '{')
		{
			syntaxError("error syntaxe: in location expected open parenthese");
		}
		else
		{
			std::cout << "analyse location is successfully" << std::endl;
			*ptr += str.length() + 1;
			directiveName(ptr);
			if (ptr[0][0] != '}')
				syntaxError("error syntaxe: in location content expected closed parenthese ");
			(*ptr)++;

		}
		movePointer(ptr);
	}
	else
		directiveName(ptr);
}

void	ParsingFile::	parsingServer(char const **ptr)
{
	std::string str("server");
	movePointer(ptr);
	while (**ptr)
	{
		int ret = str.compare(0, strlen(*ptr), *ptr, str.length());
		if (ret != 0)
		{
			syntaxError("error : expected server containt");
		}
		if (ptr[0][str.length()] != '{')
		{
			syntaxError("error syntaxe: expected open parenthese");
		}
		else
		{
			std::cout << "analyse server is successfully" << std::endl;
			*ptr += str.length() + 1;
			// directiveName(ptr);
			contentLocation(ptr);
			if (ptr[0][0] != '}')
				syntaxError("error syntaxe: expected closed parenthese");
			(*ptr)++;
		}
		movePointer(ptr);
	}
}