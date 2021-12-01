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
 * @param secondList 
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
	t_nested_list firstList =  ParsingFile::getList();
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