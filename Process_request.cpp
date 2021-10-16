#include "Create_server.hpp"
#include <cstring>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <map>

void        Create_server::process_GET(std::map<std::string, std::string> request, std::map<std::string, std::string> & reponse)
{
    std::string path;

    if (request["url"] == "/")
        request["url"] = "/index.html";

    path += request["url"];
    std::ifstream	ifs(path.c_str());
    if (ifs.fail())
    {
        //perror("Could not open the file");
        reponse["code"] = "404";
        reponse["status"] = "File Not Found";
    } 
    else 
    {

        std::stringstream buf;
        buf << ifs.rdbuf();
        std::string	filestr = buf.str();

        ifs.close();

        reponse["code"] = "200";
        reponse["status"] = "OK";
        reponse["body"] = filestr;
        reponse["Content-Length"] = filestr.length();

        reponse["Content-Type"] = "text/plain; charset=utf-8";
        if(request["url"].substr(request["url"].find_last_of(".") + 1) == "html")
            reponse["Content-Type"] = "text/html; charset=utf-8";

    }
}

void    Create_server::process_POST(std::map<std::string, std::string> request, std::map<std::string, std::string> & reponse)
{
    (void)request;
    (void)reponse;
}
