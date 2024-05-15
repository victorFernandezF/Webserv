#ifndef CLUSTER_HPP
# define CLUSTER_HPP

#include <iostream>
#include <vector>
#include <map>
#include <cstring>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <fcntl.h>
#include <poll.h>

#include <cerrno>

#include "Server.hpp"
#include "ft_utils.hpp"

#define MAXCLIENT 20
#define BUFFSIZE 2048

class Cluster
{
	private:
		std::vector<Server>		_servers;
		std::vector<pollfd>		_pollFDs;
		std::map<int, Server *>	_clients;

		void	_startListen();
		int		_startSocket(Server &Server, std::string port);
		
		void	_makeServerPolls();
		bool	_isServerFD(int fd);
		pollfd	_makeClientPoll(int fd);
		Server	*_getServerbyFD(int fd);
		void	_readClient(size_t &i, size_t &pollSize, pollfd &client, Server *server);

	public:
		Cluster();
		Cluster(Cluster const &src);
		Cluster &operator=(Cluster const &rhs);
		~Cluster();

		void	addServer(Server srv);
		std::vector<Server> getServers() const;
		void	runServers();

		void	_closeFDs();
		

		class errorParseListenPort;
		class errorBindSocket;
		class errorListenSocket;
		class errorNonBlockFD;
		class errorPolling;
};

std::ostream	&operator<<(std::ostream &o, Cluster const &i);

#endif