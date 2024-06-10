#include "Response.hpp"

/* ************************************************************************** */
/*                             MEMBER EXCEPTIONS                               */
/* ************************************************************************** */



/* ************************************************************************** */
/*                        CONSTRUCTORS / DESTRUCTORS                          */
/* ************************************************************************** */

Response::Response()
{
	return ;
}

Response::Response(Response const &src)
{
	this->_req = src._req;
	this->_srv = src._srv;
	this->_clientFD = src._clientFD;
	return ;
}

Response::Response(Request req, Server *srv, int client)
{
	_srv = *srv;
	_req = req;
	_clientFD = client;

	_exeResponse();

	return ;
}

Response::~Response()
{
	return ;
}

/* ************************************************************************** */
/*                            OPERATORS OVERLOAD                              */
/* ************************************************************************** */

Response	&Response::operator=(Response const &rhs)
{
	if (this != &rhs)
	{
		_req = rhs._req;
		_srv = rhs._srv;
		_clientFD = rhs._clientFD;
	}
	return (*this);
}

/* ************************************************************************** */
/*                           NON MEMBER FUNCTIONS                             */
/* ************************************************************************** */


/* ************************************************************************** */
/*                             MEMBER FUNCTIONS                               */
/* ************************************************************************** */

std::string _makeReponseTestRedirect();

void	Response::_exeResponse()
{
	if (_isLocation(_req.getPath()))
	{
		if (_isAllowedMethod())
		{
			if (_req.getMethod() == "GET")
			{
				//_sendResponse(_makeReponseTestRedirect());
				_getMethodTemp();
			}
			else if (_req.getMethod() == "POST")
				_postMethod();
			else if (_req.getMethod() == "DELETE")
				_deleteMethod();
		}
		else
		{
			_sendResponse(_makeResponse(_getErrorPage(HTTP_METHOD_NOT_ALLOWED)));
		}
	}
	else
	{
		_sendResponse(_makeResponse(_getErrorPage(HTTP_NOT_FOUND)));
	}

}

std::string	Response::_makeResponse(std::string body)
{
	std::string ret;

	ret += "HTTP/1.1 200 OK";
	ret += "\r\n";
	ret += "Connection: close";
	ret += "\r\n";
	ret += "Content-Type: text/html"; //Crear función para rellenar Content-Type
	ret += "\r\n";
	ret += "Content-Length: ";
	ret += ft_itoa(body.size());
	ret += "\r\n";
	ret += "\r\n";
	ret += body;
	ret += "\r\n\r\n";

	return (ret);
}

void	Response::_sendResponse(std::string msg)
{
	std::cout << "SendResponse: " << msg << std::endl;

	const char *str = msg.c_str();
	int msg_len = strlen(str);
	int bytes_sent;

	bytes_sent = send(_clientFD, str, msg_len, 0);
	(void)bytes_sent;
}

std::string _makeReponseTestRedirect()
{
	std::string		header;
	std::string 	body;

	header += "HTTP/1.1 301 MOVED PERMANENTRLY";
	header += "\r\n";
	header += "Connection: close";
	header += "\r\n";
	//header += "Content-Type: text/html";//; charset=utf-8"; 
	//header += "\r\n";
	header += "Location: https://www.google.es";
	header += "\r\n";
	header += "Content-Length: ";
	header += ft_itoa(body.size());
	header += "\r\n";
	header += "\r\n";
	header += body;
	header += "\r\n\r\n";

	return (header);
}

std::string _makeResponseTest()
{
	std::ifstream	file("./root/index.html");
	std::string		line;
	std::string		header;
	std::string 	body;

	while (getline(file, line))
	{
		body += line;
		body += '\n';
	}

	file.close();

	header += "HTTP/1.1 200 OK";
	header += "\r\n";
	header += "Connection: close";
	header += "\r\n";
	header += "Content-Type: text/html";
	header += "\r\n";
	header += "Content-Length: ";
	header += ft_itoa(body.size());
	header += "\r\n";
	header += "\r\n";
	header += body;
	header += "\r\n\r\n";

	std::cout << std::endl;

	return (header);
}

