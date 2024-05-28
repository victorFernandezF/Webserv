#ifndef LOCATION_HPP
# define LOCATION_HPP

# include <iostream>
# include <vector>

class Location
{
	private:
		std::string					_location;
		std::string					_root;
		bool						_autoindex;
		std::string					_index;
		std::vector<std::string>	_methods;
		std::string					_return;
		std::string					_compiler;

		bool	_checkDup(std::vector<std::string> methods);

	public:
		Location();
		Location(Location const &src);
		Location &operator=(Location const &rhs);
		~Location();

		void	setLocation(std::string location);
		void	setRoot(std::string root);
		void	setAutoIndex(bool autoIndex);
		void	setIndex(std::string index);
		void	setMethods(std::vector<std::string> methods);
		void	setReturn(std::string ret);
		void	setCompiler(std::string comp);

		std::string	getLocation() const;
		std::string	getRoot() const;
		bool		getAutoIndex() const;
		std::string	getIndex() const;
		std::vector<std::string>	getMethods() const;
		std::string	getReturn() const;
		std::string getCompiler() const;

		void	clear();
		bool	isComplet();
		
		class	rootReAsing;
		class	badMethod;
		class	indexDup;
		class	methodDup;
		class	returnDup;
		class	compilerDup;
};

std::ostream	&operator<<(std::ostream &o, Location const &i);

#endif