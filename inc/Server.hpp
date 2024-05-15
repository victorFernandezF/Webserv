#ifndef SERVER_HPP
# define SERVER_HPP

# include <iostream>
# include <vector>
# include <map>

# include "Location.hpp"

class Server
{
	private:
		std::string				_host;
		std::vector<std::string>			_listen;
		bool					_listenSet;
		std::string				_server_name;
		unsigned int			_client_body_size;
		bool					_client_body_sizeSet;
		std::string				_upload_path;
		std::map<unsigned short, std::string>				_error_page;

		std::vector<int>		_fd;

		std::vector<Location>	_locations;

	public:
		Server();
		Server(Server const &src);
		Server &operator=(Server const &rhs);
		~Server();

		void	setHost(std::string host);
		void	setListen(std::vector<std::string> listen);
		void	setServerName(std::string sName);
		void	setClientBSize(unsigned int clientBSize);
		void	setUploadPath(std::string path);
		void	setErrorPage(unsigned short key, std::string value);
		void	setErrorPageMap(std::map<unsigned short, std::string> map);
		void	setFD(int fd);
		void	addLocation(Location location);

		std::string	getHost();
		std::vector<std::string> const	&getListen();
		bool	isSetListen();
		std::string getServerName();
		unsigned int	getClientBSize();
		bool	isSetClientBSize();
		std::string	getUploadPath();
		std::map<unsigned short, std::string> getErrorPage(unsigned short nError);
		std::map<unsigned short, std::string> getErrorPageMap();
		std::vector<int> const	&getFD();
		std::vector<Location>	getLocations();

		void	clear();
};

std::ostream	&operator<<(std::ostream &o, Server const &i);

#endif