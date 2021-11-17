#include "Request.hpp"

/*
 *	Global map containing all requests of all clients stored as vector<unsigned char>
 *  Map key is the fd of poll_tab for that client 	
 */
std::map<int, std::vector<unsigned char> >	g_request;




Request::Request() {}

Request::~Request() {}

/*	
 *	Copy the portion of the request in the buffer to a vector<unsigned char>
 */
int			Request::read(char buffer[BUFFER_SIZE], struct pollfd *ptr_tab_poll) {

	size_t ret = 0;

	g_request[ptr_tab_poll->fd];

	for (size_t i = 0; buffer[i]; i++) {
		g_request[ptr_tab_poll->fd].push_back(buffer[i]);
		ret++;
	}

	return ret;
}

/*
 *	Once the whole request has been read, it is parsed and transformed into a header map
 *	If the request is not good, we throw the appropiate http error code with the appropiate message.

TODO: Récupérer correctement les arguments. (Raph s'en occupe.)
 */
void		Request::parse(struct pollfd *ptr_tab_poll) {

	// Conversion of vector<unsigned char> to string
	std::string request_str(g_request[ptr_tab_poll->fd].begin(), g_request[ptr_tab_poll->fd].end());
    std::istringstream iss(request_str);
    std::string key;
    std::string val;

	header.clear();
    reponse.clear();
	g_request[ptr_tab_poll->fd].clear();

    header["args"] = "";
    getline(iss, header["method"], ' ');

    /*
       Pour header["url"] : Récupérer les arguments (?say=hi&to=mom) et les séparer du fichier
     */
    getline(iss, header["url"], ' ');
    if (header["url"].find('?') != std::string::npos)
    {
        header["args"] = header["url"].substr(header["url"].find('?') + 1, header["url"].size());
        header["url"] = header["url"].substr(0, header["url"].find('?'));
    }
    getline(iss, header["http"], '\r');
    if (header["method"] == "" || header["url"] == "" || header["http"] == ""
			|| header["http"].find("\n") != header["http"].npos || header["url"][0] != '/')
        return http_code("400");
    if (header["http"] != "HTTP/1.1")
        return http_code("505");

    size_t begin_key;
    size_t end_key;

    val.clear();
    request_str.erase(0, request_str.find('\n') + 1);
    /*
       Loop to put all key and value in header.
     */
    while (request_str.size() > 0)
    {
        //std::cout << "str = [" << request_str << "]\n";

        /*
           When we have a body in the request, the body is separate from the informations thanks to 13 then 10 ascii char.
         */
        if ((request_str.size() > 2) && (request_str.at(0) == 13) && (request_str.at(1) == '\n')) //at(0) == 13; at(1) == '\n' because a new line split the header from the body
        {
            request_str.erase(0, 2); //+ 1 for the '\n'.
            header["body"] = request_str.substr(0, request_str.size());
            //std::cout << "BODY:\n[" << header["body"] << "]\n";
            request_str.erase(0, request_str.size()); //+ 1 for the '\n'.
        }
        /*
           The key and the value are separate by ':'. The function is going to find the separator then fill header with the key and value
         */
        begin_key = request_str.find(':');
        if (begin_key == std::string::npos)
        {
            break ;
        }
        key = request_str.substr(0, begin_key);
        //std::cout << "key = [" << key << "]\nbegin = " << begin_key << "\n";
        /*
           The request_str is the full informations the request receive. So the function erase the traited informations when header is filled.
         */
        request_str.erase(0, begin_key + 2); //+ 1 for the ':' and 2 (1 more) for ' ' next to the ':'.
        end_key = request_str.find('\n');
        //std::cout << "str[" << end_key << "] = [" << request_str.at(end_key) << "]\n";
        if (end_key == std::string::npos)
        {
            break ;
        }
        val = request_str.substr(0, end_key - 1);
        //std::cout << "val = [" << val << "]\n";
        header[key] = val;
        request_str.erase(0, end_key + 1); //+ 1 for the '\n'.
    }

	g_request[ptr_tab_poll->fd].clear(); // empty vector to allow incoming request from the same client


	// --------  affichage  --------------------------------------------------------------------------
	
    for (std::map<std::string, std::string>::iterator it = header.begin(); it != header.end(); ++it) {
        std::cout << it->first << ":" << it->second << std::endl;
    }
	
}

