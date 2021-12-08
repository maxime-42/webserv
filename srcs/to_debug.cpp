#include <string>
#include <map>
#include <iostream>
#include <list>


typedef std::list < std::list < std::map < std::string, std::string > > > t_nested_list;
typedef std::list<std::map < std::string, std::string > > t_single_list;

void		displayDirectionary(std::map<std::string, std::string> &Map)
{
	std::cout << ">>>>>dictionnay<<<<<" << std::endl;
	for (std::map<std::string, std::string>::iterator it = Map.begin(); it != Map.end(); it++)
	{
		std::cout << "[" << it->first << "]\t[" << it->second << "]" << "\n";
	}
}

void							display_singleList(std::list<std::map < std::string, std::string > > &secondList)
{
	t_single_list::iterator itr_secondList;
	std::map < std::string, std::string > dictionary;
	std::map < std::string, std::string >::iterator itr_dictionary;

	for (itr_secondList = secondList.begin();  itr_secondList != secondList.end(); itr_secondList++)
	{
		dictionary = *itr_secondList; //to get dictionary of each node
		displayDirectionary(dictionary);
	}
}


void	display_neestedList(t_nested_list firstList)
{
	int i = 1;
	for (t_nested_list::iterator itr_firstList = firstList.begin();  itr_firstList != firstList.end(); itr_firstList++)
	{
		t_single_list & secondList  = *itr_firstList;
		std::cout << "node == " << i << std::endl;
		display_singleList(secondList);
		std::cout <<  std::endl;
		i++;
		
	}
}

void print_arg(char **arg)
{
	int i = 0;
	while (arg[i])
	{
		std::cout << "["<< arg[i] << "]" << std::endl;
		i++;
	}
	
}
// int main(void)
// {
// 	std::cout << "/* message */" << std::endl;
// 	return 0;
// }