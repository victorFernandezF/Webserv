#ifndef RESPONSE_HPP
# define RESPONSE_HPP

# include <iostream>
# include <map>
# include <cstring>
# include <sys/socket.h>

# include <Server.hpp>
# include <dirent.h>
# include <Request.hpp>

# include <ft_utils.hpp>

# define HTTP_CONTINUE 100
# define HTTP_OK 200
# define HTTP_CREATED 201
# define HTTP_NO_CONTENT 204
# define HTTP_BAD_REQUEST 400
# define HTTP_UNAUTHORIZED 401
# define HTTP_FORBIDDEN 403
# define HTTP_NOT_FOUND 404
# define HTTP_METHOD_NOT_ALLOWED 405
# define HTTP_REQUEST_ENTITY_TOO_LARGE 413
# define HTTP_REQUEST_URI_TOO_LONG 414
# define HTTP_INTERNAL_SERVER_ERROR 500

class Response
{
	private:
			Request	_req;
			Server 	_srv;
			Location	_loc;
			int		_clientFD;

			Response();

			void	_exeResponse();

			bool	_isPathAFile(Request req);
			void	_isPathAccessible(Request req);
			std::string _autoindex(Request req);

			std::string	_getErrorPage(unsigned short nbr);
			std::string	_makeErrorPage(unsigned short nrb);
			std::string	_makeHtmlHead(std::string title);
			std::string _makeHtmlTail();
			std::string _makeErrorBody(unsigned short nbr);
			std::string	_getCodePageText(unsigned short nbr);

			std::string	_makeResponse(std::string body);
			std::string _makeResponseRedirect();
			void	_sendResponse(std::string msg);

			void	_getMethod();
			void	_postMethod();
			void	_deleteMethod();

			bool	_isLocation(std::string loc);
			bool	_isAllowedMethod();
			std::string _parsePathIndex();
			std::string	_parsePathUrl();
			std::string _getFile(std::string name);
			void	_takeFile();
			std::string	_getFileName();
			std::string	_cleanBoundary();
			void	_takeForm();

			void	_getMethodTemp();

	public:
		Response(Response const &src);
		Response(Request req, Server *srv, int client);
		~Response();
		Response &operator=(Response const &rhs);

		Request getRequest() const;
		Server	getServer() const;
		int		getClientFD() const;
};

std::ostream	&operator<<(std::ostream &o, Response const &i);

#endif
