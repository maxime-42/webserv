#include "Service.hpp"
#include<sstream> 
#include "header.hpp"
typedef std::list < std::list < std::map < std::string, std::string > > > t_nested_list;
typedef std::list<std::map < std::string, std::string > > t_single_list;

void	displayDirectionary(std::map<std::string, std::string> &map);

/*
** All function in this file consist to retrieve info inside list of server
*/

/*
**	this function converting an integer into a string
**  the integer to converting is  given in parameter
*/
static std::string get_value_in_string(int value)
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
static bool	look_over_singleList(t_single_list & secondList, std::string elemToFind, std::map<std::string, std::string> &dictionary_find)
{
	std::map < std::string, std::string > dictionary;
	std::map < std::string, std::string >::iterator itr_dictionary;
	std::list < std::map < std::string, std::string > >::iterator itr_second_dimension;
	for (itr_second_dimension = secondList.begin();  itr_second_dimension != secondList.end(); itr_second_dimension++)
	{
			dictionary = *itr_second_dimension;
			itr_dictionary = dictionary.find(elemToFind);
			if (itr_dictionary != dictionary.end())
			{
				dictionary_find = dictionary;
				return (true);
			}
	}
	return (false);
}
/*
**To know:
** t_nested_list	= firstList = itr_firstList
** t_single_list	= secondList = itr_secondList
**
** this function let to get accurate info on server
** the server is identify through a port
** I look up the server which has this specific port to get info on it
**
***	first loop it for the nested list, second loop for it for the single list.
**	Each node of single list there a dictionary which store data 
**
** In the second loop the instructions is to :
**	firstly find the port, once  port find,  function "look_over_list" it called
*/
bool  getInfo(t_nested_list firstList, int port, std::string elemToFind, std::string &reponse)
{
	bool has_find = false;
	std::string str_port = get_value_in_string(port);//get value of port under string format
	std::map < std::string, std::string > dictionary;
	std::map < std::string, std::string >::iterator itr_dictionary;
	t_nested_list::iterator itr_firstList = firstList.begin(); 
	t_single_list::iterator itr_secondList;
	for (;  itr_firstList != firstList.end(); itr_firstList++)
	{
		t_single_list & secondList  = *itr_firstList;
		for (itr_secondList = secondList.begin();  itr_secondList != secondList.end(); itr_secondList++)
		{
			dictionary = *itr_secondList; //to get dictionary of each node
			itr_dictionary = dictionary.find("listen");// identify port
			if (itr_dictionary != dictionary.end() && str_port.compare(itr_dictionary->second) == 0)// if the port exist
			{
				has_find = look_over_singleList(secondList, elemToFind, dictionary);// loopback in the  single list to retrieve the map which contain  the item to search
				if (has_find == true)
				{
					itr_dictionary = dictionary.find(elemToFind);
					reponse = itr_dictionary->second;
					return(has_find); // the element has be found
				}
			}
		}
	}
	return (has_find);	//return false , the element has not be found
}

/********************************************************************************ACCESS TO ELEM it _serverList it is a neested list************************************************************************************/
/*
**To know:
** t_nested_list	= firstList = itr_firstList
** t_single_list	= secondList = itr_secondList
**	if nothing has been find it return empty string
** 	if nothing has been find, it return empty string
**	maybe you need to glance diagram of parsing to really caught how i look up data in this nested list

**	first loop it for the main list, second loop for it for the nested list.
**	It has to know each node of nested list there a dictionary which store data 
*/
std::string							getElem(t_nested_list firstList, size_t lineServer, std::string elem)
{
	std::map < std::string, std::string > dictionary;
	std::map < std::string, std::string >::iterator itr_directionary;
	t_nested_list::iterator itr_firstList = firstList.begin() ;
	t_single_list::iterator itr_secondList;
	for (; itr_firstList != firstList.end(); itr_firstList++)
	{
		if (lineServer == 0)
		{
			t_single_list & secondList  = *itr_firstList;//
			for (itr_secondList = secondList.begin();  itr_secondList != secondList.end(); itr_secondList++)
			{
				dictionary = *itr_secondList;
				itr_directionary = dictionary.find(elem);
				if (itr_directionary != dictionary.end())
					return (itr_directionary->second);
			}
		}
		lineServer--;
	}
	return (std::string());	//empty string
}

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

bool		find_directive(t_single_list & secondList, std::string &elemToFind, void *ptrReponse)
{
	t_single_list::iterator itr_secondList;
	std::map < std::string, std::string > dictionary;
	std::map < std::string, std::string >::iterator itr_dictionary;
	std::string *ptrString = NULL;
	for (itr_secondList = secondList.begin();  itr_secondList != secondList.end(); itr_secondList++)
	{
		dictionary = *itr_secondList; //to get dictionary of each node
		itr_dictionary = dictionary.find(elemToFind);// identify port
		if (itr_dictionary != dictionary.end())
		{
			ptrString = (std::string *) ptrReponse;
			*ptrString = itr_dictionary->second;
			return (true);
		}
	}
	return (false);
}

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