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
	(void)nbr;
	std::string ret;

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


/* ************************************************************************** */
/*                            NON MEMBER OVERLOAD                             */
/* ************************************************************************** */

std::ostream	&operator<<(std::ostream &o, Response const &i)
{
	std::cout << "*******************************************************" << std::endl;
	std::cout << std::endl << "En Response:" << std::endl << std::endl;
	std::cout << "Cliente: " << i.getClientFD() << std::endl;
	std::cout << i.getRequest() << std::endl;
	std::cout << i.getServer() << std::endl;
	std::cout << "*******************************************************" << std::endl;
	return (o);
}
