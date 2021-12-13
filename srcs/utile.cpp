#include <sstream>
#include <string>
#include <vector>
#include "utile.hpp"

/**
 * @brief 
 * this function converting an integer into a string * 
 * @param value  the integer to converting
 * @return std::string value in string format
 */
std::string get_value_in_string(int value)
{
	std:: stringstream ss;
	ss << value;
	std::string str_value ;
	ss >> str_value;
	return (str_value);
}

/*
**	this function take the list which has the server which has the port given in parameter 
**	afterwards i look up over the list which has the specific port to get the element wanted
*/


/********************************************************************************ACCESS TO ELEM it _serverList it is a neested list************************************************************************************/

/*
** i will comment to later...
*/

/**
 * @brief 
 * skim the linked list at looking a location 
 * @param secondList linked  list to skim
 * @param elemToFind the location ton find
 * @param ptrReponse will contains the location
 * @return true if the location was  found
 * @return false if the location was not found 
 */
bool		find_location(t_single_list & secondList, std::string &elemToFind, void *ptrReponse)
{
	t_single_list::iterator itr_secondList;
	std::map < std::string, std::string > dictionary;
	std::map < std::string, std::string >::iterator itr_dictionary;

	for (itr_secondList = secondList.begin();  itr_secondList != secondList.end(); itr_secondList++)
	{
		dictionary = *itr_secondList; //to get dictionary of each node
		itr_dictionary = dictionary.find("location");// identify port
		if (itr_dictionary != dictionary.end() && elemToFind.compare(itr_dictionary->second) == 0)
		{
			std::map<std::string, std::string> *ptrMap = (std::map<std::string, std::string> *) ptrReponse;
			(*ptrMap) = dictionary;
			return (true);
		}
	}
	return (false);
}


/**
 * @brief 
 * this function check if the dictionary contain a location
 * if there are a location it return true else return false
 * @param dictionary where to check the location 
 * @return true if the "location" is in map
 * @return false if the "location" is not in map
 */
static bool	isLocation(std::map < std::string, std::string > &dictionary)
{
	std::map < std::string, std::string >::iterator itr_dictionary;
	itr_dictionary = dictionary.find("location");
	if (itr_dictionary != dictionary.end())
	{
		return (true);
	}
	return (false);
}


/**
 * @brief find directive in server block
 * take a item in map if is exist
 * if item is there in map return true and store item found in ptrReponse
 * otherwise return false
 * @param secondList the list which is in the first list
 * @param elemToFind the item which is look up in map
 * @param ptrReponse 	the result of looking will store in
 * @return true 	if the elemToFind has been found 
 * @return false 	if the elemToFind was not been found 
 */
bool		find_directive(t_single_list & secondList, std::string &elemToFind, void *ptrReponse)
{
	t_single_list::iterator itr_secondList;
	std::map < std::string, std::string > dictionary;
	std::map < std::string, std::string >::iterator itr_dictionary;
	std::string *ptrString = NULL;
	itr_secondList = secondList.begin();
	dictionary = *itr_secondList; //to get dictionary of  node
	itr_dictionary = dictionary.find(elemToFind);// identify port
	bool is_location = isLocation(dictionary);
	if (itr_dictionary != dictionary.end() && is_location == false)
	{
		ptrString = (std::string *) ptrReponse;
		*ptrString = itr_dictionary->second;
		return (true);
	}
	return (false);
}

