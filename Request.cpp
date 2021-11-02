#include "Request.hpp"

#include <strings.h>

Request::Request() {}

Request::~Request() {}

/*
    Fonction pour parser la requete du client (browser).

    Elle n'a pas presque pas évoluer donc je pense qu'il y a encore des pbs
    mais le body est récupérer correctement.
*/
void      Request::parse(std::string request_str)
{
    std::istringstream iss(request_str);
    std::string key;
    std::string val;

    //std::cout << "REQUEST STR \n|" << request_str << "|\n";
    iss >> val;
    header["method"] = val;
    iss >> val;
    header["url"] = val;
    iss >> val;
    header["http"] = val;

    size_t begin_key;
    size_t end_key;

    val.clear();
    request_str.erase(0, request_str.find('\n') + 1);
    while (request_str.size() > 0)
    {
        std::cout << "str = [" << request_str << "]\n";

        if ((request_str.size() > 2) && (request_str.at(0) == 13) && (request_str.at(1) == '\n') && (request_str.at(2) == '{')) //at(0) == 13; at(1) == '\n' at(2) == '{' because a new line split the header from the body
        {
            request_str.erase(0, 2); //+ 1 for the '\n'.
            begin_key = request_str.find('}');
            if (begin_key == std::string::npos)
            {
                break ;
            }
            header["body"] = request_str.substr(0, begin_key + 1);
            std::cout << "BODY:\n[" << header["body"] << "]\n";
            request_str.erase(0, begin_key + 1); //+ 1 for the '\n'.
        }
        begin_key = request_str.find(':');
        if (begin_key == std::string::npos)
        {
            break ;
        }
        key = request_str.substr(0, begin_key);
        std::cout << "key = [" << key << "]\nbegin = " << begin_key << "\n";
        request_str.erase(0, begin_key + 2); //+ 1 for the ':' and 2 (1 more) for ' ' next to the ':'.
        end_key = request_str.find('\n');
        //std::cout << "str[" << end_key << "] = [" << request_str.at(end_key) << "]\n";
        if (end_key == std::string::npos)
        {
            break ;
        }
        val = request_str.substr(0, end_key - 1);
        std::cout << "val = [" << val << "]\n";
        header[key] = val;
        request_str.erase(0, end_key + 1); //+ 1 for the '\n'.
    }

    if (header["url"][0] != '/')
        std::cout << "url error\n";
    if (header["http"] != "HTTP/1.1")
        std::cout << "Error 505 HTTP Version Not Supported" << std::endl;


    // Just a display to check header value
    std::map<std::string, std::string>::const_iterator it;
    int i = 0;
    for (it = header.begin(); it != header.end(); ++it)
    {
        std::cout << i << ":\n[" << it->first << "]\n>" << it->second << "<\n";
        i++;
    }
    (void)reponse;

}

/*
    En fonction des différentes méthodes, on envoie à la fonction censé traiter
    ce type de requete.
*/
void        Request::process() {

    reponse.clear();

    if (header["method"] == "GET") 
        _process_GET();
    else if (header["method"] == "POST")
        _process_POST();
    else if (header["method"] =="DELETE")
    	_process_DELETE();
    else
    {
        reponse["code"] = "405";
        reponse["status"] = "Method Not Allowed";
    }

}

/*
    Il me semble que je l'ai remis en place pour qu'il fonctionne à nouveau 
    avec GET.
    La fonction va devoir évoluer lors de l'intégration des CGI.
    A voir comment.
*/
void        Request::send_reponse(int socket)
{
    reponse["http_version"] = "HTTP/1.1";

    std::string reply = reponse["http_version"] + " " + reponse["code"] + " " + reponse["status"] + "\n";
    std::map<std::string, std::string> it = reponse;


    if (reponse.find("body") == reponse.end())
        reply.append("\n");
    else
    {
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

    std::cout << "CONTENT TYPE FROM MIME TYPES = [" << it->first << "]\n";

    reponse["Content-Length"]   = header["Content-Length"];
    reponse["Content-Type"]     = header["Content-Type"];
    if (create_file(it->first) != SUCCESS)
    {
        std::cout << "IL FAUT QUE JE CHECK CE QU IL SE PASSE SI LE FICHIER NE SE CREE PAS\n";
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

void    Request::_process_DELETE()
{
    char const *file_to_delete = header["url"].c_str();

    std::cout << "file to delete = (" << file_to_delete << ")\n";
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
