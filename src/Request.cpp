#include "Request.hpp"

/* ************************************************************************** */
/*                             MEMBER EXCEPTIONS                               */
/* ************************************************************************** */



/* ************************************************************************** */
/*                        CONSTRUCTORS / DESTRUCTORS                          */
/* ************************************************************************** */

Request::Request()
{
	return ;
}

Request::Request(std::string req, int fd, Server srv)
{
	this->_clientFD = fd;
	this->_Server = srv;
	_parseRequest(req);
}

Request::Request(Request const &src)
{
	*this = src;
	return ;
}

Request::~Request()
{
	return ;
}

/* ************************************************************************** */
/*                            OPERATORS OVERLOAD                              */
/* ************************************************************************** */

Request	&Request::operator=(Request const &rhs)
{
	(void)rhs;
	return (*this);
}

/* ************************************************************************** */
/*                           NON MEMBER FUNCTIONS                             */
/* ************************************************************************** */



/* ************************************************************************** */
/*                             MEMBER FUNCTIONS                               */
/* ************************************************************************** */

/*void	Request::_parseRequest(std::string req)
{
	size_t pos;

	if (req.find("\r\n") != std::string::npos)
	{
		pos = req.find("\r\n");
		_firstLine(req.substr(0, pos));
		req.erase(0, pos + 2);
	}
	//std::cout << std::endl << "Post first line: " << req << std::endl << std::endl;
	if (req.find("\r\n\r\n") != std::string::npos)
	{
		pos = req.find("\r\n\r\n");
		_setHeader(req.substr(0, pos));
		_setHeaderParams(req.substr(0, pos));
		req.erase(0, pos + 4);
	}
	if (req.find("\r\n\r\n") != std::string::npos)
	{
		_setBody(req);
	}
}*/

void	Request::_parseRequest(std::string req)
{
	size_t pos;

	if (req.find("\r\n") != std::string::npos)
	{
		pos = req.find("\r\n");
		_firstLine(req.substr(0, pos));
		req.erase(0, pos + 2);
	}
	//std::cout << std::endl << "Post first line: " << req << std::endl << std::endl;
	if (req.find("\r\n\r\n") != std::string::npos)
	{
		pos = req.find("\r\n\r\n");
		_setHeader(req.substr(0, pos));
		_setHeaderParams(req.substr(0, pos));
		req.erase(0, pos + 4);
		_setBody(req);
	}
	//if (req.find("\r\n\r\n") != std::string::npos)
	//{
		//std::cout << "Resto request pre body: " << std::endl << req << std::endl;
	//}
}

void	Request::_firstLine(std::string str)
{
	std::vector<std::string>	tmpVect;
	std::istringstream			toRead(str);
	std::string					tmp;
	std::string					ret;

	while (getline(toRead, tmp, ' '))
	{
		tmpVect.push_back(tmp);
	}
	if (tmpVect.size() != 3)
		return ;
	size_t j = 1;
	for (std::vector<std::string>::iterator it = tmpVect.begin(); it != tmpVect.end(); it++)
	{
		tmp = *it;
		ret.clear();
		for (size_t i = 0; i < tmp.size(); i++)
		{
			if (tmp[i] != ' ')
			{
				ret = ret + tmp[i];
			}
		}
		if (j == 1)
			_setMethod(ret);
		else if (j == 2)
			_setPath(ret);
		else if (j == 3)
			_setVersion(ret);
		j++;
	}
}

void	Request::_setHeaderParams(std::string header)
{
	std::vector<std::string>	tmpVect;
	std::istringstream			toRead(header);
	std::string					tmp;
	std::string					ret;
	size_t						pos;

	while (getline(toRead, tmp))
	{
		if (tmp.find(':') != std::string::npos)
		{
			pos = tmp.find(':');
			//std::cout << "1-> " << tmp.substr(0, pos - 1) << "-----" << "2-> " << tmp.substr(pos) << std::endl;
			this->_headerParams[tmp.substr(0, pos)] = tmp.substr(pos + 2);
		}
	}
}

/*void	Request::_body(std::string str)
{
	(void)str;
}*/

/* ************************************************************************** */
/*                             GETTERS / SETTERS                              */
/* ************************************************************************** */

std::string	Request::getPath() const
{
	return (this->_path);
}

std::string	Request::getMethod() const
{
	return (this->_method);
}

std::string	Request::getVersion() const
{
	return (this->_version);
}

std::string	Request::getHeader() const
{
	return (this->_header);
}

std::string	Request::getBody() const
{
	return (this->_body);
}

std::map<std::string, std::string>	Request::getHeaderParams() const
{
	return (this->_headerParams);
}

void	Request::_setPath(std::string path)
{
	this->_path = path;
}

void	Request::_setMethod(std::string method)
{
	this->_method = method;
}

void	Request::_setVersion(std::string version)
{
	this->_version = version;
}

void	Request::_setHeader(std::string header)
{
	this->_header = header;
}

void	Request::_setBody(std::string body)
{
	this->_body = body;
}

/* ************************************************************************** */
/*                            NON MEMBER OVERLOAD                             */
/* ************************************************************************** */

std::ostream	&operator<<(std::ostream &o, Request const &i)
{
	std::map<std::string, std::string>	hp = i.getHeaderParams();

	std::cout << "Request info" << std::endl << std::endl;
	std::cout << "Method: " << i.getMethod() << std::endl;
	std::cout << "Version: " << i.getVersion() << std::endl;
	std::cout << "Path: " << i.getPath() << std::endl;
	//std::cout << "Header: " << i.getHeader() << std::endl;
	std::cout << std::endl << "Header params:" << std::endl;
	for (std::map<std::string, std::string>::iterator it = hp.begin(); it != hp.end(); it++)
		std::cout << it->first << " ----> " << it->second << std::endl;
	std::cout << std::endl << "Body: " << std::endl << i.getBody() << std::endl;
	return (o);
}
