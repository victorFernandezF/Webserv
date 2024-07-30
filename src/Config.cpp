#include "Config.hpp"


/* ************************************************************************** */
/*                             MEMBER EXCEPTIONS                              */
/* ************************************************************************** */

class	Config::errorOpen : public std::exception
{
	public:
		virtual const char *what(void) const throw()
		{
			return ("Error: cannot open configuration file");
		}
};

class	Config::errorBraces : public std::exception
{
	public:
		virtual const char *what(void) const throw()
		{
			return ("Error: braces { } unpaired");
		}
};

class	Config::errorServerTag : public std::exception
{
	public:
		virtual const char *what(void) const throw()
		{
			return ("Error: bad or non server tag");
		}
};

class	Config::errorParam : public std::exception
{
	public:
		virtual const char *what(void) const throw()
		{
			return ("Error: params must be\nkey: \"value\"");
		}
};

class	Config::errorQuotes : public std::exception
{
	public:
		virtual const char *what(void) const throw()
		{
			return ("Error: quotes \" \" unpaired");
		}
};

class	Config::errorUnkownParam : public std::exception
{
	public:
		virtual const char *what(void) const throw()
		{
			return ("Error: config param unkown");
		}
};

class	Config::errorListen : public std::exception
{
	public:
		virtual const char *what(void) const throw()
		{
			return ("Error: listen port(s) must be from 0 to 65535");
		}
};

class	Config::errorClientBodySize : public std::exception
{
	public:
		virtual const char *what(void) const throw()
		{
			return ("Error: client body size must be from 0 to 4294967295");
		}
};

class	Config::errorParamMethod : public std::exception
{
	public:
		virtual const char *what(void) const throw()
		{
			return ("Error: HTTP methods params example:\n\"GET\",\"POST\"");
		}
};

class	Config::errorMethod : public std::exception
{
	public:
		virtual const char *what(void) const throw()
		{
			return ("Error: unkown HTTP method or not implemented. Params must be GET, POST and/or DELETE");
		}
};

class	Config::errorUnpairErrorPage : public std::exception
{
	public:
		virtual const char *what(void) const throw()
		{
			return ("Error: error pages params example:\n\"404\",\"./route_to/404.html\",\"405\",\"./route_to/405.html\"");
		}
};

class	Config::errorCodePageError : public std::exception
{
	public:
		virtual const char *what(void) const throw()
		{
			return ("Error: code error of code error page not admitted");
		}
};

class	Config::errorReAssingHost : public std::exception
{
	public:
		virtual const char *what(void) const throw()
		{
			return ("Error: reassigning host param");
		}
};

class	Config::errorNotValidHost : public std::exception
{
	public:
		virtual const char *what(void) const throw()
		{
			return ("Error: listen host address not valid");
		}
};

class	Config::errorReAssingListen : public std::exception
{
	public:
		virtual const char *what(void) const throw()
		{
			return ("Error: reassigning listen param");
		}
};

class	Config::errorReAssingRoot : public std::exception
{
	public:
		virtual const char *what(void) const throw()
		{
			return ("Error: reassigning root param in server");
		}
};

class	Config::errorReAssingClientBodySize : public std::exception
{
	public:
		virtual const char *what(void) const throw()
		{
			return ("Error: reassigning client body size param");
		}
};

class	Config::errorReAssingUploadPath : public std::exception
{
	public:
		virtual const char *what(void) const throw()
		{
			return ("Error: reassigning upload path");
		}
};

class	Config::errorReAssingErrorPages : public std::exception
{
	public:
		virtual const char *what(void) const throw()
		{
			return ("Error: reassigning error pages");
		}
};

class	Config::errorReAssingServerName : public std::exception
{
	public:
		virtual const char *what(void) const throw()
		{
			return ("Error: reassigning server name");
		}
};

class	Config::errorReAssingIndex : public std::exception
{
	public:
		virtual const char *what(void) const throw()
		{
			return ("Error: reassigning index file");
		}
};