/**
 * @brief Get the Info of a server block
 * 
 * 
 * **To know:
 * t_nested_list	= firstList = itr_firstList
 * t_single_list	= secondList = itr_secondList
 *
 * this function let to get accurate info on server
 * the server is identify through a port
 * I look up the server which has this specific port to get info on it
 *
 *	first loop it for the nested list, second loop for it for the single list.
 *	Each node of single list there a dictionary which store data 
 *
 * In the second loop the instructions is to :
 *	firstly find the port, once  port find  "callback" it called
 * @param port of the server  
 * @param elemToFind  the item which is look up 
 * l'element que vous cherché pour une location exemple si tu as dans le fichier conf "location toto{...}" ici t'aura juste a passé "toto"
 * pour une directive il faut passé le nom, exemple si tu as dans le fichier conf "server_name tata" dans se cas tu va passé "server_name"
 * 
 * @param reponse the result of looking will store in
 * là ou la reponse va être stocker, donnée l'adresse de "variable"
 *	pour une location donneée l'adresse d'une map 
 *	pour une directive donnée l'adresse d'une string 
 * @param callback il faut passé une fonction en parametre si tu chercher une location tu passe la fonction "find_location" 
 * sinon si c'est une directive que tu cherche tu passe la fonction "find_directive"
 * 
 * @return true  if the elemToFind has been found 
 * @return false  if the elemToFind was not been found 
 */
bool  getInfo(int port, std::string elemToFind, void *reponse, bool(*callback)(t_single_list & secondList, std::string &elemToFind, void *ptrReponse))
{
	bool has_find = false;
	t_nested_list firstList =  Parse_config_file::getList();
	std::string str_port = get_value_in_string(port);//get value of port under string format
	std::map < std::string, std::string > dictionary;
	std::map < std::string, std::string >::iterator itr_dictionary;
	t_single_list::iterator itr_secondList;
	for (t_nested_list::iterator itr_firstList = firstList.begin();  itr_firstList != firstList.end(); itr_firstList++)
	{
		t_single_list & secondList  = *itr_firstList;
		for (itr_secondList = secondList.begin();  itr_secondList != secondList.end(); itr_secondList++)
		{
			dictionary = *itr_secondList; //to get dictionary of each node
			itr_dictionary = dictionary.find("listen");// identify port
			if (itr_dictionary != dictionary.end() && str_port.compare(itr_dictionary->second) == 0)
			{
				has_find = callback(secondList, elemToFind, reponse);
				if (has_find)
					return (has_find);
			}
		}
	}
	return (has_find);	//return false , the element has not be found
}



/**
 * @brief Get the location url object
 * look up  through each word on url delimited by  back slash if is a location block in server block
 * split "url" with help to  function "strtok"
 * "strtok" function return a token on each backloop
 * then concaten '/' and token which going to generate new string "elemToFind"
 * @param port server
 * @param url to skim 
 * @param ptrReponse 	the result of looking will store in
 * @return true 	if the elemToFind has been found 
 * @return false 	if the elemToFind was not been found 
 */
bool			get_location_url(int port, std::string url, void *ptrReponse)
{
	bool		ret;
	char		*token = strtok((char *)url.c_str(), "/");

	while (token != NULL)
	{
		std::string	elemToFind = "/";
		elemToFind.append(token);
		ret = getInfo(port, elemToFind, ptrReponse, find_location);
		if (ret == true)
			return (true);
		token = strtok(NULL, "/");
	}
	return (false);
}

/**
 * @brief 
 * verify if the given string in parameter matches to a directory
 * @param s name of directory
 * @return true if is a exiting directory
 * @return false if it is not a exiting directory
 */
bool is_a_directory(const std::string &s)
{
  	struct stat buffer;
  	return (stat (s.c_str(), &buffer) == 0 && buffer.st_mode & S_IFDIR); // if exist && is a directory return 1
}

/**
 * @brief 
 * verify if the given string in parameter matches to a file
 * @param s name of directory
 * @return true if is a exiting file
 * @return false if it is not a exiting file
 */
bool exists(const std::string &s)
{
	struct stat buffer;
	return (stat (s.c_str(), &buffer) == 0);
}

/**
 * @brief 
 * this function verify the l'existence of error page through the concatenation  of two string.
 * the concatenated string  is send to a other function which tell us  the existence or not of  file 
 * @param error_page_value contain the file name of error page 
 * @param path_root contain the absolute path of error page
 * @return true the file existe 
 * @return false the file doesn't existe 
 */
