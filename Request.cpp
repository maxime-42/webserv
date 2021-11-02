#include "Request.hpp"

Request::Request() {}

Request::~Request() {}

void      Request::parse(std::string request_str) {

    std::istringstream iss(request_str);
    std::string tmp;
    std::string key;
    std::string val;

    reponse.clear();

    getline(iss, header["method"], ' ');
    getline(iss, header["url"], ' ');
    getline(iss, header["http"], '\r');
    if (header["method"] == "" || header["url"] == "" || header["http"] == "")
        http_code("400");
    if (header["url"][0] != '/')
        http_code("400");
    if (header["http"] != "HTTP/1.1")
        http_code("505");

    getline(iss, tmp, '\n');
    if (tmp != "")
        http_code("401");

    while(getline(iss, tmp, '\n')) {

        if (size_t pos = val.find(":") != tmp.npos) {
            key = tmp.substr(0, pos);
            val = tmp.substr(pos + 1, tmp.npos - pos);
        }
        else
            http_code("402");

        header[key] = val;
        //key.erase(std::remove(key.begin(), key.end(), ':'), key.end());
    }

    for (std::map<std::string, std::string>::iterator it = header.begin(); it != header.end(); ++it) {
        std::cout << it->first << ":" << it->second << std::endl;
    }
    (void)reponse;
}

void        Request::process() {


    if (reponse.find("code") != reponse.end())
        return ;

    if (header["method"] == "GET") 
        _process_GET();
    else if (header["method"] == "POST")
        _process_POST();
    else if (header["method"] =="DELETE")
        _process_DELETE();
    else
        http_code("405");

}

void        Request::send_reponse(int socket) {

    reponse["http_version"] = "HTTP/1.1";

    std::string reply = reponse["http_version"] + " " + reponse["code"] + " " + reponse["status"] + "\n";

    if (reponse.find("body") == reponse.end())
        reply.append("\n");
    else {
        reply.append("Content-Type: " + reponse["Content-Type"] + " \n");
        reply.append("Content-Length: " + reponse["Content-Length"] + " \n");
        reply.append("\n");
        reply.append(reponse["body"]);
    }

    write(socket, reply.c_str(),reply.length());


    write(1, "\nREPONSE:\n\n", 12);
    write(1, reply.c_str(),reply.length());
    write(1, "\n\n", 2);


}

void        Request::_process_GET()
{
    std::string path;

    if (header["url"] == "/")
        header["url"] = "index.html";
    else if (header["url"][0] == '/')
        header["url"].erase(0,1);

    path += header["url"];

    std::ifstream	ifs(path.c_str());
    if (ifs.fail())
        http_code("404");
    else 
    {

        std::stringstream buf;
        std::stringstream content_len;
        buf << ifs.rdbuf();
        std::string	filestr = buf.str();

        ifs.close();

        http_code("200");
        reponse["body"] = filestr;
        content_len << filestr.length();
        reponse["Content-Length"] = content_len.str();

        reponse["Content-Type"] = "text/plain; charset=utf-8";
        if(header["url"].substr(header["url"].find_last_of(".") + 1) == "html")
            reponse["Content-Type"] = "text/html; charset=utf-8";

    }
}

void    Request::_process_POST()
{

}

void    Request::_process_DELETE()
{

}

void    Request::http_code(std::string http_code) {

    std::map<std::string, std::string> http = http_table();

    reponse["code"] = http_code; 
    reponse["status"] = http[http_code];
}

std::map<std::string, std::string> Request::http_table()
{
    std::map<std::string, std::string> http;

    http[ "100" ] = "Continue";
    http[ "101" ] = "Switching Protocols";
    http[ "102" ] = "Processing";
    http[ "103" ] = "Checkpoint";

    http[ "200" ] = "OK";
    http[ "201" ] = "Created";
    http[ "202" ] = "Accepted";
    http[ "203" ] = "Non-Authoritative Information";
    http[ "204" ] = "No Content";
    http[ "205" ] = "Reset Content";
    http[ "206" ] = "Partial Content";
    http[ "207" ] = "Multi-Status";
    http[ "208" ] = "Already Reported";

    http[ "300" ] = "Multiple Choices";
    http[ "301" ] = "Moved Permanently";
    http[ "302" ] = "Found";
    http[ "303" ] = "See Other";
    http[ "304" ] = "Not Modified";
    http[ "305" ] = "Use Proxy";
    http[ "306" ] = "Switch Proxy";
    http[ "307" ] = "Temporary Redirect";
    http[ "308" ] = "Permanent Redirect";

    http[ "400" ] = "Bad Request";
    http[ "401" ] = "Unauthorized";
    http[ "402" ] = "Payment Required";
    http[ "403" ] = "Forbidden";
    http[ "404" ] = "Not Found";
    http[ "405" ] = "Method Not Allowed";
    http[ "406" ] = "Not Acceptable";
    http[ "407" ] = "Proxy Authentication Required";
    http[ "408" ] = "Request Timeout";
    http[ "409" ] = "Conflict";
    http[ "410" ] = "Gone";
    http[ "411" ] = "Length Required";
    http[ "412" ] = "Precondition Failed";
    http[ "413" ] = "Payload Too Large";
    http[ "414" ] = "URI Too Long";
    http[ "415" ] = "Unsupported Media Type";
    http[ "416" ] = "Requested Range Not Satisfiable";
    http[ "417" ] = "Expectation Failed";
    http[ "418" ] = "I'm a teapot";
    http[ "421" ] = "Misdirected Request";
    http[ "422" ] = "Unprocessable Entity";
    http[ "423" ] = "Locked";
    http[ "424" ] = "Failed Dependency";
    http[ "426" ] = "Upgrade Required";
    http[ "428" ] = "Precondition Required";
    http[ "429" ] = "Too Many Request";
    http[ "431" ] = "Request Header Fields Too Large";
    http[ "451" ] = "Unavailable For Legal Reasons";

    http[ "500" ] = "Internal Server Error";
    http[ "501" ] = "Not Implemented";
    http[ "502" ] = "Bad Gateway";
    http[ "503" ] = "Service Unavailable";
    http[ "504" ] = "Gateway Timeout";
    http[ "505" ] = "HTTP Version Not Supported";
    http[ "506" ] = "Variant Also Negotiates";
    http[ "507" ] = "Insufficient Storage";
    http[ "508" ] = "Loop Detected";
    http[ "510" ] = "Not Extended";
    http[ "511" ] = "Network Authentication Required";

    return http;
}