class	Config::errorInCompletLocationSrv : public std::exception
{
	public:
		virtual const char *what(void) const throw()
		{
			return ("Error: need add basic \"location\" params:\nallow methods and root\nor\nreturn value");
		}
};

class	Config::errorNoLocationRoute : public std::exception
{
	public:
		virtual const char *what(void) const throw()
		{
			return ("Error: Location param need a route");
		}
};

class	Config::errorNoLocationNoOpened : public std::exception
{
	public:
		virtual const char *what(void) const throw()
		{
			return ("Error: Location params must be into braces { ... }");
		}
};

class	Config::errorReAssingReturn : public std::exception
{
	public:
		virtual const char *what(void) const throw()
		{
			return ("Error: reassigning return param");
		}
};

class	Config::errorReAssingAutoIndex : public std::exception
{
	public:
		virtual const char *what(void) const throw()
		{
			return ("Error: reassigning autoindex param");
		}
};

class	Config::errorReAssingCompiler : public std::exception
{
	public:
		virtual const char *what(void) const throw()
		{
			return ("Error: reassigning compiler param");
		}
};

class	Config::errorServerNameDup : public std::exception
{
	public:
		virtual const char *what(void) const throw()
		{
			return ("Error: server name duplicate in one o more servers");
		}
};

class	Config::errorListenDup : public std::exception
{
	public:
		virtual const char *what(void) const throw()
		{
			return ("Error: listen port duplicate in one o more servers");
		}
};

class	Config::errorNoListenPort : public std::exception
{
	public:
		virtual const char *what(void) const throw()
		{
			return ("Error: listen port needed in servers");
		}
};

class	Config::errorNoListenAddr : public std::exception
{
	public:
		virtual const char *what(void) const throw()
		{
			return ("Error: IP address needed in servers");
		}
};

/* ************************************************************************** */
/*                        CONSTRUCTORS / DESTRUCTORS                          */
/* ************************************************************************** */

Config::Config(std::string file, Cluster &cluster): _file(file)
{
	_fillCLines();
	_checknBraces();
	_makeServers();
	_checkServers();

	for (std::vector<Server>::iterator it = this->_servers.begin(); it != this->_servers.end(); it++)
		cluster.addServer(*it);
}

Config::Config(Config const &src)
{
	this->_file = src._file;
	this->_fLines = src._fLines;
	this->_servers = src._servers;
	return ;
}

Config::~Config()
{
	return ;
}

/* ************************************************************************** */
/*                            OPERATORS OVERLOAD                              */
/* ************************************************************************** */

Config	&Config::operator=(Config const &rhs)
{
	if (this != &rhs)
	{
		this->_file = rhs._file;
		this->_fLines = rhs._fLines;
		this->_servers = rhs._servers;
	}
	return (*this);
}

/* ************************************************************************** */
/*                           NON MEMBER FUNCTIONS                             */
/* ************************************************************************** */


/* ************************************************************************** */
/*                             MEMBER FUNCTIONS                               */
/* ************************************************************************** */

void	Config::_fillCLines()
{
	std::ifstream	conf(this->_file.c_str());
	std::string		line;
	size_t			pos;

	if (!conf.is_open())
		throw errorOpen();
	while (getline(conf, line))
	{
		if (_isEmptyLine(line) || line[0] == '#')
			continue;
		if (line.find('#' != std::string::npos))
		{
			pos = line.find('#');
			line = line.substr(0, pos - 1);
		}
		_fLines.push_back(line);
	}
}

void	Config::_checknBraces()
{
	int	brcOpen = 0;
	int	brcClose = 0;

	for (std::vector<std::string>::iterator it = _fLines.begin(); it != _fLines.end(); it++)
	{
		size_t i = 0;
		while ((*it)[i])
		{
			if ((*it)[i] == '{')
				brcOpen++;
			if ((*it)[i] == '}')
				brcClose++;
			i++;
		}
	}
	if (brcOpen != brcClose)
		throw errorBraces();
}

bool	Config::_isEmptyLine(std::string str)
{
	size_t i = 0;

	while (str[i])
	{
		if (str[i] > 32 && str[i] < 127)
			return (false);
		i++;
	}
	return (true);
}

