// Server side C program to demonstrate Socket programming
#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>


#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>


#define PORT 8080
//#define PORT 28000

int     get_listener_socket(void) {

    int server_fd;
    int yes = 1;

    struct sockaddr_in address;
    //int addrlen = sizeof(address);

    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("In socket");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr("0.0.0.0");//INADDR_ANY;
    address.sin_port = htons( PORT );
    
    memset(address.sin_zero, '\0', sizeof address.sin_zero);

	 if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &yes,
                sizeof(int)) == -1) {
            perror("setsockopt");
            exit(1);
        }

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address))<0)
    {
        perror("In bind");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 10) < 0)
    {
        perror("In listen");
        exit(EXIT_FAILURE);
    }

    return server_fd;
    
}


void      parse_request(std::string r, std::map<std::string, std::string> & request) {

    std::istringstream iss(r);
    std::string key;
    std::string val;

    iss >> val;
    request["method"] = val;
    iss >> val;
    request["url"] = val;
    iss >> val;
    request["http"] = val;
    while(iss >> key) {
        iss >> val;
        key.erase(std::remove(key.begin(), key.end(), ':'), key.end());
        request[key] = val;
    }


}

void        process_request(std::map<std::string, std::string> request, std::map<std::string, std::string> & reponse) {

    reponse.clear();

    if (request["method"] != "GET") {
        reponse["code"] = "405";
        reponse["status"] = "Method Not Allowed";
        return ;
    }
        
    char buffer[256];
    getwd(buffer);
    std::string path(buffer);
    
    if (request["url"] == "/")
        request["url"] = "/index.html";

    std::ifstream	ifs(path + request["url"]);
	if (ifs.fail()) {
		//perror("Could not open the file");
        reponse["code"] = "404";
        reponse["status"] = "File Not Found";
    } else {

	    std::stringstream buf;
	    buf << ifs.rdbuf();
	    std::string	filestr = buf.str();

	    ifs.close();

        reponse["code"] = "200";
        reponse["status"] = "OK";
        reponse["body"] = filestr;
        reponse["Content-Length"] = std::to_string(filestr.length());
    
        reponse["Content-Type"] = "text/plain; charset=utf-8";
        if(request["url"].substr(request["url"].find_last_of(".") + 1) == "html")
            reponse["Content-Type"] = "text/html; charset=utf-8";

    }
}
void        send_reponse(int socket, std::map<std::string, std::string> & reponse) {

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

int main(int argc, char const *argv[])
{
    int server_fd, new_socket; long valread;
    struct sockaddr_in address;
    socklen_t   addrlen;
    std::map<std::string, std::string> request;
    std::map<std::string, std::string> reponse;
    
	//const char *http_ok_header = "HTTP/1.1 200 OK\nContent-Type: text/html; charset=UTF-8\nContent-Length: ";//12\n\nHello world!";
	//const char *http_ok_header = "HTTP/1.1 405 Not Found\n";//\nContent-Type: text/html; charset=UTF-8\nContent-Length: ";//12\n\nHello world!";

    server_fd = get_listener_socket();
    
    while(1)
    {
//        printf("\n+++++++ Waiting for new connection ++++++++\n\n");
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0)
        {
            perror("In accept");
            exit(EXIT_FAILURE);
        }
        
        char buffer[30000] = {0};
        valread = read( new_socket , buffer, 30000);
        printf("%s\n",buffer );



        parse_request(std::string(buffer), request); 

        process_request(request, reponse);

        send_reponse(new_socket, reponse);
        
        
        
/*  server kk
 *
 *
        std::ifstream	ifs("index.html");
	    if (ifs.fail()) 
		    perror("Could not open the file");

	    std::stringstream buf;
	    buf << ifs.rdbuf();
	    std::string	filestr = buf.str();

	    ifs.close();



		const char *len = std::to_string(filestr.length()).c_str();
		const char *newline = "\n\n";

        write(new_socket , http_ok_header, strlen(http_ok_header));
		write(new_socket, len, strlen(len)); 
		write(new_socket, newline, strlen(newline));
		write(new_socket, filestr.c_str(), filestr.length());

*/

//        printf("------------------Hello message sent-------------------\n");
        close(new_socket);
    }
    return 0;
}
