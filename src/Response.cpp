#include "Response.hpp"

/* ************************************************************************** */
/*                             MEMBER EXCEPTIONS                               */
/* ************************************************************************** */



/* ************************************************************************** */
/*                        CONSTRUCTORS / DESTRUCTORS                          */
/* ************************************************************************** */

Response::Response()
{
	return ;
}

Response::Response(std::string req, int fd, Server srv)
{

}

Response::Response(Response const &src)
{
	*this = src;
	return ;
}

Response::~Response()
{
	return ;
}

/* ************************************************************************** */
/*                            OPERATORS OVERLOAD                              */
/* ************************************************************************** */

Response	&Response::operator=(Response const &rhs)
{
	(void)rhs;
	return (*this);
}

/* ************************************************************************** */
/*                           NON MEMBER FUNCTIONS                             */
/* ************************************************************************** */

std::string checkMethod(Request &req){
    std::string method = req.getMethod();
    if (method == "POST" || method == "GET" || method == "DELETE")
        return method;
    return "METHOD NOT ALLOWED";
}

void MakeResponse(Request &req){
    
}

/* ************************************************************************** */
/*                             MEMBER FUNCTIONS                               */
/* ************************************************************************** */


bool Response::checkIfPathIsFile(){
	int position;
	position = path.find(".");
	if (position == std::string::npos)
		return false;
	return true;
}




/*void	Response::_parseResponse(std::string req)
{
	size_t pos;

	if (req.find("\r\n") != std::string::npos)
	{
		pos = req.find("\r\n");
		_firstLine(req.substr(0, pos));
		req.erase(0, pos + 2);
	}
	//std::cout << std::endl << "Post first line: " << req << std::endl << std::endl;
	if (req.find("\r\n\r\n") != std::string::npos)
	{
		pos = req.find("\r\n\r\n");
		_setHeader(req.substr(0, pos));
		_setHeaderParams(req.substr(0, pos));
		req.erase(0, pos + 4);
	}
	if (req.find("\r\n\r\n") != std::string::npos)
	{
		_setBody(req);
	}
}*/



/* ************************************************************************** */
/*                             GETTERS / SETTERS                              */
/* ************************************************************************** */


/* ************************************************************************** */
/*                            NON MEMBER OVERLOAD                             */
/* ************************************************************************** */

std::ostream	&operator<<(std::ostream &o, Response const &i)
{
	
}
