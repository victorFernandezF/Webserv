#ifndef REQUEST_HPP
# define REQUEST_HPP

# include <iostream>
# include <vector>
# include <map>
# include <fstream>
# include <sstream>

# include <Server.hpp>
# include <ft_utils.hpp>

class Request
{
	private:
		int				_clientFD;
		Server			_Server;

		std::string		_path;
		std::string		_method;
		std::string		_version;
		std::string		_header;
		std::string		_body;
		std::map<std::string, std::string>	_headerParams;
		std::map<std::string, std::string>	_varUrl;
		bool			_isHeader;

		void	_parseRequest(std::string req);
		void	_firstLine(std::string str);
		void	_urlSetVar();

		void	_setPath(std::string path);
		void	_setMethod(std::string method);
		void	_setVersion(std::string version);
		void	_setHeader(std::string header);
		void	_setBody(std::string body);
		void	_setHeaderParams(std::string header);

	public:
		Request();
		Request(std::string req);
		Request(std::string req, int fd, Server srv);
		Request(Request const &src);
		Request &operator=(Request const &rhs);
		~Request();

		std::string getPath() const;
		std::string getMethod() const;
		std::string getVersion() const;
		std::string getBody() const;
		std::string getHeader() const;
		std::map<std::string, std::string>	getHeaderParams() const;
		std::map<std::string, std::string>	getVarUrl() const;
		bool	areHeader();
		bool	areBody();
		size_t	getContentLength() const;
		std::string getContentType() const;
		std::string getBoundary() const;
		std::string	getExpect() const;
		unsigned int	getPayloadSize();
		void	setVarsUrl(std::map<std::string, std::string> vars);
		void	delHeaderParam(std::string ref);
};

std::ostream	&operator<<(std::ostream &o, Request const &i);

#endif