void	print_string_dictionnary(std::map<std::string, std::string> &first)
{
	for (std::map<std::string, std::string>::iterator  itr = first.begin(); itr != first.end(); ++itr) 
	{
        std::cout << '\t' << itr->first << '\t' << itr->second << '\n';
	}
	std::cout << "\n" << std::endl;
}

/*
 * 	If the requested method is supported, we call the appropiate function
 */
void        Request::process()
{
    std::map<std::string, std::string> reponse;
	bool ret = getInfo(3300, "/", &reponse, find_location);
	if (ret)
	{
		std::cout << ">>>>>Exit<<<<" << std::endl;
		print_string_dictionnary(reponse);
	}
	else
	{
		std::cout << "<<< Nooot exit<<<<" << std::endl;
	}

	// Reponse["code"] will only exist if the parsing threw an error. Execution stops then
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

int		Request::sendall(int s, const char *buf, int len)
{
    int total = 0;        // how many bytes we've sent
    int bytesleft = len; // how many we have left to send
    int n;

    while(total < len) {
        n = send(s, buf+total, bytesleft, 0);
        if (n == -1) { break; }
        total += n;
        bytesleft -= n;
    }

    len = total; // return number actually sent here

    return n==-1?-1:0; // return -1 on failure, 0 on success
}

/*
 *	Send the reponse with the proper HTTP headers and format
 */
int        Request::send_reponse(int socket) {

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


	if (sendall(socket, reply.c_str(),reply.length()) < 0)
		return 0;

    write(1, "\nREPONSE:\n\n", 12);
    write(1, reply.c_str(),reply.length());
    write(1, "\n\n", 2);

	return 1;

}

bool is_a_directory(const std::string &s)
{
  	struct stat buffer;
  	return (stat (s.c_str(), &buffer) == 0 && buffer.st_mode & S_IFDIR);
}

void        Request::_process_GET()
{
	std::string	filestr;
    std::string path;
	std::string	root;

	// set root as described in config file
	root = "root/";
	chdir(root.c_str());

    if (header["url"] == "/") {
        path = "index.html";
    } else if (header["url"][0] == '/') {
        path = header["url"].erase(0,1);
	}
//	path = root + path;

    std::ifstream	ifs(path.c_str());

	if (is_a_directory(path) && 1 /* autoindex is on  */) {

		DIR *dir;
		struct dirent *ent;
		if ((dir = opendir (path.c_str())) != NULL) {

			filestr = "<!DOCTYPE html><html><body>";

  			while ((ent = readdir (dir)) != NULL) {

				filestr.append("<a href=\"/" + path + "/");
				filestr.append(ent->d_name);
				filestr.append("\">");
				filestr.append(ent->d_name);
				filestr.append("</a><br>");
  			}
  			closedir (dir);
			filestr.append("</body></html>");

		} else {
			return http_code("401");	
		}
	} else if (ifs.fail()) {
        return http_code("404");
	} else {

        std::stringstream buf;
        buf << ifs.rdbuf();
        filestr = buf.str();

        ifs.close();
	}

    http_code("200");
    reponse["body"] = filestr;
	std::stringstream content_len;
	content_len	<< filestr.length();
    reponse["Content-Length"] = content_len.str();

    reponse["Content-Type"] = "text/plain; charset=utf-8";
    if (is_a_directory(path) || path.substr(path.find_last_of(".") + 1) == "html")
        reponse["Content-Type"] = "text/html; charset=utf-8";
}

/*
   Cette fonction va permettre de déterminer quel type de fichier la requete POST
   va nous demander de créer.
   Elle va initialiser une map et relier chaque valeur de Content-type (valeur à
   droite) avec le type de fichier à créer (valeur à gauche).
   Le nom MIME vient des différents Content-type qui existe. La liste est non
   exclusive
 */
void	initialize_mime_types(std::map<std::string, std::string> &mime_types)
{
	mime_types[".aac"]      = "audio/aac";
	mime_types[".abw"]      = "application/x-abiword";
	mime_types[".arc"]      = "application/octet-stream";
	mime_types[".avi"]      = "video/x-msvideo";
	mime_types[".azw"]      = "application/vnd.amazon.ebook";
	mime_types[".bin"]      = "application/octet-stream";
	mime_types[".bz"]       = "application/x-bzip";
	mime_types[".bz2"]      = "application/x-bzip2";
	mime_types[".csh"]      = "application/x-csh";
	mime_types[".css"]      = "text/css";
	mime_types[".csv"]      = "text/csv";
	mime_types[".doc"]      = "application/msword";
	mime_types[".epub"]     = "application/epub+zip";
	mime_types[".gif"]      = "image/gif";
	mime_types[".htm"]      = "text/html";
	mime_types[".html"]     = "text/html";
	mime_types[".ico"]      = "image/x-icon";
	mime_types[".ics"]      = "text/calendar";
	mime_types[".jar"]      = "Temporary Redirect";
	mime_types[".jpeg"]     = "image/jpeg";
	mime_types[".jpg"]      = "image/jpeg";
	mime_types[".js"]       = "application/js";
	mime_types[".json"]     = "application/json";
	mime_types[".mid"]      = "audio/midi";
	mime_types[".midi"]     = "audio/midi";
	mime_types[".mpeg"]     = "video/mpeg";
	mime_types[".mpkg"]     = "application/vnd.apple.installer+xml";
	mime_types[".odp"]      = "application/vnd.oasis.opendocument.presentation";
	mime_types[".ods"]      = "application/vnd.oasis.opendocument.spreadsheet";
	mime_types[".odt"]      = "application/vnd.oasis.opendocument.text";
	mime_types[".oga"]      = "audio/ogg";
	mime_types[".ogv"]      = "video/ogg";
	mime_types[".ogx"]      = "application/ogg";
	mime_types[".png"]      = "image/png";
	mime_types[".pdf"]      = "application/pdf";
	mime_types[".ppt"]      = "application/vnd.ms-powerpoint";
	mime_types[".rar"]      = "application/x-rar-compressed";
	mime_types[".rtf"]      = "application/rtf";
	mime_types[".sh"]       = "application/x-sh";
	mime_types[".svg"]      = "image/svg+xml";
	mime_types[".swf"]      = "application/x-shockwave-flash";
	mime_types[".tar"]      = "application/x-tar";
	mime_types[".tif"]      = "image/tiff";
	mime_types[".tiff"]     = "image/tiff";
	mime_types[".ttf"]      = "application/x-font-ttf";
	mime_types[".txt"]      = "text/plain";
	mime_types[".vsd"]      = "application/vnd.visio";
	mime_types[".wav"]      = "audio/x-wav";
	mime_types[".weba"]     = "audio/webm";
	mime_types[".webm"]     = "video/webm";
	mime_types[".webp"]     = "image/webp";
	mime_types[".woff"]     = "application/x-font-woff";
	mime_types[".xhtml"]    = "application/xhtml+xml";
	mime_types[".xls"]      = "application/vnd.ms-excel";
	mime_types[".xml"]      = "application/xml";
	mime_types[".xul"]      = "application/vnd.mozilla.xul+xml";
	mime_types[".zip"]      = "application/zip";
	mime_types[".3gp"]      = "video/3gpp audio/3gpp";
	mime_types[".3g2"]      = "video/3gpp2 audio/3gpp2";
	mime_types[".7z"]       = "application/x-7z-compressed";
}

/*
   Méthode pour créer un fichier et le remplir du body présent dans header[].
   On lui envoie le type de fichier en argument (ex : .json)
 */
int    Request::create_file(std::string const file_type)
{
    // Le fichier s'appelle actuellement "test"...
    std::string const nomFichier("test" + file_type);
    std::ofstream monFlux(nomFichier.c_str());

    if(monFlux)
    {
        monFlux << header["body"];
    }
    else
    {
        std::cout << "ERREUR: Impossible d'ouvrir le fichier." << std::endl;
        return (FAILURE);
    }
    return (SUCCESS);
}

/*
   Parsing (+OK) => body = le content que je dois mettre dans un fichier
   Je suis en train de créer le fichier pour mettre le body dedans.
   Etape 1 : récupérer l'index du ficher (ex : application/json => need .json) cf la fct init_mimes_type.
   Etape 2 : Crée le fichier (cf la méthode create_file).
   Etape 3 : Mettre le body dans le ficher.
   Etape 4 : Renvoyez au client le fait que la requete a été validé !
 */
void    Request::_process_POST()
{
    std::map<std::string, std::string> mime_types;
    std::map<std::string, std::string>::const_iterator it;

    initialize_mime_types(mime_types);
    for (it = mime_types.begin(); it != mime_types.end(); ++it)
    {
        //std::cout << it->second << "\n";
        if (it->second == header["Content-Type"])
            break ;
    }
    // Si on trouve pas le type en question
    if (it == mime_types.end())
    {
        reponse["code"] = "415";
        reponse["status"] = "KO";
    }

    std::cout << "CONTENT TYPE FROM MIME TYPES = [" << it->first << "]\n";

    reponse["Content-Length"]   = header["Content-Length"];
    reponse["Content-Type"]     = header["Content-Type"];
    if (create_file(it->first) != SUCCESS)
    {
        reponse["code"] = "400";
        reponse["status"] = "KO";
    }
    else
    {
        reponse["code"] = "200";
        reponse["status"] = "OK";
    }
    /*
       Il faut voir les valeurs qu'on a envie de renvoyer. En voici certaines d'entre elle qui
       vont surtout nous servir pour intégrer les CGI.
       reponse["arg"] = "";
       reponse["data"] = "";
       reponse["files"] = "";
       reponse["form"] = "";
     */
}

/*
   Delete request
 */
void    Request::_process_DELETE()
{
    char const *file_to_delete = header["url"].c_str();

    //std::cout << "file to delete = (" << file_to_delete << ")\n";
    if (header["url"].compare("/") == 0)
    {
        reponse["code"] = "204";
        reponse["status"] = "No Content";
        return ;
    }
    if (remove(file_to_delete) != 0)
    {
        reponse["code"] = "204";
        reponse["status"] = "No Content";
        return ;
    }
    reponse["code"] = "200";
    reponse["status"] = "OK";
}

/*
 * 	Checks if we had already reached the end of the request ( \r\n\r\n )
 */
bool	Request::end_reached(struct pollfd *ptr_tab_poll) {

	size_t len = g_request[ptr_tab_poll->fd].size();

	for (size_t i = 0; i < len; i++) {

		if (g_request[ptr_tab_poll->fd][i] == '\r'
				&& (++i < len && g_request[ptr_tab_poll->fd][i] == '\n')
				&& (++i < len && g_request[ptr_tab_poll->fd][i] == '\r')
				&& (++i < len && g_request[ptr_tab_poll->fd][i] == '\n'))
			return true;
	}
	return false;

}

/*
 *	Takes an HTTP code in string format and assigns it to the 
 *	reponse, also retrieving the proper HTTP status
 */
void	Request::http_code(std::string http_code) {

	int					int_code;
	std::istringstream(http_code) >> int_code;

    std::map<std::string, std::string> http = http_table();

	if (int_code > 400 && int_code < 405) {
		header["url"] = "/error_pages/error_page_" + http_code + ".html";
		_process_GET();
	}
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
