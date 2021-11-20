#ifndef LOOKUP_IN_NESTED_LIST
# define LOOKUP_IN_NESTED_LIST

#include <sstream> 
#include "header.hpp"
#include "ParsingFile.hpp"

typedef std::list < std::list < std::map < std::string, std::string > > > t_nested_list;
typedef std::list<std::map < std::string, std::string > > t_single_list;

bool							find_location(t_single_list & secondList, std::string &elemToFind, void *ptrReponse);
bool							find_directive(t_single_list & secondList, std::string &elemToFind, void *ptrReponse);
bool							getInfo(int port, std::string elemToFind, void *reponse, bool(*callback)(t_single_list & secondList, std::string &elemToFind, void *ptrReponse));

bool	 						isNumber(std::string &str);
#endif
