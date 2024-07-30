#include "Cluster.hpp"

/* ************************************************************************** */
/*                             MEMBER EXCEPTIONS                              */
/* ************************************************************************** */

class	Cluster::errorParseListenPort : public std::exception
{
	public:
		virtual const char *what(void) const throw()
		{
			return ("Error: listen port not allowed");
		}
};

class	Cluster::errorCreateSocket : public std::exception
{
	public:
		virtual const char *what(void) const throw()
		{
			return ("Error: creating socket error");
		}
};

class	Cluster::errorOptSocket : public std::exception
{
	public:
		virtual const char *what(void) const throw()
		{
			return ("Error: set option socket error");
		}
};

class	Cluster::errorBindSocket : public std::exception
{
	public:
		virtual const char *what(void) const throw()
		{
			return ("Error: bind socket error, IP or port unavailable");
		}
};

class	Cluster::errorListenSocket : public std::exception
{
	public:
		virtual const char *what(void) const throw()
		{
			return ("Error: cannot listen on a socket server");
		}
};

class	Cluster::errorNonBlockFD : public std::exception
{
	public:
		virtual const char *what(void) const throw()
		{
			return ("Error: cannot change socket options");
		}
};

class	Cluster::errorPolling : public std::exception
{
	public:
		virtual const char *what(void) const throw()
		{
			return ("Error: cannot poll sockets");
		}
};

/* ************************************************************************** */
/*                        CONSTRUCTORS / DESTRUCTORS                          */
/* ************************************************************************** */

Cluster::Cluster()
{
	return ;
}

Cluster::Cluster(Cluster const &src)
{
	this->_servers = src._servers;
	this->_pollFDs = src._pollFDs;
	this->_clients = src._clients;
	this->_requests = src._requests;
	return ;
}

Cluster::~Cluster()
{
	_closeFDs();
	return ;
}

/* ************************************************************************** */
/*                            OPERATORS OVERLOAD                              */
/* ************************************************************************** */

Cluster	&Cluster::operator=(Cluster const &rhs)
{
	if (this != &rhs)
	{
		this->_servers = rhs._servers;
		this->_pollFDs = rhs._pollFDs;
		this->_clients = rhs._clients;
		this->_requests = rhs._requests;
	}
	return (*this);
}

/* ************************************************************************** */
/*                           NON MEMBER FUNCTIONS                             */
/* ************************************************************************** */


/* ************************************************************************** */
/*                             MEMBER FUNCTIONS                               */
/* ************************************************************************** */

void	Cluster::addServer(Server srv)
{
	this->_servers.push_back(srv);
}

void	Cluster::_startListen()
{
	for (std::vector<Server>::iterator srv = _servers.begin(); srv != _servers.end(); srv++)
	{
		std::vector<std::string> vListen = srv->getListen();
		for (std::vector<std::string>::iterator lis = vListen.begin(); lis != vListen.end(); lis++)
		{
			srv->setFD(_startSocket(*srv, (*lis)));
		}
	}
}

int		Cluster::_startSocket(Server &server, std::string port)
{
	struct sockaddr_in 		sa;
	int						fd;
	int						status;
	int						optval = 1;

	//Rellenar datos para el socket
	bzero(&sa, sizeof(sa));	//Todo a 0
	sa.sin_family = AF_INET; 		//Permitir IPv4 o IPv6
	sa.sin_addr.s_addr = inet_addr(server.getHost().c_str());		//Indica socket para la escucha, dirección IP local
	sa.sin_port = htons(ft_atoiUnInt(port));

	fd = socket(sa.sin_family, SOCK_STREAM, 0); //Generar FD de socket
	if (fd < 0)
		throw errorCreateSocket();

	status = setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));
	if (status != 0)
		throw errorOptSocket();

	status = bind(fd, (struct sockaddr *)&sa, sizeof sa);	//Asigna dirección local para la escucha
	if (status != 0)
		throw errorBindSocket();

	status = listen(fd, MAXCLIENT);				//Escucha en el fd del socket con cola de 20
	if (status != 0)
		throw errorListenSocket();

	status = fcntl(fd, F_SETFL, O_NONBLOCK);	//Establece el fd objetivo en no bloqueante
	if (status != 0)
		throw errorNonBlockFD();
	return (fd);
}

void	Cluster::_makeServerPolls()
{
	for (std::vector<Server>::iterator srv = _servers.begin(); srv != _servers.end(); srv++)
	{
		std::vector<int> FDS = srv->getFD();
		for (std::vector<int>::iterator fd = FDS.begin(); fd != FDS.end(); fd++)
		{
			struct pollfd 			pollFd;
			pollFd.fd = *fd;
			pollFd.events = POLLIN; //asignar monitorizacion de lectura
			this->_pollFDs.push_back(pollFd);
		}
	}
}

