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

class	Location::binReAsing: public std::exception
{
	virtual const char	*what() const throw ()
	{
		return ("Error: reasignation of binary cgi in a location");
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
	this->_bin = src._bin;
	this->_return = src._return;
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
		this->_bin = rhs._bin;
		this->_return = rhs._return;
	}
	return (*this);
}

/* ************************************************************************** */
/*                           NON MEMBER FUNCTIONS                             */
/* ************************************************************************** */

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
/*                             MEMBER FUNCTIONS                               */
/* ************************************************************************** */

void	Location::clear()
{
	_location.clear();
	_root.clear();
	_autoindex = false;
	_index.clear();
	_methods.clear();
	_bin.clear();
	_return.clear();
	_compiler.clear();
}

bool	Location::isComplet()
{
	if ((!(this->_return.empty()) && !(this->_location.empty()))
		|| (!(this->_location.empty()) && !(this->_root.empty())
		&& (!(this->_index.empty()) || this->_autoindex == true || !(this->_compiler.empty()))
		&& !(this->_methods.empty())))
		return (true);
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

void	Location::setBin(std::string bin)
{
	if (_bin.size() == 0)
		this->_bin = bin;
	else
		throw binReAsing();
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

std::string	Location::getLocation()
{
	return (this->_location);
}

std::string	Location::getRoot()
{
	return (this->_root);
}

bool		Location::getAutoIndex()
{
	return (this->_autoindex);
}

std::string	Location::getIndex()
{
	return (this->_index);
}

std::vector<std::string>	Location::getMethods()
{
	return (this->_methods);
}

std::string	Location::getBin()
{
	return (this->_bin);
}

std::string	Location::getReturn()
{
	return (this->_return);
}

std::string Location::getCompiler()
{
	return (this->_compiler);
}

/* ************************************************************************** */
/*                            NON MEMBER OVERLOAD                             */
/* ************************************************************************** */

std::ostream	&operator<<(std::ostream &o, Location const &i)
{
	(void)i;
	return (o);
}