// bool 	verrify_path_error_page(std::string &error_page_value, std::string &path_root)
// {
// 	bool ret;
// 	// if (error_page_value[0] == '/')
// 	// {
// 	// 	std::cout << "chemin absolut" << std::endl;
// 	// 	ret = exists(error_page_value);
// 	// }
// 	// else
// 	// {
// 		// std::cout << "chemin relatif == " << error_page_value<< std::endl;
// 		std::string error_page_path = path_root + "/" + error_page_value;
// 		/* @error_page_path is the concatenate string*/
// 		// std::cout << "test = [" << error_page_path <<"]" << std::endl;
// 		ret = exists(error_page_path);/* this function return true or false according to the existence of file*/
// 	// }
// 	return (ret);
// }

/**
 * @brief 
 * look up in the dictionationary to find a error_page path and likely a path root of this dictionnary
 * the pupose is to find a error page 
 * this function verify the l'existence of error page through the concatenation  of two string.
 * the concatenated string  is send to a other function which tell us  the existence or not of  file 
 * @param dictionary a dictionary match up with a block server or block location 
 * @param path_error_page reference where to store the error page
 * @param root_location reference where to store the path of root
 * @return true there are a a error page in this block
 * @return false there are not a error page in this block
 */
static bool	there_error_page(std::map < std::string, std::string > &dictionary, std::string &path_error_page, std::string &root_location)
{
	for (std::map < std::string, std::string >::iterator it = dictionary.begin(); it != dictionary.end() ; it++)
	{
		int cmp = it->first.compare(0, 10, "error_page");
		if (cmp == 0)
		{
			/*a error page has been find*/
			path_error_page = it->second;
			if (dictionary.find("root") != dictionary.end())/*check if the are a path root */
				root_location = dictionary["root"];
			// std::cout << "root_location == " << root_location << std::endl;
			return (true);
		}
	}
	return (false);
}
/**
 * @brief 
 * skim the linked list a take a look at on each node if there are a error page 
 * a node meant a dictionary
 * then skim the dinctionary to check the error page and likely a path root
 * @there_error_page skim the dictionary of linked list
 * @param secondList the linked liste to skim
 */
void  verrify_error_page(t_single_list &secondList)
{
	bool ret = true;
	std::map < std::string, std::string > dictionary;
	std::map < std::string, std::string >::iterator it;
	t_single_list::iterator itr_secondList = secondList.begin();
	dictionary = *itr_secondList;
	std::string path_error_page;
	std::string root_location;
	std::string server_root = dictionary["root"]; 
	int i = 0;
	for (;  itr_secondList != secondList.end(); itr_secondList++)
	{
		i++;
		if (there_error_page(*itr_secondList, path_error_page, root_location) == true)
		{
			/*if a error page has been find path_error_page will contain the path same to root_location*/
			if (i == 0)
			{
				// std::cout << "block server"<< std::endl;
				// ret = verrify_path_error_page(dictionary["error_page"], dictionary["root"]);
				ret = exists( dictionary["root"]+ "/" + dictionary["error_page"]);/* this function return true or false according to the existence of file*/
			}
			else
			{
				if (root_location.size() == 0)
				{
					/*the block location doesn't have a root inside the dictionary the why it take the root of block server*/
					root_location = server_root;
				}
				if (dictionary.find("root") != dictionary.end())
				{
					// std::cout << "block location with root"<< std::endl;
					// ret = verrify_path_error_page(path_error_page, root_location);
					ret = exists(root_location + "/" + path_error_page);/* this function return true or false according to the existence of file*/
				}
				else
				{
					// std::cout << "block location without root"<< std::endl;
					// ret = verrify_path_error_page(path_error_page, server_root);
					ret = exists(server_root + "/" + path_error_page);/* this function return true or false according to the existence of file*/
				}
			}
		}
		if (ret == false)
			throw("error: path of 'error page' doesn't existed");
	}
}
