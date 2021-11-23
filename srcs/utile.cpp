#include <sstream>
#include <string>
#include <vector>

/*
** Returns true if given string in parameter is a number else false
*/

bool	 						isNumber(std::string &str)
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

int								convert_string_to_integer(std::string &str)
{
	int 						number;
	std:: stringstream ss(str);
	ss >> number;
	return (number);
}

/*
** this function check if item is already in vector 
** it is to prevent duplication
** each item in vector must  be uniquely
*/
bool							check_duplicated(std::vector<int>&vect, int item_to_search)
{
	for (size_t i = 0; i < vect.size(); i++)
	{
		if (vect[i] == item_to_search)
			return (true);
	}
	return(false);
}