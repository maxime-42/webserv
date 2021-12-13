#include <sstream>
#include <string>
#include <vector>

/*
** Returns true if given string in parameter is a number else false
*/

bool							is_hex(std::string const& s)
{
  	return s.find_first_not_of("0123456789abcdefABCDEF") == std::string::npos;
}

int         ft_skip_space(std::string request_str, size_t i, int mode)
{
    if (mode == 0)
    {
        size_t str_end = request_str.size();

        while (i < str_end)
        {
            if (request_str[i] != ' ')
            {
                break ;
            }
            i++;
        }
        return (i);
    }
    while (i > 0)
    {
        if (request_str[i] != ' ')
        {
            break ;
        }
        i--;
    }
    return (i);
}

std::string		time_to_string() {

	std::string	week[7] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
	std::string month[12] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};

    std::time_t t = std::time(0);   // get time now
    std::tm* now = std::gmtime(&t);
	std::stringstream ss;
	ss	<< week[now->tm_wday] << ", "
		<< now->tm_mday << " "
        << (month[now->tm_mon]) << " "
		<< (now->tm_year + 1900) << " " 
		<< now->tm_hour << ":"
		<< now->tm_min << ":"
		<< now->tm_sec << " GMT";

	return ss.str();
}

bool is_a_directory(const std::string &s)
{
  	struct stat buffer;
  	return (stat (s.c_str(), &buffer) == 0 && buffer.st_mode & S_IFDIR); // if exist && is a directory return 1
}

bool exists(const std::string &s)
{
	struct stat buffer;
	return (stat (s.c_str(), &buffer) == 0);
}

struct my_equal {
    bool operator()(char ch1, char ch2) {
        return std::toupper(ch1) == std::toupper(ch2);
    }
};

// find substring (case insensitive)
int ci_find_substr( const std::string& str1, const std::string& str2 )
{
	std::string::const_iterator it = std::search( str1.begin(), str1.end(),
        	str2.begin(), str2.end(), my_equal() );
    if ( it != str1.end() ) return it - str1.begin();
    else return -1; // not found
}



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
