#ifndef CONFIG_HPP
# define CONFIG_HPP

# include <iostream>
# include <fstream>
# include <sstream>
# include <vector>

# include "ft_utils.hpp"
# include "Server.hpp"
# include "Cluster.hpp"

class Config
{
	private:
		std::string					_file;
		std::vector<std::string>	_fLines;
		std::vector<Server>			_servers;

		void	_fillCLines();
		bool	_isEmptyLine(std::string str);
		void	_checknBraces();
		void	_serverTag(std::string str);
		void	_makeServers();
		size_t	_beginWord(std::string const line);
		std::string	_takeKey(std::string &line);
		void	_checkQuotes(std::string str);
		void	_addServerParam(Server &srv, Location &location, std::string key, std::string &val);
		void	_addLocationHead(Location &location, std::string &val);
		void	_addLocationParam(Location &location, std::string key, std::string &val);

		std::string	_parseVal(std::string &val);
		std::vector<std::string>	_parseListen(std::string &val);
		unsigned int	_parseClientBodySize(std::string &val);
		std::vector<std::string> _parseMethods(std::string &val);
		std::map<unsigned short, std::string> _parseErrorPages(std::string &val);

		void	_checkServers();

		void	_printServers();

	public:
		Config(std::string file, Cluster &cluster);
		Config(Config const &src);
		Config &operator=(Config const &rhs);
		~Config();

		class errorOpen;
		class errorBraces;
		class errorServerTag;
		class errorParam;
		class errorQuotes;
		class errorUnkownParam;
		class errorListen;
		class errorClientBodySize;
		class errorParamMethod;
		class errorMethod;
		class errorUnpairErrorPage;
		class errorCodePageError;
		class errorReAssingHost;
		class errorNotValidHost;
		class errorReAssingListen;
		class errorReAssingRoot;
		class errorReAssingClientBodySize;
		class errorReAssingUploadPath;
		class errorReAssingErrorPages;
		class errorReAssingServerName;
		class errorReAssingIndex;
		class errorInCompletLocationSrv;
		class errorNoLocationRoute;
		class errorReAssingReturn;
		class errorReAssingAutoIndex;
		class errorReAssingCompiler;
		class errorServerNameDup;
		class errorListenDup;
		class errorNoLocationNoOpened;
};


std::ostream	&operator<<(std::ostream &o, Config const &i);

#endif