#include "Service.hpp"
#include<sstream> 

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
static std::string	look_over_list(std::list<std::map < std::string, std::string > > & single_list, std::string elemToFind)
{
	std::map < std::string, std::string > dictionary;
	std::map < std::string, std::string >::iterator itr_dictionary;
	std::list < std::map < std::string, std::string > >::iterator itr_second_dimension;
	for (itr_second_dimension = single_list.begin();  itr_second_dimension != single_list.end(); itr_second_dimension++)
	{
			dictionary = *itr_second_dimension;
			itr_dictionary = dictionary.find(elemToFind);
			if (itr_dictionary != dictionary.end())
			{
				return (itr_dictionary->second);
			}
	}
	return (std::string());
}
/*
** get info in specific server, the server is identify by port
** I look up the server wich has this specific port to get of the server
**
***	first loop it for the main list, second loop for it for the nested list.
**	Each node of nested list there a dictionary which store data 
**
** In the instruction of second loop , i firstly i try to find the port
** In the second loop the instructions is to find the port, once  port find,  function "look_over_list" it called

*/
std::string  getInfo(std::list < std::list < std::map < std::string, std::string > > >nestedList, int port, std::string elemToFind)
{
	std::string to_return("");
	std::string str_port = get_value_in_string(port);
	std::map < std::string, std::string > dictionary;
	std::map < std::string, std::string >::iterator itr_dictionary;
	std::list < std::list < std::map < std::string, std::string > > >::iterator itr_first_dimension = nestedList.begin(); 
	std::list < std::map < std::string, std::string > >::iterator itr_second_dimension;
	for (;  itr_first_dimension != nestedList.end(); itr_first_dimension++)
	{
		std::list<std::map < std::string, std::string > > & single_list  = *itr_first_dimension;
		for (itr_second_dimension = single_list.begin();  itr_second_dimension != single_list.end(); itr_second_dimension++)
		{
			dictionary = *itr_second_dimension;
			itr_dictionary = dictionary.find("listen");
			if (str_port.compare(itr_dictionary->second) == 0)
			{
				to_return = look_over_list(single_list, elemToFind);
				if (to_return.length())
					return(to_return);
			}
		}
	}
	return (std::string());	//empty string
}

/********************************************************************************ACCESS TO ELEM it _serverList it is a neested list************************************************************************************/
/*
**	if nothing has been find it return empty string
** 	if nothing has been find, it return empty string
**	maybe you need to glance diagram of parsing to really caught how i look up data in this nested list

**	first loop it for the main list, second loop for it for the nested list.
**	It has to know each node of nested list there a dictionary which store data 
*/
std::string							getElem(std::list < std::list < std::map < std::string, std::string > > > nestedList, size_t lineServer, std::string elem)
{
	std::map < std::string, std::string > dictionary;
	std::map < std::string, std::string >::iterator it;
	std::list < std::list < std::map < std::string, std::string > > >::iterator first = nestedList.begin() ;
	std::list < std::map < std::string, std::string > >::iterator itrSingle_list_pointer;
	for (; first != nestedList.end(); first++)
	{
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
		lineServer--;
	}
	return (std::string());	//empty string
}
