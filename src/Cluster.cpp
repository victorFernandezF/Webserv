#include "Cluster.hpp"

/* ************************************************************************** */
/*                             MEMBER EXCEPTIONS                               */
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
	*this = src;
	return ;
}

Cluster::~Cluster()
{
	return ;
}

/* ************************************************************************** */
/*                            OPERATORS OVERLOAD                              */
/* ************************************************************************** */

Cluster	&Cluster::operator=(Cluster const &rhs)
{
	(void)rhs;
	return (*this);
}

/* ************************************************************************** */
/*                           NON MEMBER FUNCTIONS                             */
/* ************************************************************************** */

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

int		Cluster::_startSocket(Server &Server, std::string port)
{
	(void)Server;
	struct addrinfo			hints;			//para las flags
	struct addrinfo			*result;		//almacenar resultados
	int						fd;
	int						status;

	//Rellenar datos para el socket
	bzero(&hints, sizeof(hints));	//Todo a 0
	hints.ai_family = AF_UNSPEC; 		//Permitir IPv4 o IPv6
	hints.ai_socktype = SOCK_STREAM;	//Protocolo TCP
	hints.ai_flags = AI_PASSIVE;		//Indica socket para la escucha, dirección IP local


	//Necesario cambiar AI_PASSIVE a la dirección del archivo de configuración!!!!!


	status = getaddrinfo(NULL, port.c_str(), &hints, &result); //parseo y relleno de result
	if (status != 0)
		throw errorParseListenPort();

	fd = socket(result->ai_family, result->ai_socktype, result->ai_protocol); //Generar FD de socket
	status = bind(fd, result->ai_addr, result->ai_addrlen);	//Asigna dirección local para la escucha
	if (status != 0)
	{
		std::cout << "[Server] Bind error: [" << status << "] -> " << strerror(errno) << std::endl;
		throw errorBindSocket();
	}



	//Añadir control de errores y no utilar errno!!!!



	status = listen(fd, MAXCLIENT);				//Escucha en el fd del socket con cola de 20
	if (status != 0)
		throw errorListenSocket();
	status = fcntl(fd, F_SETFL, O_NONBLOCK);	//Establece el fd obejetivo en no bloqueante
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
			pollFd.events = POLLIN; //cambio a monitorizar de lectura
			this->_pollFDs.push_back(pollFd);
		}
	}
}

void	Cluster::runServers()
{

	//Añadir control de errores y no utilar errno!!!!
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
		//int	pollSize = this->_pollFDs.size();
		status = poll(_pollFDs.data(), _pollFDs.size(), 1000); //esperar 1 segs (ms) a cambios
		if (status == -1)
		{
			//Si error cerrar sockets y lanzar exepcion
			for (std::vector<pollfd>::iterator pollFDs = _pollFDs.begin(); pollFDs != _pollFDs.end(); pollFDs++)
			{
				std::cout << "Cerrando FD " << pollFDs->fd << std::endl;
				close(pollFDs->fd);
			}
			throw errorPolling();
		}
		else
		{
			if (status == 0)
			{
			//si devulve 0, poll correcto pero sin cambios
			std::cout << "Esperando conexiones..." << std::endl;
			continue ;
			}
			for (size_t i = 0; i < pollSize; i++)
			{
				if ((_pollFDs[i].revents & POLLIN) != 1)
					//operador de bits sobre revents y POLLIN
					//si no concide, comprueba siguiente socket
					continue ;
				if (_isServerFD(_pollFDs[i].fd) && pollSize < MAXCLIENT)
				{
					//Si el socket con cambios es del servidor
					//aceptar conexión y generar socket cliente

					tmpFD = accept(_pollFDs[i].fd, (struct sockaddr *)&client_addr, &addr_size);
					if (tmpFD == -1 || fcntl(tmpFD,  F_SETFL, O_NONBLOCK) == -1)
					{
						std::cerr << "Error generar socket cliente: " << strerror(errno) << std::endl;
						continue ;
					}
					std::cout << "Cliente conectado en fd: " << tmpFD << std::endl;
					_clients[tmpFD] = _getServerbyFD(_pollFDs[i].fd);
					_pollFDs.push_back(_makeClientPoll(tmpFD));
				}
				else if (_isServerFD(_pollFDs[i].fd) && _pollFDs.size() >= MAXCLIENT)
				{
					std::cerr << "Maximo de clientes alcanzados" << std::endl;
				}
				else
				{
					//Si no es el socket servidor, es un socket cliente, leer
					_readClient(i, pollSize, _pollFDs[i], _clients[_pollFDs[i].fd]);
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
	pollFd.events = POLLIN; //cambio a monitorizar de lectura
	return (pollFd);
}

void	Cluster::_readClient(size_t &i, size_t &pollSize, pollfd &client, Server *server)
{
	std::cout << "Recibiendo datos de cliente en fd: " << client.fd << ", de servidor: "
	<< server->getServerName() << std::endl;

	char	buff[BUFFSIZE];
	int		reads = recv(client.fd, buff, BUFFSIZE, 0);
	if (reads >= 0)
	{
		if (reads == 0)
		{
			std::cout << "Conexión cerrada de cliente en fd (0 reads): " << client.fd << std::endl;
		}
		else
		{


			//Respuesta futura tras parseo de peticion



			buff[reads] = '\0';
			std::cout << std::endl << buff << std::endl;

			Request req(buff, client.fd, *server);

			std::cout << std::endl << req << std::endl;

			std::string toReponse = "Recibido. Corto y cambio\n*****************************\n\0";
			const char *msg = toReponse.c_str();
            int msg_len = strlen(msg);
            int bytes_sent;

			bytes_sent = send(client.fd, msg, msg_len, 0);
            if (bytes_sent == -1)
			{
				std::cerr << "send error: " << strerror(errno) << std::endl;
            }
            else if (bytes_sent == msg_len)
			{
				std::cerr << "Sent full message to client socket " << client.fd << ": " << msg << std::endl;
            }
            else 
			{
				std::cerr << "Sent partial message to client socket " << client.fd << ": " << bytes_sent
				<< " bytes sent" << std::endl;
            }
		}
	}
	else if (reads == -1)
	{
		std::cerr << "recv error: " << strerror(errno) << std::endl;
	}
	std::cout << "Cerrando transmision con cliente " << client.fd << std::endl;
	close(client.fd);
	_clients.erase(client.fd);
	this->_pollFDs.erase(_pollFDs.begin() + i);
	i--;
	pollSize--;
	exit (1);
}

/* ************************************************************************** */
/*                             MEMBER FUNCTIONS                               */
/* ************************************************************************** */

void	Cluster::addServer(Server srv)
{
	this->_servers.push_back(srv);
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
