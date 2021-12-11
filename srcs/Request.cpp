#include "Request.hpp"

/*
 *	Global map containing all requests of all clients stored as vector<unsigned char>
 *  Map key is the fd of poll_tab for that client 	
 */
std::map<int, std::vector<unsigned char> >	g_request;
std::map<int, std::vector<unsigned char> >	g_reponse;


Request::Request() {}

Request::~Request() {}

/*	
 *	Copy the portion of the request in the buffer to a vector<unsigned char>
 */
int			Request::store(char buffer[BUFFER_SIZE], struct pollfd *ptr_tab_poll, int bytes_to_read) {

	size_t ret = 0;

	g_request[ptr_tab_poll->fd];

	for (int i = 0; i < bytes_to_read; i++) {
		g_request[ptr_tab_poll->fd].push_back(buffer[i]);
		ret++;
	}

	return ret;
}

bool is_hex(std::string const& s)
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

/*
 *	Once the whole request has been read, it is parsed and transformed into a header map
 *	If the request is not good, we throw the appropiate http error code with the appropiate message.
 */
void		Request::parse(struct pollfd *ptr_tab_poll, int port)
{

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

    header["args"] = "";

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
    if (header["method"] == "" || header["url"] == "" || header["http"] == "" || header["url"][0] != '/'
			|| header["http"].find("\n") != std::string::npos || header["http"].find(" ") != std::string::npos)
        return http_code("400");
    if (header["http"] != "HTTP/1.1") {
        return http_code("505");
	}



	/*
 	 * check that path is valid (does not go beyond root)
 	 */
	std::string s = header["url"];
	int	pathcount = 0;

	s.erase(0, 1);

	size_t pos = 0;
	std::string token;
	while ((pos = s.find("/")) != std::string::npos) {
    	token = s.substr(0, pos);

		pathcount = token == ".." ? --pathcount : ++pathcount;

		if (pathcount < 0)
			return http_code("400");

    	s.erase(0, pos + 1);
	}


    std::stringstream ss;
    ss << port;  
    ss >> header["port"];

    size_t begin_key;
    size_t end_key;
    
    std::string bound = "";
    
    val.clear();
    request_str.erase(0, request_str.find('\n') + 1);
    /*
       Loop to put all key and value in header.
       */
    while (request_str.size() > 0)
    {
        /*
           When we have a body in the request, the body is separate from the informations thanks to 13 then 10 ascii char.
        */
        if ((request_str.size() > 2) && (request_str.at(0) == 13) && (request_str.at(1) == '\n')) //at(0) == 13; at(1) == '\n' because a new line split the header from the body
        {
			request_str.erase(0, 2); // erase the 13 char ascii then the '\n'.
            /*
                In case where the client is uploading something, there can have a big '-' line, that is not the body.
            */
            if (strncmp(request_str.c_str(), "-----------------------------", 28) == 0)
            {
                if (header["CONTENT-TYPE"].size() > 0)
                {
                    std::size_t found = header["CONTENT-TYPE"].find("boundary=");
                    if (found != std::string::npos)
                    {
                        bound = header["CONTENT-TYPE"].substr(header["CONTENT-TYPE"].find("boundary=") + 9);
                    }
                }
                if (request_str.find(bound) < 3)
                {
                    begin_key = request_str.find('\n');
                    request_str.erase(0, begin_key + 1); //+ 1 for the '\n'.
                }
            }
			else if (header.find("TRANSFER-ENCODING") != header.end() && header["TRANSFER-ENCODING"] == "chunked")
            {

				std::string			to_read;
				size_t				bytes;

				while (request_str.size() > 0) {

					/*if (request_str.compare(0, 2, "0x") == 0)
					  request_str.erase(0, 2);*/
					to_read = request_str.substr(0, request_str.find("\r"));
					if (to_read == "") {
						request_str.erase(0, 2);
						continue;
					}
					if (!is_hex(to_read))
						return http_code("400");

					std::istringstream(to_read) >> std::hex >> bytes;

					if (request_str.at(request_str.find("\r") + 1) != '\n')
						return http_code("401");
					request_str.erase(0, request_str.find("\n") + 1);

					header["body"] += request_str.substr(0, bytes);

					try { if (request_str.at(bytes) != '\r' || request_str.at(bytes + 1) != '\n')
						return http_code("402");
					} catch (std::out_of_range) { break; }

					request_str.erase(0, bytes + 2);
				}

			}
            else
            {
                if (request_str.find(bound) != std::string::npos)
                {
                    begin_key = (int)request_str.find(bound);
                    if (begin_key >= 5)
                    	header["body"] = request_str.erase(begin_key - 5); // -5 because there is those 5 char "'\13''\n''-''-''\n'"
                    else if (begin_key >= 2)
                    	header["body"] = request_str.erase(begin_key - 2); // -3 because there is those 2 char "'\13''\n'"
                    request_str.erase(0, request_str.size()); //+ 1 for the '\n'.
                }
                else
                {
                	header["body"] = request_str.substr(0, request_str.size());
                	request_str.erase(0, request_str.size()); //+ 1 for the '\n'.
                }
			}
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
        /*
            DEBUG: What is that ?
        */
		std::transform(key.begin(), key.end(), key.begin(), ::toupper);

        /*
           The request_str is the full informations the request receive. So the function erase the traited informations when header is filled.
        */
        request_str.erase(0, ft_skip_space(request_str, begin_key + 1, 0)); //+ 1 for the ':'. ft_skip_space will skip OWS space.
        end_key = request_str.find('\n');
        if (end_key == std::string::npos)
        {
            break ;
        }
        val = request_str.substr(0, ft_skip_space(request_str, end_key - 1, 1)); // -1 for '\n'. ft_skip_space will skip OWS space
        header[key] = val;
        request_str.erase(0, end_key + 1); //+ 1 for the '\n'.
    }

	g_request[ptr_tab_poll->fd].clear(); // empty vector to allow incoming request from the same client

	// --------  affichage  --------------------------------------------------------------------------
    /*
   	std::cout << "Display header parsed begin" << std::endl;
    for (std::map<std::string, std::string>::iterator it = header.begin(); it != header.end(); ++it)
    {
       	std::cout << it->first << ":" << it->second << std::endl;
    }
       	std::cout << "\nDisplay header parsed end" << std::endl;
    */
}

std::string        Request::get_method()
{
    return (header["method"]);
}

std::string        Request::get_body()
{
    return (header["body"]);
}

std::string        Request::get_path()
{
    return (header["url"]);
}

std::string        Request::get_host()
{
    return (header["HOST"]);
}

std::string        Request::get_accept()
{
    return (header["ACCEPT"]);
}

std::string        Request::get_accept_encoding()
{
    return (header["ACCEPT-ENCODING"]);
}

std::string        Request::get_accept_language()
{
    return (header["ACCEPT-LANGUAGE"]);
}
std::string        Request::get_path_info()
{
    bool ret;
    std::string get_info = return_config_info("cgi_pass");

    std::map<std::string, std::string> location_rep;
	ret = getInfo(atoi(header["port"].c_str()), "*.php", &location_rep, find_location);
	if (ret)
	{
        /*
            If there is some information at a location from the url, search if there is a /root informations in the config file
        */
        std::map<std::string, std::string>::const_iterator it;
        for (it = location_rep.begin(); it != location_rep.end(); ++it)
        {
            if (it->first.compare("cgi_pass") == 0)
            {
                get_info = it->second;
            }
        }
	}
    return (get_info);
}

std::string        Request::get_content_type()
{
    return (header["CONTENT-TYPE"]);
}

std::string        Request::get_content_length()
{
    return (header["CONTENT-LENGTH"]);
}

/*
 * 	If the requested method is supported, we call the appropiate function
 */
void        Request::process()
{
    std::string rep;

	// Reponse["code"] will only exist if the parsing threw an error. Execution stops then
    if (reponse.find("code") != reponse.end()) {
        return ;
	}

	rep = return_config_info("allow");
    if (header["method"] == "GET" && rep.find("GET") != std::string::npos) {
            _process_GET();
    }
    else if (header["method"] == "POST" && rep.find("POST") !=std::string::npos) {
            _process_POST();
    }
    else if (header["method"] =="DELETE" && rep.find("DELETE") !=std::string::npos) {
            _process_DELETE();
    }
    else
        http_code("405");
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

/*
 *	Send the reponse with the proper HTTP headers and format
 */
void		Request::compose_reponse(struct pollfd *ptr_tab_poll)
{

    reponse["http_version"] = "HTTP/1.1";

	bool content_type = false;

    std::string reply = reponse["http_version"] + " " + reponse["code"] + " " + reponse["status"] + "\n";
	
	if (reponse.find("LOCATION") != reponse.end())
		reply.append("Location: " + reponse["LOCATION"] + "\n");

	reply.append("Date: " + time_to_string() + " \n");
	reply.append("Server: Webserv/1.0 (Unix)\n");

    if (reponse.find("body") == reponse.end()) {

		reply.append("Connection: Closed\n");
        reply.append("\n");
        reply.append("\n");

    	write(1, "\nREPONSE:\n\n", 11);
	    write(1, reply.c_str(),reply.length());

	} else {

		for (std::map<std::string, std::string>::iterator it = cgi_head.begin(); it != cgi_head.end(); it++) {

			reply.append(it->first + ": " + it->second + "\n");
			std::string str = it->first;
			std::transform(str.begin(), str.end(), str.begin(), ::toupper);
			if (str == "CONTENT-TYPE")
				content_type = true;
		}
		if (!content_type)
	        reply.append("Content-Type: " + reponse["CONTENT-TYPE"] + "\n");
        reply.append("Content-Length: " + reponse["CONTENT-LENGTH"] + "\n");

		reply.append("Connection: Closed\n");
        reply.append("\n");

    	write(1, "\nREPONSE:\n\n", 11);
	    write(1, reply.c_str(),reply.length());

        reply.append(reponse["body"]);
    }

	g_reponse[ptr_tab_poll->fd];
	g_reponse[ptr_tab_poll->fd].clear();

	for (size_t i = 0; i < reply.length(); i++) {
		g_reponse[ptr_tab_poll->fd].push_back(reply[i]);
	}
	ptr_tab_poll->events = POLLOUT;
}

int			Request::send_reponse(struct pollfd *ptr_tab_poll) {

	int ret;
	int	fd = ptr_tab_poll->fd;
	size_t	rsize = g_reponse[fd].size();

	size_t i;
	for (i = 0; i < rsize && i < BUFFER_SIZE; i++) {

		buf[i] = g_reponse[fd][i];

	}	
	buf[i] = '\0';

	ret = send(fd, buf, (BUFFER_SIZE < rsize ? BUFFER_SIZE : rsize), 0);

	if (ret < 0)
		return ret;
	else
		std::cout << "send() successfully sent " << ret << " bytes, " << rsize - ret << " left" << std::endl;

	g_reponse[fd].erase(g_reponse[fd].begin(), g_reponse[fd].begin() + ret);

	if (g_reponse[fd].size() == 0)
		ptr_tab_poll->events = POLLIN;

	return ret;
}

std::string     Request::return_config_info(std::string searching_index)
{
   	std::map<std::string, std::string>	location_rep;
    std::string 						search_rep;
	std::string 						location;
	int									port = atoi(header["port"].c_str());
    
    //		Search_rep will be initialised with the directive value for the searching_index
    bool ret1 = getInfo(port, searching_index, &search_rep, find_directive);
    //		Search if the searching_index is in the config file
	bool ret2 = get_location_url(port, header["url"], &location_rep);
	if (ret2)
	{
        //		Search if the searching_index is inside a location
        std::map<std::string, std::string>::const_iterator it;
        for (it = location_rep.begin(); it != location_rep.end(); ++it)
        {
            if (it->first == searching_index)
            {
                search_rep = it->second;
            }
        }
	}
    return ((ret1 | ret2) ? search_rep : "");
}

// bool is_a_directory(const std::string &s)
// {
//   	struct stat buffer;
//   	return (stat (s.c_str(), &buffer) == 0 && buffer.st_mode & S_IFDIR); // if exist && is a directory return 1
// }

// bool exists(const std::string &s)
// {
// 	struct stat buffer;
// 	return (stat (s.c_str(), &buffer) == 0);
// }

void        Request::_process_GET() {
    std::map<std::string, std::string> mime_types;
    std::map<std::string, std::string>::const_iterator it;
    initialize_mime_types(mime_types);

	std::string	filestr;
    std::string path;
	std::string index_path;
	std::string	root = return_config_info("root");
	std::string redir = return_config_info("return");

	if (redir != "") {

		size_t pos = redir.find(" ");
		std::string code = redir.substr(0, pos);
		std::string url = redir.substr(pos + 1, std::string::npos);

    	reponse["LOCATION"] = url;
		std::stringstream content_len;
		content_len	<< url.length();
    	reponse["CONTENT-LENGTH"] = content_len.str();
	    reponse["CONTENT-TYPE"] = "text/html; charset=utf-8";
    	return http_code(code);
	}

	chdir(root.c_str());

    int	auto_index = 0;
    std::string rep = return_config_info("autoindex");
    if (rep.compare("on") == 0) {
        auto_index = 1;
	}

	path = header["url"];
	if (path[0] == '/') {
        path = path.erase(0,1);
	}
	if (path == "" || is_a_directory(path)) {
		index_path = path;
		if (is_a_directory(path))
			index_path += "/";
		index_path += return_config_info("index");

	}

	if (exists(index_path) && !is_a_directory(index_path))
		path = index_path;
	if (path == "")
		path = ".";

    std::ifstream	ifs(path.c_str());

/*
    TODO: Faire une variable pour récupérer l'auto index s il est présent.
*/

	if (is_a_directory(path) && auto_index == 1) {

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
	}
	else if (path.substr(path.find_last_of(".") + 1) == "php") {

		Cgi	c(path, this,  cgi_head);

		filestr = c.get_data();
	
	}
    else
    {

        std::stringstream buf;
        buf << ifs.rdbuf();
        filestr = buf.str();

        ifs.close();
	}

    http_code("200");
    reponse["body"] = filestr;
	std::stringstream content_len;
	content_len	<< filestr.length();
    reponse["CONTENT-LENGTH"] = content_len.str();

    reponse["CONTENT-TYPE"] = "text/plain; charset=utf-8";
    if (is_a_directory(path))// || path.substr(path.find_last_of(".") + 1) == "html")
        reponse["CONTENT-TYPE"] = "text/html; charset=utf-8";
	else
	{
		for (it = mime_types.begin(); it != mime_types.end(); ++it)
    	{
			std::string extension("");
			size_t pos = path.find_last_of(".");
			if (pos != std::string::npos)
				extension = path.substr(pos);
        	if (it->first == extension)
				reponse["CONTENT-TYPE"] = it->second;
    	}

	}
}

/*
   Cette fonction va permettre de déterminer quel type de fichier la requete POST
   va nous demander de créer.
   Elle va initialiser une map et relier chaque valeur de CONTENT-TYPE (valeur à
   droite) avec le type de fichier à créer (valeur à gauche).
   Le nom MIME vient des différents CONTENT-TYPE qui existe. La liste est non
   exclusive
   */
void		Request::initialize_mime_types(std::map<std::string, std::string> &mime_types)
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

std::string Request::find_url_and_name_from_file(std::string const file_type)
{
    /*
        Search if there is a /root in the config file to initialise the url_file and know where the server have to create the file.
    */
    std::string url_file = return_config_info("root");
    std::string file_name = header["url"];

    if (header["method"] != "DELETE")
    {
        url_file += file_name + "/";
        file_name = "newfile" + file_type;
    }

    /*
        Starting by check if the file the webserv server have to create have a name.
    */
    if (header["CONTENT-DISPOSITION"].size() > 0)
    {
        file_name = header["CONTENT-DISPOSITION"].substr(header["CONTENT-DISPOSITION"].find("filename=") + 10);
        file_name.erase(file_name.size() - 1, file_name.size());
    }

    return (url_file + file_name);
}

/*
   Méthode pour créer un fichier et le remplir du body présent dans header[].
   On lui envoie le type de fichier en argument (ex : .json)
 */
int    Request::create_file(std::string const file_type)
{
    std::string const nomFichier(find_url_and_name_from_file(file_type));
    std::ofstream monFlux(nomFichier.c_str());

    if(monFlux)
    {
        monFlux << header["body"];
    }
    else
    {
        /*
            DEBUG : (TODO: delete the ERREUR msg)
        */
        return (FAILURE);
    }
    return (SUCCESS);
}

/*
   Parsing (+OK) => body = le content que je dois mettre dans un fichier
   Etape 1 : récupérer l'index du ficher (ex : application/json => need .json) cf la fct init_mimes_type.
   Etape 2 : Crée le fichier (cf la méthode create_file).
   Etape 3 : Mettre le body dans le ficher.
   Etape 4 : Renvoyez au client le fait que la requete a été validé !

 */
void    Request::_process_POST()
{
    std::string size_max = return_config_info("cli_max_size");
    if (size_max.size() < header["CONTENT-LENGTH"].size())
    {
        return http_code("411");
    }
    else if (size_max.size() == header["CONTENT-LENGTH"].size())
    {
        for (int i = 0; i < (int)size_max.size(); i++)
        {
            if (size_max.at(i) > header["CONTENT-LENGTH"].at(i))
                break ;
            else if (size_max.at(i) < header["CONTENT-LENGTH"].at(i))
            {
                return http_code("411");
            }
        }
    }

    std::map<std::string, std::string> mime_types;
    std::map<std::string, std::string>::const_iterator it;

    initialize_mime_types(mime_types);
    for (it = mime_types.begin(); it != mime_types.end(); ++it)
    {
        if (it->second == header["CONTENT-TYPE"])
            break ;
    }
    // Si on trouve pas le type en question
    if ((it == mime_types.end()) && (header["CONTENT-TYPE"] != "application/x-www-form-urlencoded"))
    {
        return http_code("415");
    }

    reponse["CONTENT-LENGTH"]   = header["CONTENT-LENGTH"];
    reponse["CONTENT-TYPE"]     = header["CONTENT-TYPE"];
    if (header["url"].substr(header["url"].find_last_of(".") + 1) == "php")
    {
		std::string path(header["url"]);
		path = path.erase(0, 1);
		Cgi	c(path, this, cgi_head);

		http_code("200");
		reponse["body"] = c.get_data();
		std::stringstream len;
		len << reponse["body"].length();
		reponse["CONTENT-LENGTH"] = len.str();
    	reponse["CONTENT-TYPE"] = "text/plain; charset=utf-8";
	
	}
    else if (create_file(it->first) != SUCCESS)
    {
        /*
            TODO : Afficher la page error avec le bon numéro
        */
        return http_code("400");
    }
    else
    {
        return http_code("200");
    }
}

/*
   Delete request
 */
void    Request::_process_DELETE()
{
    std::string const nomFichier(find_url_and_name_from_file(""));
    char const *file_to_delete = nomFichier.c_str();

    if (header["url"].compare("/") == 0)
    {
        return http_code("204");
    }
    if (remove(file_to_delete) != 0)
    {
        return http_code("204");
    }
    return http_code("200");
}

// templated version of my_equal so it could work with both char and wchar_t
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

/*
 * 	Checks if we had already reached the end of the request ( \r\n\r\n )
 */
bool	Request::end_reached(struct pollfd *ptr_tab_poll) {

	size_t len = g_request[ptr_tab_poll->fd].size();
	std::string request_str(g_request[ptr_tab_poll->fd].begin(), g_request[ptr_tab_poll->fd].end());

	if (ci_find_substr(request_str, "transfer-encoding") != -1 && request_str.find("chunked") != std::string::npos)
		/*	if (request_str.find("TRANSFER-ENCODING") != std::string::npos
			&& request_str.find("CHUNKED") != std::string::npos) */{

				for (size_t i = 0; i < len; i++) {

					if (g_request[ptr_tab_poll->fd][i] == '\r'
							&& (++i < len && g_request[ptr_tab_poll->fd][i] == '\n')
							&& (++i < len && g_request[ptr_tab_poll->fd][i] == '0')
							&& (++i < len && g_request[ptr_tab_poll->fd][i] == '\r')
							&& (++i < len && g_request[ptr_tab_poll->fd][i] == '\n')
							&& (++i < len && g_request[ptr_tab_poll->fd][i] == '\r')
							&& (++i < len && g_request[ptr_tab_poll->fd][i] == '\n'))
						return true;
				}
			} else {

				for (size_t i = 0; i < len; i++) {

					if (g_request[ptr_tab_poll->fd][i] == '\r'
							&& (++i < len && g_request[ptr_tab_poll->fd][i] == '\n')
							&& (++i < len && g_request[ptr_tab_poll->fd][i] == '\r')
							&& (++i < len && g_request[ptr_tab_poll->fd][i] == '\n'))
						return true;
				}
			}
	return false;

}

/*
 *	Takes an HTTP code in string format and assigns it to the 
 *	reponse, also retrieving the proper HTTP status
 */
void	Request::http_code(std::string http_code)
{
	int					int_code;
	std::istringstream(http_code) >> int_code;
    std::map<std::string, std::string> http = http_table();
	std::ostringstream	s;

	if (int_code == 404)
    {
		header["url"] = "/error_page/error_page_" + http_code + ".html";
		_process_GET();
	}
	else if (int_code >= 400) {
		reponse["body"] = "<h1>" + http_code + " " +  http[http_code] + "</h1>";
		s << reponse["body"].length();
		reponse["CONTENT-LENGTH"] = std::string(s.str());
        reponse["CONTENT-TYPE"] = "text/html; charset=utf-8";
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
