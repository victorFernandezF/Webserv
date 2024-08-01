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
	_contentType = "text/html";
	_responseCode = "200 OK";

	_exeResponse();

	return ;
}

Response::Response(int code, int fd)
{
	_clientFD = fd;
	_sendResponse(_makeResponse(_getErrorPage(code)));

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

void	Response::_exeResponse()
{
	if (_srv.getServerName().size() + _req.getPath().size() > URI_MAX_SIZE)
	{
		_sendResponse(_makeResponse(_getErrorPage(HTTP_REQUEST_URI_TOO_LONG)));
		return ;
	}
	if (_req.getContentLength() > _srv.getClientBSize())
	{
		_sendResponse(_makeResponse(_getErrorPage(HTTP_REQUEST_ENTITY_TOO_LARGE)));
		return ;
	}

	if (_isLocation(_req.getPath()))
	{
		if (!_loc.getReturn().empty())
		{
			_sendResponse(_makeResponseRedirect());
		}
		else if (_isAllowedMethod())
		{
			if (_req.getMethod() == "GET")
				_getMethod();
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
	}

	for (std::vector<Location>::iterator it = paths.begin(); it != paths.end(); it++)
	{
		std::string tmp;
		size_t pos;
		std::string ext = "*." + _getExtFile(loc);
		if (it->getLocation() == ext)
		{
			_loc = *it;
			pos = loc.find_last_of("/");
			if (pos != std::string::npos)
				tmp = loc.substr(pos + 1);
			else
				tmp = loc;
			_resourcePath = _loc.getRoot();
			if (_resourcePath[_resourcePath.size() - 1] != '/')
				_resourcePath += '/';
			_resourcePath += tmp;
			return (true);
		}
	}

	for (std::vector<Location>::iterator it = paths.begin(); it != paths.end(); it++)
	{

		if (ft_isBeginStr(loc, it->getLocation()) && it->getLocation() != "/")
		{
			std::string tmp;
			size_t pos;
			pos = loc.find(it->getLocation());
			if (pos != std::string::npos)
				tmp = loc.substr(pos + it->getLocation().size());
			else
				tmp = loc;
			if ((tmp.size() > 0 && tmp[0] == '/')
				|| tmp.find('/') == std::string::npos)
			{
				_loc = *it;
				_resourcePath = _loc.getRoot();
				if (_resourcePath[_resourcePath.size() - 1] != '/')
					_resourcePath += '/';
				_resourcePath += tmp;
				return (true);
			}
		}
	}

	for (std::vector<Location>::iterator it = paths.begin(); it != paths.end(); it++)
	{

		if (it->getLocation() == "/")
		{
			_loc = *it;
			_resourcePath = _loc.getRoot();
				if (_resourcePath[_resourcePath.size() - 1] != '/')
					_resourcePath += '/';
				_resourcePath += loc;
				return (true);
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

void	Response::_getMethod()
{
	if (!_loc.getCompiler().empty())
	{
		_sendResponse(_makeResponse(_exeCgi()));
	}
	else if (_loc.getAutoIndex() && _loc.getLocation() == _req.getPath())
	{
		_sendResponse(_makeResponse(_autoindex()));
	}
	else if (_isPathOrDirectory(_parsePathUrl()) != -1)
	{
		_sendResponse(_makeResponse(_getFile(_parsePathUrl())));
	}
	else
	{
		std::cout << "Not found GET" << std::endl;
		_sendResponse(_makeResponse(_getErrorPage(HTTP_NOT_FOUND)));
	}
}

std::string	Response::_makeResponse(std::string body)
{
	std::string ret;

	ret += "HTTP/1.1 ";
	ret += _getResponseCode();
	ret += "\r\n";
	ret += "Connection: close";
	ret += "\r\n";
	ret += "Content-Type: ";
	ret += getContentType();
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
	const char *str = msg.c_str();
	int msg_len = msg.size();

	send(_clientFD, str, msg_len, 0);
}

std::string Response::_makeResponseRedirect()
{
	std::string		header;
	std::string 	body;

	header += "HTTP/1.1 301 MOVED PERMANENTLY";
	header += "\r\n";
	header += "Connection: close";
	header += "\r\n";
	header += "Location: ";
	header += _loc.getReturn();
	header += "\r\n";
	header += "Content-Length: ";
	header += ft_itoa(body.size());
	header += "\r\n";
	header += "\r\n";
	header += body;
	header += "\r\n\r\n";

	return (header);
}

std::string Response::_autoindex(){
	std::string html = "";
	std::string path = _loc.getRoot();
	html = "<!DOCTYPE html>\n<html>\n<head>\n";
    html += "<title>Autoindex</title>\n";
    html += "</head>\n<body>\n";
	html += "<p><a href=\"/\">Inicio</a></p>\n";
    html += "<h1>Contenido del directorio:</h1>\n";
	html += "<hr>";
    html += "<ul>\n";

	if (!_isPathAccesible(path))
		_sendResponse(_makeResponse(_getErrorPage(HTTP_METHOD_NOT_ALLOWED)));

	DIR* directory = opendir(path.c_str());
	if (_isPathOrDirectory(path) == 0) {
		struct dirent* entry;
			while ((entry = readdir(directory)) != NULL) {
				std::string name = entry->d_name;
				if (name != "." && name != "..") {
					if (entry->d_type == DT_REG) {
						html += "<li><a href=\"" + _req.getPath() + "\\" + name + "\">" + name + "</a></li>\n";
					} else if (entry->d_type == DT_DIR) {
						html += "<li><a href=\"" + name + "/\">" + name + "</a></li>\n";
					}
				}
			}
		html += "</ul>\n";
		html += "</body>\n</html>\n";
    }
	return (html);
}

std::string	Response::_parseRoute(std::string path)
{
	std::string	ret;
	size_t		pos = path.find(' ');

	while (pos != std::string::npos)
	{
		ret += path.substr(0, pos);
		ret += 92;
		ret += 32;
		path = path.substr(pos + 1);
		pos = path.find(' ');
	}
	return (ret);
}

// Return 1 if path is file, 0 if is directory -1 if not found or doesn't exist
int Response::_isPathOrDirectory(const std::string path){
	struct stat datos;

	std::cout << "entra comprobacion pathOr: " << path << std::endl;
    if (stat(path.c_str(), &datos) == 0){
    	if (S_ISREG(datos.st_mode))
			return 1;
    	if (S_ISDIR(datos.st_mode))
			return 0;
	}
	return -1;
}

bool Response::_isPathAccesible(std::string path){
	return access(path.c_str(), F_OK) == 0 ? true : false;
}

int	Response::_isFolderOrFile()
{
	return (0);
}

void	Response::_postMethod()
{
	if (!(_srv.getUploadPath().empty()) && _req.getContentType().find("multipart/form-data") != std::string::npos)
		_takeFile();
	else if (_req.getContentType().find("application/x-www-form-urlencoded") != std::string::npos)
		_takeForm();
	else
		_sendResponse(_makeResponse(_getErrorPage(HTTP_METHOD_NOT_ALLOWED)));
}

void	Response::_takeFile()
{
	std::string	dir = _loc.getRoot();
	std::string file = _getFileName();
	std::string data = _cleanBoundary();
	int 		status;

	if (dir.size() - 1 != '/')
		dir += '/';

	dir = dir + _srv.getUploadPath();

	if (!(data.empty()) && !(file.empty()))
	{
		struct stat fold;
		status = stat(dir.c_str(), &fold);
		if (status == -1)
			status = mkdir(dir.c_str(), 0700);
		if (status == -1)
		{
			_sendResponse(_makeResponse(_getErrorPage(HTTP_FORBIDDEN)));
			return ;
		}
		if (dir.size() - 1 != '/')
			dir += '/';
		dir = dir + file;
		status = open(dir.c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (status > 0)
		{
			write(status, data.c_str(), data.size());
			close(status);
			_sendResponse(_makeResponse(_getErrorPage(HTTP_CREATED)));
		}
		else
		{
			_sendResponse(_makeResponse(_getErrorPage(HTTP_INTERNAL_SERVER_ERROR)));
		}
	}
	else
	{
		_sendResponse(_makeResponse(_getErrorPage(HTTP_BAD_REQUEST)));
	}
}

std::string Response::_getFileName()
{
	std::string	filename;
	std::string	body = _req.getBody();
	size_t		pos = body.find("filename=\"");
	if (pos != std::string::npos)
	{
		std::string tmp;
		tmp = body.substr(pos + 10);
		pos = tmp.find("\"");
		filename = tmp.substr(0, pos);
	}
	return (filename);
}

std::string	Response::_cleanBoundary()
{
	std::string	ret;
	std::string body = _req.getBody();

	std::string boundary = "--" + _req.getBoundary() + "--";
	size_t pos = body.find("\r\n\r\n");
	std::string tmp;

	if(pos != std::string::npos)
	{
		tmp = body.substr(pos + 4);
		pos = tmp.find_last_of("\r\n");
		ret = tmp.substr(0, pos - boundary.size());
	}

	return (ret);
}

std::string	Response::_exeCgi()
{
	std::cout << "exeCgi" << std::endl;

	std::string	ret;
	int			readsTot = 0;
	int			pFd[2];
	int			pid;
	int			status;

	if (_isPathOrDirectory(_resourcePath) != 1)
		return(_getErrorPage(HTTP_NOT_FOUND));

	status = pipe(pFd);
	if (status == -1)
		return (_getErrorPage(HTTP_INTERNAL_SERVER_ERROR));

	pid = fork();
	if (pid == -1)
		return (_getErrorPage(HTTP_INTERNAL_SERVER_ERROR));
	if (pid == 0)
	{
		dup2(pFd[1], STDOUT_FILENO);
		close(pFd[0]);
		close(pFd[1]);

		std::string compiler = _loc.getCompiler();
		std::string resourcePath = _resourcePath;

		char *args[] = {
					(char*)compiler.c_str(),
					(char*)resourcePath.c_str(),
					(char*)0
				};
		char **envArgs = makeMatArgs(_req.getVarUrl());

		execve(_loc.getCompiler().c_str(), args, envArgs);
		freeMat(envArgs);
		exit(0);
	}
	else
	{
		char	buff[512];
		int		reads;

		dup2(pFd[0], STDIN_FILENO);
		close(pFd[1]);

		reads = read(pFd[0], buff, 512);
		while (reads > 0)
		{
			readsTot += reads;
			ret += buff;
			reads = read(pFd[0], buff, 512);
		}
		close(pFd[0]);

		ret = ret.substr(0, readsTot);

		int	waitSt;
		waitpid(pid, &waitSt, 0);
		if (WIFEXITED(waitSt))
		{
			status = WEXITSTATUS(waitSt);
			if (status != 0)
				return (_getErrorPage(HTTP_INTERNAL_SERVER_ERROR));
		}
		else
		{
			return (_getErrorPage(HTTP_INTERNAL_SERVER_ERROR));
		}
	}
	return (ret);
}

void	Response::_takeForm()
{
	std::istringstream toRead(_req.getBody());
	std::map<std::string, std::string>	vars;
	std::string	buff;
	size_t		pos;

	while (getline(toRead, buff, '&'))
	{
		pos = buff.find('=');
		if (pos != std::string::npos)
			vars[buff.substr(0, pos)] = buff.substr(pos + 1);
	}
	_req.setVarsUrl(vars);
	if (!_loc.getCompiler().empty())
		_sendResponse(_makeResponse(_exeCgi()));
	else
		_sendResponse(_makeResponse(_getErrorPage(HTTP_OK)));
}

void	Response::_deleteMethod()
{
	std::string path = _parsePathUrl();
	if (_isPathOrDirectory(path) == -1)
	{
		_sendResponse("HTTP/1.1 204 Not Content\r\nConnection: close\r\n");
	}
	else
	{
		if (remove(path.c_str()) == 0)
			_sendResponse(_makeResponse(_getErrorPage(HTTP_OK)));
		else
			_sendResponse(_makeResponse(_getErrorPage(HTTP_INTERNAL_SERVER_ERROR)));
	}
}

std::string Response::_parsePathUrl()
{
	std::string	tmp;

	if (_loc.getLocation() == _req.getPath())
	{
		return (_parsePathIndex());
	}
	return (_resourcePath);
}

std::string	Response::_parsePathIndex()
{
	std::string ret;

	ret += _loc.getRoot();
	if (ret[ret.size() -1] != '/')
		ret += '/';
	ret += _loc.getIndex();

	std::cout << "Ret parseIndex: " << ret << std::endl;
	return (ret);
}

std::string	Response::_getFile(std::string name)
{
	std::ifstream	file;
	std::string 	ret;
	std::stringstream buff;

	file.open(name.c_str(), std::ifstream::in);

	std::cout << "Busca en getFile: " << name << std::endl;

	if (!file.is_open() || _isPathOrDirectory(name) != 1)
	{
		std::cout << "Devuelve not found en getFile" << std::endl;
		return (_getErrorPage(HTTP_NOT_FOUND));
	}

	_contentType = _takeContentType(name);

	buff << file.rdbuf();
	ret = buff.str();

	file.close();

	std::cout << "File: " << name << std::endl;
	std::cout << ret << std::endl;

	return (ret);
}

std::string	Response::_getExtFile(std::string filename)
{
	std::string	ret;
	size_t		pos = filename.find_last_of(".");

	if (pos != std::string::npos)
		ret = str_tolower(filename.substr(pos + 1));
	pos = ret.find('?');
	if (pos != std::string::npos)
		ret = ret.substr(0, pos);
	return (ret);
}

std::string Response::_takeContentType(std::string filename)
{
	std::string ext = _getExtFile(filename);

	if (ext.empty())
		return("unknown");
	if (ext == "html" || ext == "htm")
		return ("text/html");
	else if (ext == "txt")
		return ("text/plain");
	else if (ext == "csv")
		return ("text/csv");
	else if (ext == "xml")
		return ("text/xml");
	else if (ext == "css")
		return ("text/css");
	else if (ext == "gif")
		return ("image/gif");
	else if (ext == "jpeg" || ext == "jpg")
		return ("image/jpeg");
	else if (ext == "png")
		return ("image/png");
	else if (ext == "tiff")
		return ("image/tiff");
	else if (ext == "js")
		return ("application/javascript");
	else if (ext == "pdf")
		return ("application/pdf");
	else if (ext == "json")
		return ("application/json");
	else if (ext == "zip")
		return ("application/zip");
	else
		return ("unknown/" + ext);
}

std::string	Response::_getErrorPage(unsigned short nbr)
{
	std::string	ret;
	std::string filePath;
	std::string	dir = _srv.getMainLoc().getRoot();
	std::map<unsigned short, std::string>	pages = _srv.getErrorPageMap();

	_responseCode = ft_itoa(nbr) + " " + _getCodePageText(nbr);

	if (pages.find(nbr) != pages.end())
	{
		filePath = pages[nbr];
		if (dir.size() - 1 != '/')
				dir += '/';
		filePath = dir + filePath;
	}
	std::cout << "MakeErrorPage filepath: " << filePath << std::endl;
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

	ret += _makeHtmlHead(_getCodePageText(nbr));
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

	ret += "<p><a href=\"/\">Inicio</a></p>";
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
	if (nbr == HTTP_REQUEST_TIMEOUT)
		return ("Request Timeout");
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

std::string	Response::_getResponseCode()
{
	return (_responseCode);
}

std::string Response::getContentType() const
{
	return (_contentType);
}

/* ************************************************************************** */
/*                            NON MEMBER OVERLOAD                             */
/* ************************************************************************** */

std::ostream	&operator<<(std::ostream &o, Response const &i)
{
	(void)i;
	return (o);
}
