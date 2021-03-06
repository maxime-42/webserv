

#include "Parse_config_file.hpp"
/*
*THIS IS A SINGLETON CLASS : Singleton design pattern is a software design principle that is used to restrict the instantiation of a class to one object
* this class  get config file and parse them and store it
*/



 void Parse_config_file:: operator=(Parse_config_file &other) {(void)other;}// Singletons should prevent copy or assign

Parse_config_file ::Parse_config_file  (const Parse_config_file & other) {(void)other;};// Singletons should not be cloneable, so it private.


static  std::string  s_fileName;

Parse_config_file & Parse_config_file::							getInstance(std::string fileName)
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

Parse_config_file::												Parse_config_file(): _fileName("./configFile/default.conf"), _configFile(std::string()), _errorHappened(false)
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
Parse_config_file::												Parse_config_file(std::string fileName): _fileName(fileName), _configFile(std::string()), _errorHappened(false)
{
	s_fileName = fileName;
	//std::cout << "****************GET FILE\tFROM\tPARAMETER****************" << std::endl;
	set_defaut_config();

	int result = getStartProcess();
	if (result == ERROR)
	{
		_errorHappened = true;
	}
}

bool	Parse_config_file::										getErrorHappened(){return (_errorHappened);}

/*
**	step one 	:	get file in std::string "configFile"
**	step three	:	parsing file, file is located in std::string _configFile;
*/
void  verrify_error_page(t_single_list &secondList);

int	Parse_config_file::											getStartProcess()
{
	try
	{
		getFile();
		_bracket_counter = 0;
		_previousToken = initialized;
		_indexConfigFile = 0;
		_hisLocation = false;
		set_defaut_config();
		_block_server = _defautConfig;
		parse();
		std::cout << ">>  CONFIG FILE [ OK ]" << std::endl; } catch(char const *  msg_error)
	{
		std::cerr << msg_error << std::endl;
		return (ERROR);
	}
	return (SUCCESS);
}

Parse_config_file::~Parse_config_file(){ }

/***************************************************************alll get function ****************************************/
size_t	Parse_config_file::										numberOfServer()
{
	return (getInstance(s_fileName).interface_numberOfServer());
}

size_t	Parse_config_file::										interface_numberOfServer()
{
	return (_serverList.size());
}


t_nested_list	&												Parse_config_file:: getList()
{
	return(getInstance(s_fileName).interface_getList());
}

t_nested_list	&	Parse_config_file::							interface_getList(){	return(_serverList);}

std::map<std::string, std::string> & Parse_config_file::		get_globalConfig()
{
	return (getInstance(s_fileName).interface_get_globalConfig());
}

std::map<std::string, std::string> & Parse_config_file::		interface_get_globalConfig(){	return (_globalConfig);}


std::vector<int> & Parse_config_file:: 							interface_get_ports () {return (_ports);}

std::vector<int> & Parse_config_file:: 							get_ports () 
{
	return (getInstance(s_fileName).interface_get_ports());
}

void	Parse_config_file::										set_defaut_config()
{
	_defautConfig["allow"] = "PUT GET DELETE POST";
	_defautConfig["listen"] = "8080";
	_defautConfig["cli_max_size"] = "5000000";
	_defautConfig["error page"] = " 404 error.html";
	_defautConfig["server_name"] = "tebi2poney";
	_defautConfig["index"] = "index.html";
	_defautConfig["cgi_pass"] = "/usr/bin/php-cgi";
	_defautConfig["autoindex"] = "on";
	char *pwd = getcwd(NULL, 0);
	if (pwd)
	{
		_pwd = pwd;
		_defautConfig["root"] = pwd;
		_defautConfig["root"] += "/www";
		free(pwd);
	}

}

std::map<std::string, std::string> & Parse_config_file::		get_defaut_config(){return(getInstance(s_fileName).interface_get_defaut_config());}
std::map<std::string, std::string> & Parse_config_file:: 		interface_get_defaut_config () {return (_defautConfig);}