void	Config::_serverTag(std::string str)
{
	size_t pos;

	if (str.find("{") != std::string::npos)
	{
		pos = str.find('{');
		str.erase(pos, std::string::npos);
	}
	if (!_isEmptyLine(str))
		throw errorServerTag();
}

void	Config::_makeServers()
{
	Server		tmpServ;
	Location	mainLoc;
	int			nbServer = -1;
	bool		startSrv = false;
	bool		startLoc = false;
	int			brcOpen = 0;
	size_t		pos;
	std::string	tmpStr;
	std::string	tmpKey;
	std::string	tmpVal;

	for (std::vector<std::string>::iterator it = _fLines.begin(); it != _fLines.end(); it++)
	{
		tmpStr = *it;
		if (tmpStr.find("server") != std::string::npos && !startSrv)
		{
			nbServer++;
			tmpServ.clear();
			mainLoc.clear();
			mainLoc.setLocation("/");
			startSrv = true;
			pos = tmpStr.find("server");
			tmpStr.erase(pos, 6);
			_serverTag(tmpStr);
			while (startSrv)
			{			
				if (_isEmptyLine(tmpStr))
				{
					it++;
					tmpStr = *it;
					continue ;
				}
				if (!_isEmptyLine(tmpStr) && tmpStr.find('}') != std::string::npos)
				{
					brcOpen--;
					pos = tmpStr.find('}');
					tmpStr.erase(pos, pos + 1);
					if (brcOpen == 1)
						startLoc = false;
				}

				if (tmpStr.find("location") != std::string::npos)
				{
					if (mainLoc.isComplet())
					{
						tmpServ.addLocation(mainLoc);
						mainLoc.clear();
					}
					else
					{
						throw errorInCompletLocationSrv();
					}
					pos = tmpStr.find("location");
					tmpStr.erase(pos, 8);
					_addLocationHead(mainLoc, tmpStr);
					startLoc = true;
				}
				if (!_isEmptyLine(tmpStr) && tmpStr.find('{') != std::string::npos)
				{
					brcOpen++;
					pos = tmpStr.find('{');
					tmpStr.erase(pos, pos + 1);
				}
				if (!_isEmptyLine(tmpStr) && brcOpen == 1)
				{
					tmpKey = _takeKey(tmpStr);
					_addServerParam(tmpServ, mainLoc, tmpKey, tmpStr);
				}
				if (!_isEmptyLine(tmpStr) && brcOpen == 2)
				{
					tmpKey = _takeKey(tmpStr);
					_addLocationParam(mainLoc, tmpKey, tmpStr);
				}
				if (startLoc && brcOpen == 1)
				{
					throw errorNoLocationNoOpened();
				}
				if (brcOpen == 0)
				{
					if (mainLoc.isComplet())
					{
						tmpServ.addLocation(mainLoc);
						mainLoc.clear();
					}
					else
					{
						throw errorInCompletLocationSrv();
					}
					if (tmpServ.getHost().empty())
						throw errorNoListenAddr();
					if (tmpServ.getListen().empty())
						throw errorNoListenPort();
					this->_servers.push_back(tmpServ);
					startSrv = false;
				}
			}
		}
	}
}

size_t	Config::_beginWord(std::string const line)
{
	size_t	i = 0;
	while (line[i] && !(line[i] > 32 && line[i] < 127))
		i++;
	return (i);
}

std::string	Config::_takeKey(std::string &line)
{
	std::string key;
	if (line.find(':') == std::string::npos)
		throw errorParam();
	size_t posKey = _beginWord(line);
	size_t posDot = line.find_first_of(':');
	key = line.substr(posKey, posDot - posKey);
	line.erase(posKey, key.size() + 1);
	return key;
}

void	Config::_checkQuotes(std::string str)
{
	size_t	i = 0;
	int		quot = 0;
	while (str[i])
	{
		if (str[i] == '"')
			quot++;
		i++;
	}
	if (quot % 2 != 0)
		throw errorQuotes();
}

