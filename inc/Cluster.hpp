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
# include <ctime>

# include <cerrno>

# include "Server.hpp"
# include "Request.hpp"
# include "Response.hpp"
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
		std::map<int, std::string>	_tmpRecv;
		std::map<int, std::clock_t> _timeOuts;

		void	_startListen();
		int		_startSocket(Server &server, std::string port);
		
		void	_makeServerPolls();
		bool	_isServerFD(int fd);
		pollfd	_makeClientPoll(int fd);
		Server	*_getServerbyFD(int fd);
		//void	_readClient(size_t &i, size_t &pollSize, pollfd &client, Server *server);
		int	_readClient(size_t &i, size_t &pollSize, pollfd &client, Server *server);
		bool	_isCompleteRequest(std::string req);

		void	_response(pollfd &client);
		std::string	_makeResponse();
		void	_closeClient(size_t &i, size_t &pollSize, pollfd &client);

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