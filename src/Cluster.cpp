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

class	Cluster::errorBindSocket : public std::exception
{
	public:
		virtual const char *what(void) const throw()
		{
			return ("Error: bind socket error");
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

	//Rellenar datos para el socket
	bzero(&sa, sizeof(sa));	//Todo a 0
	sa.sin_family = AF_INET; 		//Permitir IPv4 o IPv6
	sa.sin_addr.s_addr = inet_addr(server.getHost().c_str());		//Indica socket para la escucha, dirección IP local
	sa.sin_port = htons(ft_atoiUnInt(port));

	fd = socket(sa.sin_family, SOCK_STREAM, 0); //Generar FD de socket
	status = bind(fd, (struct sockaddr *)&sa, sizeof sa);	//Asigna dirección local para la escucha
	if (status != 0)
	{
		std::cout << "[Server] Bind error: [" << status << "] -> " << strerror(errno) << std::endl;
		throw errorBindSocket();
	}
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
	//Revisar tema de MAXCLIENTS


	_startListen();
	_makeServerPolls();

	std::cout << std::endl << "Poll's" << std::endl << std::endl;

	for (std::vector<pollfd>::iterator it = this->_pollFDs.begin(); it != this->_pollFDs.end(); it++)
	{
		std::cout << "fd: " << it->fd << std::endl;
		std::cout << "events: " << it->events << std::endl;
		std::cout << "revents: " << it->revents << std::endl;
	}
	std::cout << std::endl;

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
			/*if (status == 0)
			{
			//si devulve 0, poll correcto pero sin cambios
			std::cout << "Esperando conexiones..." << std::endl;
			std::cout << time(NULL) << std::endl;
			continue ;
			}*/
			for (size_t i = 0; i < pollSize; i++)
			{
				/*std::cout << "·························" << std::endl;
				std::cout << "PollSize: " << pollSize << std::endl;
				std::cout << "PollObject" << std::endl;
				std::cout << "Fd: " << _pollFDs[i].fd << std::endl;
				std::cout << "Events: " << _pollFDs[i].events << std::endl;
				std::cout << "Revents: " << _pollFDs[i]. revents << std::endl;
				std::cout << "·························" << std::endl;*/
				/*if (!_isServerFD(_pollFDs[i].fd))
				{
					std::cout << "Now time: " << std::clock() << " - Init time: " << _timeOuts[_pollFDs[i].fd] << std::endl;
					std::cout << "Time: " << (( std::clock() - _timeOuts[_pollFDs[i].fd] ) / (double)(CLOCKS_PER_SEC * 1000)) << std::endl;
				}*/
				if (!_isServerFD(_pollFDs[i].fd)
					&& _timeOuts[_pollFDs[i].fd] < time(NULL))
				//	&&((( std::clock() - _timeOuts[_pollFDs[i].fd] ) / (double)(CLOCKS_PER_SEC * 1000)) > 500))
				{
					std::cout << "Time out fd " << _pollFDs[i].fd << std::endl;
					std::cout << "Time conexion " << _timeOuts[_pollFDs[i].fd] << " - Time exit: " << time(NULL) << std::endl;
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
						std::cout << "Cliente conectado en fd: " << tmpFD << std::endl;
						//_timeOuts[tmpFD] = std::clock();
						_timeOuts[tmpFD] = time(NULL) + TIMEOUT;
						std::cout << "Init time: " << _timeOuts[tmpFD] << std::endl;
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
						//_readClient(i, pollSize, _pollFDs[i], _clients[_pollFDs[i].fd]);
						status = _readClient(i, pollSize, _pollFDs[i], _clients[_pollFDs[i].fd]);
						if (status == -1)
						{
							close(_pollFDs[i].fd);
							_clients.erase(_pollFDs[i].fd);
							_tmpRecv[_pollFDs[i].fd].erase();
							this->_pollFDs.erase(_pollFDs.begin() + i);
							i--;
							pollSize--;
							//_timeOuts[_pollFDs[i].fd].erase();
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
					std::cout << Response(_requests[_pollFDs[i].fd], _clients[_pollFDs[i].fd], _pollFDs[i].fd) << std::endl;
					//_response(_pollFDs[i]);
					_tmpRecv[_pollFDs[i].fd].erase();
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
			std::cout << "Cerrando FD " << *fd << std::endl;
			close(*fd);
		}
	}
}

void	Cluster::_closeClient(size_t &i, size_t &pollSize, pollfd &client)
{
	std::cout << "Cerrando transmision con cliente " << client.fd << std::endl;
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

/*void	Cluster::_readClient(size_t &i, size_t &pollSize, pollfd &client, Server *server)
{
	std::cout << "Recibiendo datos de cliente en fd: " << client.fd << ", de servidor: "
	<< server->getServerName() << std::endl;

	char		buff[BUFFSIZE];
	//std::string	tmp;
	int			reads = 0;
	size_t		loop = 0;

	while (1)
	{
		loop++;
		reads = recv(client.fd, buff, BUFFSIZE, 0);
		if (reads == -1 && loop == 1)
		{
			close(client.fd);
			_clients.erase(client.fd);
			this->_pollFDs.erase(_pollFDs.begin() + i);
			i--;
			pollSize--;
			std::cout << "Error reading from client" << std::endl;
			return ;
		}
		if (!(reads >= 0))
		{
			std::cout << "Conexión completa de cliente en fd: " << client.fd << std::endl;
			std::cout << "Última linea: " << std::endl << "~~~~~~~~~~~" << buff << std::endl << "~~~~~~~~~~~" << std::endl;
			_tmpRecv[client.fd].insert(_tmpRecv[client.fd].end(), '\0', 1);
			client.events = POLLOUT;
			_requests[client.fd] = Request(_tmpRecv[client.fd], client.fd, *server);
			std::cout << Request(_tmpRecv[client.fd], client.fd, *server) << std::endl;
 			return ;
		}
		if (_isCompleteRequest(tmp))
		{
			std::cout << "Conexión completa de cliente en fd: " << client.fd << std::endl;
			tmp.insert(tmp.end(), '\0', 1);
			client.events = POLLOUT;
			_requests[client.fd] = Request(tmp, client.fd, *server);
			std::cout << Request(tmp, client.fd, *server) << std::endl;
			return ;
		}
		else
		{
			_tmpRecv[client.fd].insert(_tmpRecv[client.fd].end(), buff, buff + reads);
		}
	}
}*/

int	Cluster::_readClient(size_t &i, size_t &pollSize, pollfd &client, Server *server)
{
	(void)pollSize;
	(void)i;
	std::cout << "Recibiendo datos de cliente en fd: " << client.fd << ", de servidor: "
	<< server->getServerName() << std::endl;

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
		//close(client.fd);
		//_clients.erase(client.fd);
		//this->_pollFDs.erase(_pollFDs.begin() + i);
		//i--;
		//pollSize--;
		std::cout << "Error reading from client" << std::endl;
		return (-1);
	}
	else
	{
		Request tmp(_tmpRecv[client.fd], client.fd, *server);
		if (tmp.areHeader() && tmp.areBody())
		{
			std::cout << "Conexión " << client.fd << " , Is complete" << std::endl;
		/*std::cout << "Conexión completa de cliente en fd: " << client.fd << std::endl;
		_tmpRecv[client.fd].insert(_tmpRecv[client.fd].end(), '\0', 1);
		client.events = POLLOUT;
		_requests[client.fd] = Request(_tmpRecv[client.fd], client.fd, *server);
		std::cout << Request(_tmpRecv[client.fd], client.fd, *server) << std::endl;*/
			return (1);
		}
		//std::cout << "Conexión " << client.fd << " , NOT complete ELSE" << std::endl;
	}
	//std::cout << "Conexión " << client.fd << " , NOT complete OUT" << std::endl;
	return (0);
	/*if (_isCompleteRequest(tmp))
	{
		std::cout << "Conexión completa de cliente en fd: " << client.fd << std::endl;
		tmp.insert(tmp.end(), '\0', 1);
		client.events = POLLOUT;
		_requests[client.fd] = Request(tmp, client.fd, *server);
		std::cout << Request(tmp, client.fd, *server) << std::endl;
		return ;
	}*/
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
	/*std::cout << "Cerrando transmision con cliente " << client.fd << std::endl;
	close(client.fd);
	_clients.erase(client.fd);
	this->_pollFDs.erase(_pollFDs.begin() + i);
	_requests.erase(_pollFDs[i].fd);
	i--;
	pollSize--;*/
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
		size_t nbrl = 0;
		o << std::endl << "********************************************" << std::endl << std::endl;
		o << "Server number " << nbrs << std::endl;
		o << "Server name: " << its->getServerName() << std::endl;
		o << "Host: " << its->getHost() << std::endl;
		o << "Listen: " << std::endl;
		std::vector<std::string>	tmpL = its->getListen();
		for (std::vector<std::string>::iterator lis = tmpL.begin(); lis != tmpL.end(); lis++)
			o << *lis << std::endl;
		o << "File descriptor: " << std::endl;
		std::vector<int> tmpFD = its->getFD();
		for (std::vector<int>::iterator ifd = tmpFD.begin(); ifd != tmpFD.end(); ifd++)
			o << *ifd << std::endl;
		o << "Client body size: " << its->getClientBSize() << std::endl;
		o << "Upload path: " << its->getUploadPath() << std::endl;
		o << "Error pages:" << std::endl;
		std::map<unsigned short, std::string> tmpPages = its->getErrorPageMap();
		for (std::map<unsigned short, std::string>::iterator ite = tmpPages.begin(); ite != tmpPages.end(); ite++)
			o << ite->first << " -> " << ite->second << std::endl;
		o << std::endl << "Locations:" << std::endl;
		std::vector<Location> tmpLocation = its->getLocations();
		for (std::vector<Location>::iterator itl = tmpLocation.begin(); itl != tmpLocation.end(); itl++)
		{
			o << std::endl;
			o << "Location number " << nbrl << std::endl;
			o << "Location: " << itl->getLocation() << std::endl;
			o << "Return: " << itl->getReturn() << std::endl;
			o << "Root: " << itl->getRoot() << std::endl;
			o << "Autoindex: " << itl->getAutoIndex() << std::endl;
			o << "Index: " << itl->getIndex() << std::endl;
			o << "Methods:" << std::endl;
			std::vector<std::string> tmpMeth = itl->getMethods();
			for (std::vector<std::string>::iterator itm = tmpMeth.begin(); itm != tmpMeth.end(); itm++)
				o << *itm << std::endl;
			o << "Compiler: " << itl->getCompiler() << std::endl;
			nbrl++;
		}
		o << "********************************************" << std::endl << std::endl;
		nbrs++;
	}
	return (o);
}