void	Cluster::runServers()
{
	_startListen();
	_makeServerPolls();

	std::cout << *this << std::endl;

	while (1)
	{
		
		struct sockaddr_storage	client_addr;	//almacenar dirección cliente
		socklen_t				addr_size = sizeof(client_addr);		//tamaño del estruct client_addr;
		int	tmpFD;
		int	status;
		size_t	pollSize = _pollFDs.size();
		status = poll(_pollFDs.data(), _pollFDs.size(), 1000); //esperar 1 segs (ms) a cambios
		if (status == -1)
		{
			//Si error cerrar sockets y lanzar exepcion
			for (std::vector<pollfd>::iterator pollFDs = _pollFDs.begin(); pollFDs != _pollFDs.end(); pollFDs++)
			{
				close(pollFDs->fd);
			}
			throw errorPolling();
		}
		else
		{
			for (size_t i = 0; i < pollSize; i++)
			{
				if (!_isServerFD(_pollFDs[i].fd)
					&& _timeOuts[_pollFDs[i].fd] < time(NULL))
				{
					Response(HTTP_REQUEST_TIMEOUT, _pollFDs[i].fd);
					close(_pollFDs[i].fd);
					_clients.erase(_pollFDs[i].fd);
					_tmpRecv[_pollFDs[i].fd].erase();
					this->_pollFDs.erase(_pollFDs.begin() + i);
					i--;
					pollSize--;
				}
				if (_pollFDs[i].revents & POLLIN)
				{
					if (_isServerFD(_pollFDs[i].fd) && pollSize < MAXCLIENT)
					{
						//Si el socket con cambios es del servidor
						//aceptar conexión y generar socket cliente

						tmpFD = accept(_pollFDs[i].fd, (struct sockaddr *)&client_addr, &addr_size);
						if (tmpFD == -1 || fcntl(tmpFD,  F_SETFL, O_NONBLOCK) == -1)
						{
							std::cerr << "Error accepting client" << std::endl;
							continue ;
						}
						_timeOuts[tmpFD] = time(NULL) + TIMEOUT;
						_tmpRecv[tmpFD] = "";
						_clients[tmpFD] = _getServerbyFD(_pollFDs[i].fd);
						_pollFDs.push_back(_makeClientPoll(tmpFD));
					}
					else if (_isServerFD(_pollFDs[i].fd) && _pollFDs.size() >= MAXCLIENT)
					{
						std::cerr << "Maximo de clientes alcanzados" << std::endl;
					}
					else if (!_isServerFD(_pollFDs[i].fd) && pollSize < MAXCLIENT)
					{
						//Si no es el socket servidor, es un socket cliente, leer
						status = _readClient(_pollFDs[i], _clients[_pollFDs[i].fd]);
						if (status == -1)
						{
							close(_pollFDs[i].fd);
							_clients.erase(_pollFDs[i].fd);
							_tmpRecv[_pollFDs[i].fd].erase();
							this->_pollFDs.erase(_pollFDs.begin() + i);
							i--;
							pollSize--;
						}
						else if (status == 1)
						{
							_pollFDs[i].events = POLLOUT;
							_requests[_pollFDs[i].fd] = Request(_tmpRecv[_pollFDs[i].fd], _pollFDs[i].fd, *_clients[_pollFDs[i].fd]);
						}
					}
				}
				if (_pollFDs[i].revents & POLLOUT && !_isServerFD(_pollFDs[i].fd))
				{
					Response(_requests[_pollFDs[i].fd], _clients[_pollFDs[i].fd], _pollFDs[i].fd);
					_tmpRecv[_pollFDs[i].fd].erase();
					_closeClient(i, pollSize, _pollFDs[i]);
				}
				if ((_pollFDs[i].revents & POLLERR) || _pollFDs[i].revents & POLLHUP)
				{
					_closeClient(i, pollSize, _pollFDs[i]);
				}
			}
		}
	}
}

void	Cluster::_closeFDs()
{
	for (std::vector<Server>::iterator srv = _servers.begin(); srv != _servers.end(); srv++)
	{
		std::vector<int> FDS = srv->getFD();
		for (std::vector<int>::iterator fd = FDS.begin(); fd != FDS.end(); fd++)
		{
			close(*fd);
		}
	}
}

void	Cluster::_closeClient(size_t &i, size_t &pollSize, pollfd &client)
{
	close(client.fd);
	_clients.erase(client.fd);
	this->_pollFDs.erase(_pollFDs.begin() + i);
	_requests.erase(_pollFDs[i].fd);
	i--;
	pollSize--;
}