void	Config::_addServerParam(Server &srv, Location &location, std::string key, std::string &val)
{
	_checkQuotes(val);
	if (key == "host")
	{
		if (srv.getHost().empty())
			srv.setHost(_parseVal(val));
		else
			throw errorReAssingHost();
		if (inet_addr(srv.getHost().c_str()) == (in_addr_t)(-1))
			throw errorNotValidHost();
	}
	else if (key == "listen")
	{
		if (srv.getListen().empty())
			srv.setListen(_parseListen(val));
		else
			throw errorReAssingListen();
	}
	else if (key == "root")
	{
		if (location.getRoot().empty())
			location.setRoot(_parseVal(val));
		else
			throw errorReAssingRoot();
	}
	else if (key == "client_body_size")
	{
		if (srv.isSetClientBSize() == false)
			srv.setClientBSize(_parseClientBodySize(val));
		else
			throw errorReAssingClientBodySize();
	}
	else if (key == "allow_methods")
	{
		if (location.getMethods().empty())
			location.setMethods(_parseMethods(val));
	}
	else if (key == "upload_path")
	{
		if  (srv.getUploadPath().empty())
			srv.setUploadPath(_parseVal(val));
		else
			throw errorReAssingUploadPath();
	}
	else if (key == "error_page")
	{
		if (srv.getErrorPageMap().empty())
			srv.setErrorPageMap(_parseErrorPages(val));
		else
			throw errorReAssingErrorPages();
	}
	else if (key == "index")
	{
		if (location.getIndex().empty())
			location.setIndex(_parseVal(val));
		else
			throw errorReAssingIndex();
	}
	else if (key == "server_name")
	{
		if (srv.getServerName().empty())
			srv.setServerName(_parseVal(val));
		else
			throw errorReAssingServerName();
	}
	else if (key == "return")
	{
		if (location.getReturn().empty())
			location.setReturn(_parseVal(val));
		else
			throw errorReAssingIndex();
	}
	else
	{
		throw errorUnkownParam();
	}
}

std::string	Config::_parseVal(std::string &val)
{
	_checkQuotes(val);
	std::string tmp = val;
	std::string	ret;
	size_t	pos1 = tmp.find_first_of('"');
	tmp.erase(pos1, 1);
	size_t	pos2 = tmp.find_first_of('"');
	ret = val.substr(pos1 + 1, pos2 - pos1);
	val.erase(pos1, ret.size() + 2);
	if (!_isEmptyLine(val))
		throw errorParam();
	return (ret);
}

std::vector<std::string>	Config::_parseListen(std::string &val)
{
	_checkQuotes(val);

	std::vector<std::string>	ret;
	std::vector<std::string>	tmpVect;
	std::istringstream			toRead(val);
	std::string					tmp;

	while (getline(toRead, tmp, ','))
	{
		tmpVect.push_back(tmp);
	}
	for (std::vector<std::string>::iterator it = tmpVect.begin(); it != tmpVect.end(); it++)
	{
		std::string tmp = *it;
		std::string	listen;
		size_t	pos1 = tmp.find_first_of('"');
		tmp.erase(pos1, 1);
		size_t	pos2 = tmp.find_first_of('"');
		listen = (*it).substr(pos1 + 1, pos2 - pos1);
		(*it).erase(pos1, listen.size() + 2);
		if (!ft_atoiUnShortCheck(listen))
			throw errorListen();
		if (!_isEmptyLine(*it))
			throw errorParam();
		ret.push_back(listen);
	}
	val.clear();
	return (ret);
}

unsigned int	Config::_parseClientBodySize(std::string &val)
{
	_checkQuotes(val);
	std::string tmp = val;
	std::string	size;
	size_t	pos1 = tmp.find_first_of('"');
	tmp.erase(pos1, 1);
	size_t	pos2 = tmp.find_first_of('"');
	size = val.substr(pos1 + 1, pos2 - pos1);
	val.erase(pos1, size.size() + 2);
	if (!_isEmptyLine(val))
		throw errorParam();	
	if (!ft_atoiUnIntCheck(size))
		throw errorClientBodySize();
	return (ft_atoi(size));
}

