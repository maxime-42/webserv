#ifndef UTILE_HPP
# define UTILE_HPP

#include <sstream> 
#include <sys/stat.h>
#include "header.hpp"
#include "Parse_config_file.hpp"

typedef std::list < std::list < std::map < std::string, std::string > > > t_nested_list;
typedef std::list<std::map < std::string, std::string > > t_single_list;

bool							find_location(t_single_list & secondList, std::string &elemToFind, void *ptrReponse);
bool							find_directive(t_single_list & secondList, std::string &elemToFind, void *ptrReponse);
bool							getInfo(int port, std::string elemToFind, void *reponse, bool(*callback)(t_single_list & secondList, std::string &elemToFind, void *ptrReponse));
bool							get_location_url(int port, std::string url, void *ptrReponse);
std::string						get_value_in_string(int value);
bool							is_a_directory(const std::string &s);
bool 							exists(const std::string &s);

#endif