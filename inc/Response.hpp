#ifndef RESponse_HPP
# define RESponse_HPP

# include <iostream>
# include <vector>
# include <map>
# include <fstream>
# include <sstream>

# include <Server.hpp>
# include "Request.hpp"

class Response
{
	private:
    	Response();

	public:
		int status;
		std::string path;



		Response(std::string req, int fd, Server srv);
		Response(Response const &src);
		Response &operator=(Response const &rhs);
		~Response();

        std::string checkMethod(Request &req);
		bool checkIfPathIsFile();
		void MakeResponse(Request &req);

		std::map<std::string, std::string>	getHeaderParams() const;
};

std::ostream	&operator<<(std::ostream &o, Response const &i);

#endif