bool	Cluster::_isServerFD(int fdIn)
{
	for (std::vector<Server>::iterator srv = _servers.begin(); srv != _servers.end(); srv++)
	{
		std::vector<int> FDS = srv->getFD();
		for (std::vector<int>::iterator fd = FDS.begin(); fd != FDS.end(); fd++)
		{
			if (*fd == fdIn)
				return (true);
		}
	}
	return (false);
}

Server	*Cluster::_getServerbyFD(int fd)
{
	for (std::vector<Server>::iterator srv = _servers.begin(); srv != _servers.end(); srv++)
	{
		std::vector<int> fdList = srv->getFD();
		for (std::vector<int>::iterator itFD = fdList.begin(); itFD != fdList.end(); itFD++)
		{
			if (fd == *itFD)
				return (&(*srv));
		}
	}
	return (NULL);
}

pollfd	Cluster::_makeClientPoll(int fd)
{
	struct pollfd	pollFd;

	pollFd.fd = fd;
	pollFd.events = POLLIN; //asignar monitorizacion de lectura
	return (pollFd);
}

int	Cluster::_readClient(pollfd &client, Server *server)
{
	char		buff[BUFFSIZE];
	int			reads = 0;


	reads = recv(client.fd, buff, BUFFSIZE, 0);
	if (reads > 0)
	{
		_tmpRecv[client.fd].insert(_tmpRecv[client.fd].end(), buff, buff + reads);
		_timeOuts[client.fd] = time(NULL) + TIMEOUT;
	}
	if (reads == -1)
	{
		std::cerr << "Error reading from client" << std::endl;
		return (-1);
	}
	else
	{
		Request tmp(_tmpRecv[client.fd], client.fd, *server);
		/*if (server->isSetClientBSize() && tmp.getPayloadSize() > server->getClientBSize())
		{
			Response(HTTP_REQUEST_ENTITY_TOO_LARGE, client.fd);
			return (-1);
		}*/
		if (tmp.areHeader() && tmp.areBody())
		{
			return (1);
		}
	}
	return (0);
}

bool	Cluster::_isCompleteRequest(std::string req)
{
	Request tmp(req);

	if (tmp.getMethod() == "GET")
	{
		if (tmp.areHeader())
			return (true);
	}

	if (tmp.getMethod() == "POST" && tmp.getContentLength() == tmp.getBody().size())
	{
		return (true);
	}
	return (false);
}

void	Cluster::_response(pollfd &client)
{
	std::string toResponse = _makeResponse();
	const char *msg = toResponse.c_str();
	int msg_len = strlen(msg);
	int bytes_sent;

	std::cout << "Response: " << std::endl << msg << std::endl;

	bytes_sent = send(client.fd, msg, msg_len, 0);
	if (bytes_sent == -1)
	{
		std::cerr << "send error: " << strerror(errno) << std::endl;
	}
	else if (bytes_sent == msg_len)
	{
		std::cerr << std::endl << "Sent full message to client socket " << client.fd << ": "
		<< std::endl << std::endl;
	}
	else
	{
		std::cerr << "Sent partial message to client socket " << client.fd << ": " << bytes_sent
		<< " bytes sent" << std::endl;
	}
}

std::string	Cluster::_makeResponse()
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
	header += "Content-Type: text/html; charset=utf-8";
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

/* ************************************************************************** */
/*                             GETTERS / SETTERS                              */
/* ************************************************************************** */

std::vector<Server>	Cluster::getServers() const
{
	return (this->_servers);
}

/* ************************************************************************** */
/*                            NON MEMBER OVERLOAD                             */
/* ************************************************************************** */

std::ostream	&operator<<(std::ostream &o, Cluster const &i)
{
	size_t nbrs = 1;

	std::vector<Server> _servers = i.getServers();
	for (std::vector<Server>::iterator its = _servers.begin(); its != _servers.end(); its++)
	{
		o << std::endl << "********************************************" << std::endl << std::endl;
		o << "Running webserver on:" << std::endl; 
		o << "Server number " << nbrs << std::endl;
		o << "Server name: " << its->getServerName() << std::endl;
		o << "Host: " << its->getHost() << std::endl;
		o << "Listen port: " << std::endl;
		std::vector<std::string>	tmpL = its->getListen();
		for (std::vector<std::string>::iterator lis = tmpL.begin(); lis != tmpL.end(); lis++)
			o << *lis << std::endl;
		o << std::endl << "********************************************" << std::endl << std::endl;
		nbrs++;
	}
	return (o);
}
