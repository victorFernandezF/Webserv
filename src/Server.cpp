#include "Server.hpp"

/* ************************************************************************** */
/*                        CONSTRUCTORS / DESTRUCTORS                          */
/* ************************************************************************** */

Server::Server()
{
	_listenSet = false;
	_client_body_sizeSet = false;
	return ;
}

Server::Server(Server const &src)
{
	this->_host = src._host;
	this->_listen = src._listen;
	this->_listenSet = src._listenSet;
	this->_server_name = src._server_name;
	this->_client_body_size = src._client_body_size;
	this->_client_body_sizeSet = src._client_body_sizeSet;
	this->_upload_path = src._upload_path;
	this->_error_page = src._error_page;
	this->_fd = src._fd;
	this->_locations = src._locations;
	return ;
}

Server::~Server()
{
	return ;
}

/* ************************************************************************** */
/*                            OPERATORS OVERLOAD                              */
/* ************************************************************************** */

Server	&Server::operator=(Server const &rhs)
{
	if (this != &rhs)
	{
		this->_host = rhs._host;
		this->_listen = rhs._listen;
		this->_listenSet = rhs._listenSet;
		this->_server_name = rhs._server_name;
		this->_client_body_size = rhs._client_body_size;
		this->_client_body_sizeSet = rhs._client_body_sizeSet;
		this->_upload_path = rhs._upload_path;
		this->_error_page = rhs._error_page;
		this->_fd = rhs._fd;
		this->_locations = rhs._locations;
	}
	return (*this);
}

/* ************************************************************************** */
/*                           NON MEMBER FUNCTIONS                             */
/* ************************************************************************** */



/* ************************************************************************** */
/*                             MEMBER FUNCTIONS                               */
/* ************************************************************************** */

void	Server::clear()
{
	_host.clear();
	_listen.clear();
	_listenSet = false;
	_server_name.clear();
	_client_body_size = 0;
	_client_body_sizeSet = false;
	_upload_path.clear();
	_error_page.clear();
	_fd.clear();
	_locations.clear();
}

/* ************************************************************************** */
/*                             GETTERS / SETTERS                              */
/* ************************************************************************** */

void	Server::setHost(std::string host)
{
	this->_host = host;
}

void	Server::setListen(std::vector<std::string> listen)
{
	this->_listen = listen;
	this->_listenSet = true;
}

void	Server::setServerName(std::string sName)
{
	this->_server_name = sName;
}

void	Server::setClientBSize(unsigned int clientBSize)
{
	this->_client_body_size = clientBSize;
	this->_client_body_sizeSet = true;
}

void	Server::setUploadPath(std::string path)
{
	this->_upload_path = path;
}

void	Server::setErrorPage(unsigned short key, std::string value)
{
	this->_error_page[key] = value;
}

void	Server::setErrorPageMap(std::map<unsigned short, std::string> map)
{
	this->_error_page = map;
}

void	Server::setFD(int fd)
{
	this->_fd.push_back(fd);
}

void	Server::addLocation(Location location)
{
	_locations.push_back(location);
}

std::string	Server::getHost() const
{
	return (this->_host);
}

std::vector<std::string> const	&Server::getListen() const
{
	return (this->_listen);
}

bool	Server::isSetListen()
{
	return (this->_listenSet);
}

std::string Server::getServerName() const
{
	return (this->_server_name);
}

unsigned int	Server::getClientBSize() const
{
	return (this->_client_body_size);
}

bool	Server::isSetClientBSize()
{
	return (this->_client_body_sizeSet);
}

std::string	Server::getUploadPath() const
{
	return (this->_upload_path);
}

std::map<unsigned short, std::string> Server::getErrorPage(unsigned short nError)
{
	std::map<unsigned short, std::string>	tmp;
	for (std::map<unsigned short, std::string>::iterator it = _error_page.begin(); it != _error_page.end(); it++)
	{
		if (it->first == nError)
		{
			tmp[it->first] = it->second;
			return(tmp);
		}
	}
	tmp[0] = "not page"; 
	return (tmp);
}

std::map<unsigned short, std::string> Server::getErrorPageMap() const
{
	return (this->_error_page);
}

std::vector<int> const		&Server::getFD() const
{
	return (this->_fd);
}

std::vector<Location>	Server::getLocations() const
{
		return (this->_locations);
}

Location	Server::getMainLoc() const
{
	std::vector<Location> tmp = getLocations();
	Location	ret;

	for (std::vector<Location>::iterator it = tmp.begin(); it != tmp.end(); it++)
	{
		if (it->getLocation() == "/")
			return (*it);
	}
	return (ret);
}

/* ************************************************************************** */
/*                             MEMBER EXCEPTIONS                               */
/* ************************************************************************** */



/* ************************************************************************** */
/*                            NON MEMBER OVERLOAD                             */
/* ************************************************************************** */

std::ostream	&operator<<(std::ostream &o, Server const &i)
{
	(void)i;
	return (o);
}
