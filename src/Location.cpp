#include "Location.hpp"

/* ************************************************************************** */
/*                            MEMBER EXCEPTIONS	                              */
/* ************************************************************************** */

class	Location::rootReAsing: public std::exception
{
	virtual const char	*what() const throw ()
	{
		return ("Error: reasignation of root directory in a same location");
	}
};

class	Location::badMethod: public std::exception
{
	virtual const char	*what() const throw ()
	{
		return ("Error: method not exist or implemented. Only GET, POST or DELETE");
	}
};

class	Location::indexDup: public std::exception
{
	virtual const char	*what() const throw ()
	{
		return ("Error: index param duplicated in a same location");
	}
};

class	Location::methodDup: public std::exception
{
	virtual const char	*what() const throw ()
	{
		return ("Error: method param duplicated in a same location");
	}
};

class	Location::returnDup: public std::exception
{
	virtual const char	*what() const throw ()
	{
		return ("Error: return param duplicated in a same location");
	}
};

class	Location::compilerDup: public std::exception
{
	virtual const char	*what() const throw ()
	{
		return ("Error: compiler param duplicated in a same location");
	}
};

/* ************************************************************************** */
/*                        CONSTRUCTORS / DESTRUCTORS                          */
/* ************************************************************************** */

Location::Location(): _autoindex(false)
{
	return ;
}

Location::Location(Location const &src)
{
	this->_location = src._location;
	this->_root = src._root;
	this->_autoindex = src._autoindex;
	this->_index = src._index;
	this->_methods = src._methods;
	this->_return = src._return;
	this->_compiler = src._compiler;
	return ;
}

Location::~Location()
{
	return ;
}

/* ************************************************************************** */
/*                            OPERATORS OVERLOAD                              */
/* ************************************************************************** */

Location	&Location::operator=(Location const &rhs)
{
	if (this != &rhs)
	{
		this->_location = rhs._location;
		this->_root = rhs._root;
		this->_autoindex = rhs._autoindex;
		this->_index = rhs._index;
		this->_methods = rhs._methods;
		this->_return = rhs._return;
		this->_compiler = rhs._compiler;
	}
	return (*this);
}

/* ************************************************************************** */
/*                           NON MEMBER FUNCTIONS                             */
/* ************************************************************************** */


/* ************************************************************************** */
/*                             MEMBER FUNCTIONS                               */
/* ************************************************************************** */

void	Location::clear()
{
	_location.clear();
	_root.clear();
	_autoindex = false;
	_index.clear();
	_methods.clear();
	_return.clear();
	_compiler.clear();
}

bool	Location::isComplet()
{
	if (!(this->_location.empty()))
	{
		if (!(this->_return.empty()))
			return (true);
		if (!(this->_methods.empty()) && !(this->_root.empty()))
			return (true);
	}
	return (false);
}

bool	Location::_checkDup(std::vector<std::string> methods)
{
	for (std::vector<std::string>::iterator it = methods.begin(); it != methods.end(); it++)
	{
		for (std::vector<std::string>::iterator jt = methods.begin(); jt != methods.end(); jt++)
	{
		if (it != jt)
		{
			if (*it == *jt)
				return (true);
		}
	}
	}
	return (false);
}

/* ************************************************************************** */
/*                             GETTERS / SETTERS                              */
/* ************************************************************************** */

void	Location::setLocation(std::string location)
{
	this->_location = location;
}

void	Location::setRoot(std::string root)
{
	if (_root.size() == 0)
		this->_root = root;
	else
		throw rootReAsing();
}

void	Location::setAutoIndex(bool autoIndex)
{
	this->_autoindex = autoIndex;
}

void	Location::setIndex(std::string index)
{
	this->_index = index;
}

void	Location::setMethods(std::vector<std::string> methods)
{
	if (_checkDup(methods))
		throw methodDup();
	else
		this->_methods = methods;
}

void	Location::setReturn(std::string ret)
{
	if (_return.size() == 0)
		this->_return = ret;
	else
		throw returnDup();
}

void	Location::setCompiler(std::string comp)
{
	if (_compiler.size() == 0)
		this->_compiler = comp;
	else
		throw compilerDup();
}

std::string	Location::getLocation() const
{
	return (this->_location);
}

std::string	Location::getRoot() const
{
	return (this->_root);
}

bool		Location::getAutoIndex() const
{
	return (this->_autoindex);
}

std::string	Location::getIndex() const
{
	return (this->_index);
}

std::vector<std::string>	Location::getMethods() const
{
	return (this->_methods);
}

std::string	Location::getReturn() const
{
	return (this->_return);
}

std::string Location::getCompiler() const
{
	return (this->_compiler);
}

/* ************************************************************************** */
/*                            NON MEMBER OVERLOAD                             */
/* ************************************************************************** */

std::ostream	&operator<<(std::ostream &o, Location const &i)
{
	std::vector<std::string>				mth = i.getMethods();

	std::cout << "Location info:" << std::endl << std::endl;
	std::cout << "Location: " << i.getLocation() << std::endl;
	std::cout << "Root: " << i.getRoot() << std::endl;
	std::cout << "Autoindex = " << i.getAutoIndex() << std::endl;
	std::cout << "Index: " << i.getIndex() << std::endl;
	std::cout << "Methods:" << std::endl;
	for (std::vector<std::string>::iterator it = mth.begin(); it != mth.end(); it++)
		std::cout << *it << std::endl;
	std::cout << "Return: " << i.getReturn() << std::endl;
	std::cout << "Compiler: " << i.getCompiler() << std::endl;
	return (o);
}
