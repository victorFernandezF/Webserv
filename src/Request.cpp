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

void	Request::_parseRequest(std::string req)
{
	size_t pos;

	if (req.find("\r\n") != std::string::npos)
	{
		pos = req.find("\r\n");
		_firstLine(req.substr(0, pos));
		req.erase(0, pos + 2);
	}

	if (req.find("\r\n\r\n") != std::string::npos)
	{
		_setBody(req.substr(pos + 4));
	}
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
	for (std::vector<std::string>::iterator it = tmpVect.begin(); it != tmpVect.end(); it++)
	{
		size_t j = 1;
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
	(void)i;
	std::cout << "Request info" << std::endl;
	std::cout << "Method: " << i.getMethod() << std::endl;
	std::cout << "Path: " << i.getPath() << std::endl;
	std::cout << "Header: " << i.getHeader() << std::endl;
	std::cout << "Body: " << i.getBody() << std::endl;
	return (o);
}
