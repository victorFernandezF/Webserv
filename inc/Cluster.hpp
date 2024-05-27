#ifndef CLUSTER_HPP
# define CLUSTER_HPP

# include <stdlib.h>
# include <stdio.h>

# include <iostream>
# include <vector>
# include <map>
# include <cstring>
# include <unistd.h>
# include <netdb.h>
# include <arpa/inet.h>
# include <netinet/in.h>
# include <sys/socket.h>
# include <sys/types.h>
# include <fcntl.h>
# include <poll.h>

# include <cerrno>

# include "Server.hpp"
# include "Request.hpp"
# include "ft_utils.hpp"

# define MAXCLIENT 20
# define BUFFSIZE 2048

class Cluster
{
	private:
		std::vector<Server>		_servers;
		std::vector<pollfd>		_pollFDs;
		std::map<int, Server *>	_clients;
		std::map<int, Request> _requests;

		void	_startListen();
		int		_startSocket(Server &server, std::string port);
		
		void	_makeServerPolls();
		bool	_isServerFD(int fd);
		pollfd	_makeClientPoll(int fd);
		Server	*_getServerbyFD(int fd);
		void	_readClient(size_t &i, size_t &pollSize, pollfd &client, Server *server);

		void	_response(size_t &i, size_t &pollSize, pollfd &client);
		std::string	_makeResponse();

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