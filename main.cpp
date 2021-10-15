#include "ParsingFile.hpp"

int main(int argc, char const *argv[])
{
	ParsingFile test;
	(void)argc;
	(void)argv;
	return 0;
}

// #include <map>
// #include <string>

// int main()
// {
//     std::map< std::string, std::map< int, double > > my_map ;

//     my_map[ "hello" ][ 23 ] = 8.9 ;
// }

using namespace std;
using namespace std;
  
// int main()
// {
//     string str = "geeksforgeeks a computr science";
//     string str1 = "computer";
  
//     // Find first occurrence of "geeks"
//     size_t found = str.find(str1);
//     if (found != string::npos)
//         cout << "First occurrence is " << found << endl;
// 	else
// 		std::cout << "not found" << std::endl;
//     // Find next occurrence of "geeks". Note here we pass
//     // "geeks" as C style string.
//     // char arr[] = "geeks";
//     // found = str.find(arr, found+1);
//     // if (found != string::npos)
//     //     cout << "Next occurrence is " << found << endl;
//     return 0;
// }

// int main()
// {
      
// //     // char line []= "   server     {name value;name value;name value;location{name value;}} server     {name value;name value;name value;location{name value;name value;name value;}}";
//     char line []= "  server {       index   youpi ";
      
//     // Vector of string to save tokens
//     vector <string> tokens;
//     // stringstream class check1
//     stringstream check1(line);
      
//     string intermediate;
      
//     // Tokenizing w.r.t. space ' '
//     while(getline(check1, intermediate, '{'))
//     {
//         tokens.push_back(intermediate);
//     }
      
//     // Printing the token vector
//     for(size_t i = 0; i < tokens.size(); i++)
//         cout << tokens[i] << '\n';
// }