void	Response::_getMethodTemp()
{
	std::string	resp;

	if (_loc.getAutoIndex())
	{
//		std::string html = _autoindex();
		//Función de listado de ficheros, si procede, y construcción de html correspondiente
		return ;
	}
	else
	{
		std::cout << "No autoindex" << std::endl;
		_sendResponse(_makeResponse(_getFile(_parsePathUrl())));
		return ;
	}
}

/*std::string Response::_autoindex(Request req){
	std::string html = "";
	std::string path = req.getPath();
	html = "<!DOCTYPE html>\n<html>\n<head>\n";
    html += "<title>Autoindex</title>\n";
    html += "</head>\n<body>\n";
    html += "<h1>Contenido del directorio:</h1>\n";
    html += "<ul>\n";
 	
	DIR* directory = opendir(path.c_str());
	if (!_isPathAFile(req)) {
        struct dirent* entry;
        while ((entry = readdir(directory)) != nullptr) {
            std::string name = entry->d_name;
            if (name != "." && name != "..") {
				if (entry->d_type == DT_REG) {
                      html += "<li><a href=\"" + name + "\">" + name + "</a></li>\n";
                } else if (entry->d_type == DT_DIR) {
                      html += "<li><a href=\"" + name + "/\">" + name + "</a></li>\n";
                }
            }
        }
		html += "</ul>\n";
		html += "</body>\n</html>\n";
    } 

	return (html);
}*/

bool	Response::_isPathAFile(Request req)
{
	size_t dot = req.getPath().find(".");
	
	if (dot == std::string::npos){
		if ( dot == req.getPath().size() - 1)
			return false;
	}
	return true; 
}

/*
void Response::_isPathAccessible(Request req) 
{
	std::string path = req.getPath();
	if (access(path.c_str(), F_OK) != 0)
		_sendResponse(_makeResponse(_getErrorPage(HTTP_METHOD_NOT_ALLOWED)));
}*/

void	Response::_getMethod()
{
	_sendResponse(_makeResponseTest());
}

void	Response::_postMethod()
{
	
}

void	Response::_deleteMethod()
{

}

bool	Response::_isLocation(std::string loc)
{
	std::vector<Location> paths = _srv.getLocations();

	for (std::vector<Location>::iterator it = paths.begin(); it != paths.end(); it++)
	{
		if (it->getLocation() == loc)
		{
			_loc = *it;
			return (true);
		}
		if (loc.find(it->getLocation()) != std::string::npos
			&& ft_isBeginStr(loc, it->getLocation())
			&& (it->getLocation().size() == 1 || loc[it->getLocation().size()] == '/'))
		{
			_loc = *it;
			return (true);
		}
		else
		{
			return (false);
		}
	}
	return (false);
}

bool	Response::_isAllowedMethod()
{
	std::vector<std::string> mths = _loc.getMethods();
	for (std::vector<std::string>::iterator mt = mths.begin(); mt != mths.end(); mt++)
	{
		if (*mt == _req.getMethod())
			return (true);
	}
	return (false);
}

std::string	Response::_makeResponseRedirect()
{
	//construir a parte de _makeReponseTestRedirect(), insertándole _loc.getReturn()
	// en la etiqueta Location

	return ("");
}

std::string Response::_parsePathUrl()
{	
	std::string	tmp;

	if (_loc.getLocation() == _req.getPath())
	{
		return (_parsePathIndex());
	}
	else
	{
		tmp = _loc.getRoot();
		if (tmp[tmp.size() - 1] == '/' && _req.getPath()[0] == '/')
			tmp.erase(tmp[tmp.size() - 1], 1);
		return (tmp + _req.getPath());
	}
	
}

std::string	Response::_parsePathIndex()
{
	std::string ret;

	ret += _loc.getRoot();
	if (ret[ret.size() -1] != '/')
		ret += '/';
	ret += _loc.getIndex();

	return (ret);
}

std::string	Response::_getFile(std::string name)
{
	std::ifstream	file(name.c_str());
	std::string 	ret;
	std::string 	line;

	if (!file.is_open())
		return (_getErrorPage(HTTP_NOT_FOUND));

	while (getline(file, line))
	{
		ret += line;
		ret += '\n';
	}

	return (ret);
}