std::vector<std::string> Config::_parseMethods(std::string &val)
{
	_checkQuotes(val);
	std::vector<std::string>	ret;
	std::vector<std::string>	tmpVect;
	std::istringstream			toRead(val);
	std::string					tmp;

	while (getline(toRead, tmp, ','))
	{
		tmpVect.push_back(tmp);
	}
	for (std::vector<std::string>::iterator it = tmpVect.begin(); it != tmpVect.end(); it++)
	{
		std::string tmp = *it;
		std::string	method;
		size_t	pos1 = tmp.find_first_of('"');
		tmp.erase(pos1, 1);
		size_t	pos2 = tmp.find_first_of('"');
		method = (*it).substr(pos1 + 1, pos2 - pos1);
		(*it).erase(pos1, method.size() + 2);
		if (method != "GET" && method != "POST" && method != "DELETE")
			throw errorMethod();
		if (!_isEmptyLine(*it))
			throw errorParamMethod();
		ret.push_back(method);
	}
	val.clear();
	return (ret);
}

std::map<unsigned short, std::string> Config::_parseErrorPages(std::string &val)
{
	_checkQuotes(val);
	std::map<unsigned short, std::string>	ret;
	std::vector<std::string>			tmpVect;
	std::istringstream					toRead(val);
	std::string							tmp;

	while (getline(toRead, tmp, ','))
	{
		tmpVect.push_back(tmp);
	}
	if (tmpVect.size() % 2 != 0)
		throw errorUnpairErrorPage();
	for (std::vector<std::string>::iterator it = tmpVect.begin(); it != tmpVect.end(); it++)
	{
		unsigned short key;
		std::string value;
		int			i = 0;
		while (i < 2)
		{
			std::string tmp = *it;
			std::string	str;
			size_t	pos1 = tmp.find_first_of('"');
			tmp.erase(pos1, 1);
			size_t	pos2 = tmp.find_first_of('"');
			str = (*it).substr(pos1 + 1, pos2 - pos1);
			(*it).erase(pos1, str.size() + 2);
			if (!_isEmptyLine(*it))
				throw errorParamMethod();
			if (i == 0)
			{
				if (!ft_atoiErrorPageCheck(str))
					throw errorCodePageError();
				key = ft_atoi(str);
				it++;
			}
			else
				value = str;
			i++;
		}

		ret[key] = value;
	}
	val.clear();
	return (ret);
}

void	Config::_addLocationHead(Location &location, std::string &val)
{
	_checkQuotes(val);
	std::string tmp = val;
	std::string	ret;
	size_t	pos1 = tmp.find_first_of('"');
	if (pos1 == std::string::npos)
		throw errorNoLocationRoute();
	tmp.erase(pos1, 1);
	size_t	pos2 = tmp.find_first_of('"');
	ret = val.substr(pos1 + 1, pos2 - pos1);
	val.erase(pos1, ret.size() + 2);
	location.setLocation(ret);
}

void	Config::_addLocationParam(Location &location, std::string key, std::string &val)
{
	_checkQuotes(val);
	if (key == "root")
	{
		if (location.getRoot().empty())
			location.setRoot(_parseVal(val));
		else
			throw errorReAssingRoot();
	}
	else if (key == "allow_methods")
	{
		if (location.getMethods().empty())
			location.setMethods(_parseMethods(val));



		//Revisar si necesario exception!!!!!!!!



	}
	else if (key == "index")
	{
		if (location.getIndex().empty())
			location.setIndex(_parseVal(val));
		else
			throw errorReAssingIndex();
	}
	else if (key == "return")
	{
		if (location.getReturn().empty())
			location.setReturn(_parseVal(val));
		else
			throw errorReAssingReturn();
	}
	else if (key == "autoindex")
	{
		if (location.getAutoIndex() == false)
		{
			if (_parseVal(val) == "on")
				location.setAutoIndex(true);
			else
				throw errorReAssingAutoIndex();
		}	
		else
		{
			throw errorReAssingAutoIndex();
		}
	}
	else if (key == "compiler")
	{
		if (location.getCompiler().empty())
			location.setCompiler(_parseVal(val));
		else
			throw errorReAssingCompiler();
	}
	else
	{
		throw errorUnkownParam();
	}
}

