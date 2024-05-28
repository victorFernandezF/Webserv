#ifndef RESPONSE_HPP
# define RESPONSE_HPP

# include <iostream>

# include <Server.hpp>
# include <Request.hpp>

# define HTTP_CONTINUE 100
# define HTTP_OK 200
# define HTTP_CREATED 201
# define HTTP_NO_CONTENT 204
# define HTTP_BAD_REQUEST 400
# define HTTP_UNAUTHORIZED 401
# define HTTP_FORBIDDEN 403
# define HTTP_NOT_FOUND 404
# define HTTP_METHOD_NOT_ALLOWED 405
# define HTTP_PAYLOAD_TOO_LARGE 413
//# define HTTP_URI_TOO_LONG 414 ????
# define HTTP_INTERNAL_SERVER_ERROR 500

class Response
{
	private:
			Request	_req;
			Server 	_srv;
			int		_clientFD;

			Response();

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