std::string	Response::_getErrorPage(unsigned short nbr)
{
	std::string	ret;
	std::string filePath;
	std::map<unsigned short, std::string>	pages = _srv.getErrorPageMap();

	if (pages.find(nbr) != pages.end())
		filePath = pages[nbr];
	if (!filePath.empty())
	{
		std::ifstream	file(filePath.c_str());
		std::string		line;
		if (file.is_open())
		{
			while (getline(file, line))
			{
				ret += line;
				ret += '\n';
			}
			return (ret);
		}
		else
		{
			return (_makeErrorPage(nbr));
		}
	}
	else
	{
		return (_makeErrorPage(nbr));
	}
}

std::string	Response::_makeErrorPage(unsigned short nbr)
{
	std::string ret;

	ret += _makeHtmlHead("Error");
	ret += _makeErrorBody(nbr);
	ret += _makeHtmlTail();

	return (ret);
}

std::string	Response::_makeHtmlHead(std::string title)
{
	std::string ret;

	ret += "<!DOCTYPE html>\n";
	ret += "<html>\n";
	ret += "\t<head>\n";
	ret += "\t\t<title>";
	ret += title;
	ret += "</title>\n";
	ret += "\t</head>\n";
	ret += "\t<body>\n";

	return (ret);
}

std::string Response::_makeHtmlTail()
{
	std::string ret;

	ret += ret += "\t</body>\n";
	ret += "</html>\n";

	return (ret);
}

std::string Response::_makeErrorBody(unsigned short nbr)
{
	std::string ret;

	ret += "<h3>webserv 1.0</h3>\n";
	ret += "<h4>victofer  fortega-</h4>\n<hr>\n";
	ret += "<h1 style=\"text-align: center\">" + ft_itoa(nbr) + "</h1>\n";
	ret += "<h2 style=\"text-align: center\">" + _getCodePageText(nbr) + "</h2>\n";

	return (ret);
}

std::string Response::_getCodePageText(unsigned short nbr)
{
	if (nbr == HTTP_OK)
		return ("OK");
	if (nbr == HTTP_CREATED)
		return ("Created");
	if (nbr == HTTP_NO_CONTENT)
		return ("No Content");
	if (nbr == HTTP_BAD_REQUEST)
		return ("Bad Request");
	if (nbr == HTTP_UNAUTHORIZED)
		return ("Unauthorized");
	if (nbr == HTTP_FORBIDDEN)
		return ("Forbidden");
	if (nbr == HTTP_NOT_FOUND)
		return ("Page Not Found");
	if (nbr == HTTP_METHOD_NOT_ALLOWED)
		return ("Method Not Allowed");
	if (nbr == HTTP_REQUEST_ENTITY_TOO_LARGE)
		return ("Request Entity Too Large");
	if (nbr == HTTP_REQUEST_URI_TOO_LONG)
		return ("Request-URI Too Long");
	if (nbr == HTTP_INTERNAL_SERVER_ERROR)
		return ("Internal Server Error");
	return ("");
}

/* ************************************************************************** */
/*                             GETTERS / SETTERS                              */
/* ************************************************************************** */

Request	Response::getRequest() const
{
	return (_req);
}

Server Response::getServer() const
{
	return (_srv);
}

int	Response::getClientFD() const
{
	return (_clientFD);
}

/* ************************************************************************** */
/*                            NON MEMBER OVERLOAD                             */
/* ************************************************************************** */

std::ostream	&operator<<(std::ostream &o, Response const &i)
{
	std::cout << "*******************************************************" << std::endl;
	std::cout << std::endl << "En Response:" << std::endl << std::endl;
	std::cout << "Cliente: " << i.getClientFD() << std::endl;
	std::cout << i.getRequest() << std::endl;
	/*std::cout << std::endl << "/////////////////" << std::endl;
	std::cout << "ContentType = " << i.getRequest().getContentType() << std::endl;
	std::cout << "Boundry = " << i.getRequest().getBoundary() << std::endl << "/////////////////" << std::endl;*/
	//std::cout << i.getServer() << std::endl;
	std::cout << "*******************************************************" << std::endl;
	return (o);
}