void	Config::_checkServers()
{
	for (std::vector<Server>::iterator itF = _servers.begin(); itF != _servers.end(); itF++)
	{
		for (std::vector<Server>::iterator itS = _servers.begin(); itS != _servers.end(); itS++)
		{
			if (itF != itS)
			{
				if (itF->getServerName() == itS->getServerName())
					throw errorServerNameDup();
				std::vector<std::string> portF = itF->getListen();
				std::vector<std::string> portS = itS->getListen();
				for (std::vector<std::string>::iterator lisF = portF.begin(); lisF != portF.end(); lisF++)
				{
					for (std::vector<std::string>::iterator lisS = portS.begin(); lisS != portS.end(); lisS++)
						if (*lisF == *lisS)
							throw errorListenDup();
				}
			}
		}
	}
}

void	Config::_printServers()
{
	size_t nbrs = 1;
	
	for (std::vector<Server>::iterator its = _servers.begin(); its != _servers.end(); its++)
	{
		size_t nbrl = 0;
		std::cout << std::endl << "********************************************" << std::endl << std::endl;
		std::cout << "Server number " << nbrs << std::endl;
		std::cout << "Server name: " << its->getServerName() << std::endl;
		std::cout << "Host: " << its->getHost() << std::endl;
		std::cout << "Listen: " << std::endl;
		std::vector<std::string>	tmpL = its->getListen();
		for (std::vector<std::string>::iterator lis = tmpL.begin(); lis != tmpL.end(); lis++)
			std::cout << *lis << std::endl;
		std::cout << "File descriptor: " << std::endl;
		std::vector<int> tmpFD = its->getFD();
		for (std::vector<int>::iterator ifd = tmpFD.begin(); ifd != tmpFD.end(); ifd++)
			std::cout << *ifd << std::endl;
		std::cout << "Client body size: " << its->getClientBSize() << std::endl;
		std::cout << "Upload path: " << its->getUploadPath() << std::endl;
		std::cout << "Error pages:" << std::endl;
		std::map<unsigned short, std::string> tmpPages = its->getErrorPageMap();
		for (std::map<unsigned short, std::string>::iterator ite = tmpPages.begin(); ite != tmpPages.end(); ite++)
			std::cout << ite->first << " -> " << ite->second << std::endl;
		std::cout << std::endl << "Locations:" << std::endl;
		std::vector<Location> tmpLocation = its->getLocations();
		for (std::vector<Location>::iterator itl = tmpLocation.begin(); itl != tmpLocation.end(); itl++)
		{
			std::cout << std::endl;
			std::cout << "Location number " << nbrl << std::endl;
			std::cout << "Location: " << itl->getLocation() << std::endl;
			std::cout << "Return: " << itl->getReturn() << std::endl; 
			std::cout << "Root: " << itl->getRoot() << std::endl;
			std::cout << "Autoindex: " << itl->getAutoIndex() << std::endl;
			std::cout << "Index: " << itl->getIndex() << std::endl;
			std::cout << "Methods:" << std::endl;
			std::vector<std::string> tmpMeth = itl->getMethods();
			for (std::vector<std::string>::iterator itm = tmpMeth.begin(); itm != tmpMeth.end(); itm++)
				std::cout << *itm << std::endl;
			std::cout << "Compiler: " << itl->getCompiler() << std::endl;
			nbrl++;
		}
		std::cout << "********************************************" << std::endl << std::endl;
		nbrs++;
	}
}

/* ************************************************************************** */
/*                             GETTERS / SETTERS                              */
/* ************************************************************************** */


/* ************************************************************************** */
/*                            NON MEMBER OVERLOAD                             */
/* ************************************************************************** */

std::ostream	&operator<<(std::ostream &o, Config const &i)
{
	(void)i;
	return (o);
}
