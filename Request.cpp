#include "Request.hpp"

Request::Request() {}

Request::~Request() {}

void      Request::parse(std::string request_str) {

    std::istringstream iss(request_str);
    std::string key;
    std::string val;

    iss >> val;
    header["method"] = val;
    iss >> val;
    header["url"] = val;
    iss >> val;
    header["http"] = val;
    while(iss >> key) {
        iss >> val;
        key.erase(std::remove(key.begin(), key.end(), ':'), key.end());
        header[key] = val;
    }

    if (header["url"][0] != '/')
        std::cout << "url error\n";
    if (header["http"] != "HTTP/1.1")
        std::cout << "Error 505 HTTP Version Not Supported" << std::endl;

    (void)reponse;

}

void        Request::process() {

    reponse.clear();

    if (header["method"] == "GET") 
        _process_GET();
    else if (header["method"] == "POST")
        _process_POST();
    else if (header["method"] =="DELETE")
    	_process_DELETE();
    else {
        reponse["code"] = "405";
        reponse["status"] = "Method Not Allowed";
    }

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
    {
        //perror("Could not open the file");
        reponse["code"] = "404";
        reponse["status"] = "File Not Found";
    } 
    else 
    {

        std::stringstream buf;
		std::stringstream content_len;
        buf << ifs.rdbuf();
        std::string	filestr = buf.str();

        ifs.close();

        reponse["code"] = "200";
        reponse["status"] = "OK";
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
