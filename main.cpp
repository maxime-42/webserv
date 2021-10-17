#include "ParsingFile.hpp"
// #include <cstdint>
using namespace std;
using namespace std;

int main(int argc, char const *argv[])
{
	// smallenum test = a;
	// std::cout << "value= " << test << std::endl;
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

// int main()
// {
//  // Declaration of string
//     char gfg[100] = "Geeks-for-geeks-Contribute lenox kay";
  
//     // Declaration of delimiter
//     const char s[4] = "-";
//     char* tok;
  
//     // Use of strtok
//     // get first token
//     tok = strtok(gfg, s);
  
//     // Checks for delimeter
//     while (tok != 0) {
//         printf(" %s\n", tok);
  
//         // Use of strtok
//         // go through other tokens
//         tok = strtok(0, s);
//     }
  
//     return (0);
// }
// int main()
// {
//     char str[] = "Geeks for Geeks lenox-kay";
//     char *token;
//     char *rest = str;
//     while ((token = strtok_r(rest, "-", &rest)))
//         printf("%s\n", token);
//     return(0);
// }

/*
server {
	server_name youpi;
	listen 8080;

	root /YoupiBanane;
	index index.html;

	autoindex off;

	error_page	/www/error;
	
	allow_methods GET DELETE;

	location /upload {
		allow_methods POST;
		client_max_body_size 10;
		upload_pass ./YoupiBanane;
	}

	location /directory {
		allow_methods GET;
		root YoupiBanane/;
		index youpi.bad_extension;
	}

	location *.bla {
		allow_methods POST ;
		client_max_body_size 5;
		cgi	.bla;
		cgi_pass testers/ubuntu_cgi_tester;
	}

	location *.php {
		allow_methods POST GET;
		client_max_body_size 100;
		cgi	.php;
		cgi_pass testers/php-cgi_ubuntu;
	}

	location /Yeah {
		return 301 http://localhost:8080/nop/;
	}
}

*/