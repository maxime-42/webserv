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