/*********************************************************************************OPEN  FILE* *********************************************************************************/


/**
 * @brief 
 * this function remove comments line in give  string 
 * comment line start with "#"
 * @param line which has  #
 */
void	Parse_config_file::										handleCommentes(std::string &line)
{
	size_t  begin = line.find("#");
	if (begin != std::string::npos)
	{
		line.erase(begin);
	}
}

/**
 * @brief Get the File object
 * get file and store it in std::string @_configFile
 * get file line by line and then concated each line got previously
 * @line contain the line of file
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
 * check if the port is a integer
 * if @str_port is not a integer in string format a error will be  throw  
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


static bool	is_key_word(std::string &word)
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

/**
 * @brief Get the Piece Of string it is a token it return a token on each call
 * the goal is to get a word from string this word will be stored in @pieceOfString
 * this word came from '_configFile'
 * @_configFile is the config file
 * @start is the begin of word 
 * @nbCharacterTocopy number of character to copy from @start
 * @param i is index of string _configFile 
 * @return std::string 
 */
std::string	Parse_config_file::								create_token(size_t &i)
{
	size_t	nbCharacterTocopy = 0;
	size_t start = i;

	if (_configFile[i] == '{' || _configFile[i] == '}' || _configFile[i] == ';')//if { or } or ; is the first character i going to catch that piece Of String
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
std::map<std::string, std::string> Parse_config_file::get_defaut_block_serve(){return (_defautConfig);}
std::string	Parse_config_file::	get_current_directory(){return (_pwd);}

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

/**
 * @brief Set the globalConfig object
 * this function it called when a directive is found out of a block
 * this directive will be stored in special map called "_globalConfig" 
 * _globalConfig[directive_name] = directive_value
 * @param directive_name is the name value of map "_globalConfig" 
 * @param directive_value is the value of map "_globalConfig" 
 */
void				Parse_config_file::	set_globalConfig(std::string directive_name, std::string directive_value)
{
	int bracket_counter = get_bracket_counter();
	if ( bracket_counter == 0)
	{
		_globalConfig[directive_name] = directive_value;
	}
}

/**
 * @brief 
 * @param directive_name contain exactly : "error_page" 
 * @param directive_value contain something like : "xxx path/of/file.html"
 * the pupose is to extract 'xxx' from @param directive_value  and insert that at @param directive_name
 * @param directive_name will be "error page xxx"
 * @param directive_value will be something like : "path/of/file.html"
 */
void	Parse_config_file:: modify_name_error_page(std::string &directive_name, std::string &directive_value)
{
	size_t pos = directive_value.find(" ");
	std::string name = "error_page ";
	name += directive_value.substr(0, pos);
	std::string value = directive_value.substr(pos + 1);
	// std::cout << "name ==" << name << std::endl;
	// std::cout << "value ==" << value << std::endl;
	directive_name = name;
	directive_value = value;
}

/**
 * @brief 
 * check  the syntaxe of directive "errror_page" is fine
 * ensure the right syntax: "error_page XXX path/where/to/go"
 * ensure there is not a absolute path 
 * @param error_page_value string to parse
 * @return true syntax is ok
 * @return false the syntax is not respected
 */
bool	error_page_syntax(std::string error_page_value)
{
	size_t pos = error_page_value.find(" ");
	if (pos != std::string::npos)
	{
		std::string value = error_page_value.substr(pos + 1);
		if (value[0] == '/') /*absolute path start with a slash '/' */
		{
			std::cout << "we received this absolute path == " << value << std::endl;
			std::cout << "error : syntax error absolute path is not accept" << std::endl;
			return (false);
		}
		return (true);
	}

	return (false);
}


/**
 * @brief 
 * this function insure some condition are respect:
 * conditions at respect:
 * 		1 "return" have to be in block location
 *		2 "root" is an absolute path which is existing
 *		3 the port have to be a  number and have not there in block location
 *		4 syntax directive "error_page" 
 * if one of them is not respect, a error throw  will happen
 * @param ptr pointer to the class object let to access to some function
 */
 void		Parse_config_file::		not_allow(Parse_config_file *ptr)
{
	std::string directive_name = ptr->get_directive_name();
	std::string directive_value = ptr->get_directive_value();
	std::string current_directory = ptr->get_current_directory();
	if (directive_name.compare("return") == 0 && ptr->get_hisLocation() == false)
		throw("error : 'return' outside of block location ");
	if (directive_name.compare("root") == 0)
	{
		// std::cout << "current_directory == [" << current_directory << "]  received == [" << directive_value << "]" << std::endl;
		bool ret = is_a_directory(directive_value);
		if (ret == false)
		{
			throw("error : 'root' the path have to be a existing repository");
		}
	}
	if (directive_name.compare("listen") == 0 )
		ptr->checkPort(directive_value);
	if (directive_name.compare("listen") == 0 && ptr->get_hisLocation() == true)
		throw("error : 'listen' have not be in block laction");
	if (directive_name.compare("autoindex") == 0 && directive_value.compare("off") != 0 && directive_value.compare("on") != 0)
		throw("error : 'autoindex' have not be 'off' or 'on' ");
	if (directive_name.compare("error_page") == 0 )
	{
		if (error_page_syntax(directive_value) == false)
			throw("error : syntax on directive error page");
		else
		{
			modify_name_error_page(directive_name, directive_value);
			ptr->set_directive_name(directive_name);
			ptr->set_directive_value(directive_value);
		}

	}
}

/**
 * @brief 
 * semicolon mean it the endline, 
 * at this step it necessary to insert directive_name and directive_value inside a map (block server or bock location or global config)
 * the stake is to know in which map the data will be store
 * through some condition let to know what map to select to store data (directive_name and directive_value)
 * @param ptr pointer to the class object let to access to some function
 * @return true if the function match with the current_word
 * @return false if the function does not match with the current_word
 */
 bool		Parse_config_file::		 has_Semicolon(Parse_config_file *ptr)
{
	std::string current_word = ptr->get_current_word();
	if (current_word.compare(";") == 0)
	{
		token_type previousToken = ptr->get_previousToken();
		if (previousToken == value)
		{
			not_allow(ptr);/*check authorization on some directive name or directive value*/
			ptr->set_previousToken(semicolon);
			if (ptr->get_bracket_counter() == 0)/*were outside block server the data will be store in global config*/
				ptr->set_globalConfig(ptr->get_directive_name(), ptr->get_directive_value());/*go head to store data in global config*/
			else if (ptr->get_hisLocation() == true)/* if where are in block location*/
				ptr->set_block_location(ptr->get_directive_name(), ptr->get_directive_value());/*go head to store data block location*/
			else
				ptr->set_block_server(ptr->get_directive_name(), ptr->get_directive_value());/*we are in server block   the data will store there*/
			return (true);
		}
		else
			throw("/!\\ SYNTAX ERROR: insertMap /!\\");
	}
	return (false);
}

 bool	 				Parse_config_file::					has_Value(Parse_config_file *ptr)
{
	token_type previousToken = ptr->get_previousToken();
	if (previousToken == name || previousToken == location || previousToken == value)
	{
		std::string current_word = ptr->get_current_word();
		std::string directiveValue = ptr->get_directive_value();
		if (previousToken == value)
			directiveValue += " " + current_word;
		else
			directiveValue = current_word;
		ptr->set_directive_value(directiveValue);
		ptr->set_previousToken(value);
		return (true);
	}
	else
	{
		std::cout << "ptr->get_current_word() == [" << ptr->get_current_word() << "]" << std::endl;
		throw("/!\\ SYNTAX ERROR: hasValue /!\\"); }
	return (false);
}

/**
 * @brief 
 *this function try to identify if the 'current_word' it is a keyword then it check the rule of layout
 *the rule  layout of configle  is explain in pdf schema go to take a look
 * the 'current_word' is a key word it will be copy at directive_name
 * @param ptr pointer to the class object let to access to some function
 * @return true if the function match with the current_word
 * @return false if the function does not match with the current_word
 */
 bool 					Parse_config_file::				has_DirectName(Parse_config_file *ptr)
{
	std::string current_word = ptr->get_current_word();
	if (is_key_word(current_word) == true)
	{
		std::string configFile = ptr->get_configFile();
		int result = !isspace(configFile[ptr->get_indexConfigFile()]);/**/
		if (result != 0)/*after a name i should have a space*/
		{
			throw("/!\\ SYNTAX ERROR: a directive name have to seperate with a space /!\\");
		}
		token_type previousToken = ptr->get_previousToken();
		if (previousToken == semicolon || previousToken == brackets_open || previousToken == brackets_close || previousToken == initialized)//the rule of layout of config file
		{
			ptr->set_directive_name(current_word);
			ptr->set_previousToken(name);
			return (true);
		}
		else
			throw("/!\\ SYNTAX ERROR: something went wrong with directive name /!\\");
	}
	return (false);
}

/**
 * @brief 
 * if the current word is '{' the the rule  layout of config file will be apply.
 * if we are are at 'block location' it be necessary to add directive name and directive value in map 'block location'  * the rule  layout of configle  is explain in pdf schema go to take a look
 * @param ptr pointer to the class object let to access to some function
 * @return true if the function match with the current_word
 * @return false if the function does not match with the current_word
 */
 bool						Parse_config_file::				curl_bracket_open(Parse_config_file *ptr)
{
	std::string current_word = ptr->get_current_word();
	if (current_word.compare("{") == 0)
	{
		token_type previousToken = ptr->get_previousToken();
		if (previousToken == server || previousToken == location || previousToken == value)
		{
			int n = ptr->get_bracket_counter();
			n++;
			ptr->set_bracket_counter(n);
			previousToken = brackets_open;
			if (ptr->get_hisLocation() == true)
				ptr->set_block_location(ptr->get_directive_name(), ptr->get_directive_value());
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

/**
 * @brief
 * the stake it is to push server_block or push block_location or linked list
 * if we are out side of any block :
 * 		->push front 'block server' in linked list then push this linked list in other linked list (neestedList)
 * 		->then initialize 'block server' and single linked list  with default value
 * 
 * else if we coming out location block
 * 		->push back 'block_location' in linked list
 * 		->then initialize 'block server' with default value 
 * 
 * @param ptr pointer to the class object let to access to some function
 */
	void						Parse_config_file::			push_someWhere(Parse_config_file *ptr)
{
	std::map <std::string, std::string> block_server = ptr->get_block_server();
	if (ptr->get_bracket_counter() == 0)/*out side of any block */
	{
		ptr->push_front_in_singleList(block_server);
		t_single_list singList = ptr->get_singleList();
		ptr->push_in_neestedList(singList);
		verrify_error_page(singList);
		ptr->set_singleList(t_single_list());
		std::map <std::string, std::string> defaut_config = ptr->get_defaut_block_serve();
		ptr->set_block_server(defaut_config);
		// ptr->set_previousToken(initialized);
	}
	else if (ptr->get_hisLocation() == true)
	{
		std::map <std::string, std::string> block_location = ptr->get_block_location();
		ptr->push_back_in_singleList(block_location);
		ptr->set_block_location(std::map <std::string, std::string>());
		ptr->set_hisLocation(false);
	}
}

/**
 * @brief 
 *	if the current word is '}' the the rule  layout of config file will be apply.
 * when the curl bracket closed it is necessary to push somthing 
 * @param ptr pointer to the class object let to access to some function
 * @return true if the function match with the current_word
 * @return false if the function does not match with the current_word
 */
 bool									Parse_config_file::	curl_bracket_close(Parse_config_file *ptr)
{
	std::string word = ptr->get_current_word();
	if (word.compare("}") == 0)
	{
		token_type previousToken = ptr->get_previousToken();
		if ( previousToken == semicolon || previousToken == brackets_close || previousToken == brackets_open)
		{
			int n = ptr->get_bracket_counter();
			ptr->set_bracket_counter(--n);
			push_someWhere(ptr);
			previousToken = brackets_close;
			ptr->set_previousToken(previousToken);
			return (true);
		}
		else
		{
			throw("Syntaxe error :  HasBracketClose");
		}
	}
	return (false);
}

/**
 * @brief 
 *  if the current word is 'server' the the rule  layout of config file will be apply.
 * the server block in config file have own map called 'block_server' it going to store all directives of a server
 * @param ptr pointer to the class object let to access to some function
 * @return true if the function match with the current_word
 * @return false if the function does not match with the current_word
 */
 bool							Parse_config_file::			block_Server(Parse_config_file *ptr)
{
	std::string current_word = ptr->get_current_word();
	if (current_word.compare("server") == 0)
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
			std::cout << "previousToken == " << previousToken << std::endl;
			throw("Syntaxe error :  hasServer");
		}
	}
	return (false);
}

/**
 * @brief 
 *  if the current word is 'location' the the rule  layout of config file will be apply.
 * the location block in config file have own map called 'block_location' it going to store all directives of a location
 * @param ptr pointer to the class object let to access to some function
 * @return true if the function match with the current_word
 * @return false if the function does not match with the current_word
 */
bool										Parse_config_file::			has_location_block(Parse_config_file *ptr)
{
	std::string current_word = ptr->get_current_word();
	if (current_word.compare("location") == 0)
	{
		token_type	previousToken = ptr->get_previousToken();
		if ((previousToken == semicolon || previousToken == brackets_open || previousToken == brackets_close) && ptr->get_hisLocation() == false)
		{
			ptr->set_directive_name(current_word);
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

typedef bool (Parse_config_file:: *method_function)(Parse_config_file *);

void											display_neestedList(t_nested_list firstList);
void											displaySingleList(std::list<std::map < std::string, std::string > > &linkedList);

/**
 * @brief 
 * the function of array 'array_method' going try to identify the _current_word
 * if the function identify  @_current_word'  @return_method will be equal to true otherwise false
 * one time a word has been identify the loop will stop cause @i = SIZE_ARRAY_FUNC
 */
void											Parse_config_file:: method_lookup()
{
	bool return_method;
	/*all methode in this array let to done parse  each token = @_current_word, to knowing the rules of parse go take a look the diagram */
	method_function array_method[SIZE_ARRAY_FUNC] = {&Parse_config_file::block_Server, &Parse_config_file::curl_bracket_open, &Parse_config_file::curl_bracket_close, &Parse_config_file::has_DirectName, &Parse_config_file::has_Semicolon, &Parse_config_file::has_location_block };
	for (size_t i = 0; i < SIZE_ARRAY_FUNC; i++)
	{
		/* this loop  look  up the methode match up with the @_current_word then apply the rule parsing*/
		method_function method = array_method[i];
		return_method = (this->*method)(this);
		if (return_method == true)
		{
			/*this instruction let to stop the loop it meant the @_current_word has been identify*/
			i = SIZE_ARRAY_FUNC;
		}
	}
	/*if nothing has been find this instruction will be execute by default*/
	if (return_method == false)
		has_Value(this);
}

/**
 * @brief
 * this function skip whitespace charactere character to take a word of file, then @method_lookup will working with that word  
 * skim the config file by getting word by word (_current_word)
 */
void													Parse_config_file::parse()
{
	for (; _indexConfigFile < _configFile.size();)
	{
		if (!isspace(_configFile[_indexConfigFile]))
		{
			_current_word = create_token(_indexConfigFile);
			method_lookup();
		}
		else
		{
			/*increment to go on the next character*/
 			_indexConfigFile++;
		}
	}
}
