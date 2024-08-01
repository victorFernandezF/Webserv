/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_utils.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fortega- <fortega-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/21 12:30:32 by fortega-          #+#    #+#             */
/*   Updated: 2024/07/30 08:37:40 by fortega-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_utils.hpp"

unsigned short	error_codes[] = {400, 401, 402, 403, 404, 405, 406, 407, 408, 409,
								410, 411, 412, 413, 414, 415, 416, 417, 421, 422,
								426, 500, 501, 503, 504, 505};

bool	ft_isBeginStr(std::string reqPath, std::string locPath)
{
	size_t i = 0;

	if (reqPath.size() >= locPath.size())
	{
		while (i < locPath.size())
		{
			if (reqPath[i] != locPath[i])
				return (false);
			i++;
		}
		return (true);	
	}
	else
	{
		return (false);
	}
}

std::string	ft_itoa(int nbr)
{
	std::string	str;
	std::string	tmp;
	
	if (nbr < 0)
	{
		str += '-';
		nbr = -nbr;
	}
	if (nbr == 0)
	{
		str += '0';
	}
	while (nbr > 0)
	{
		tmp = (nbr % 10) + '0';
		str = tmp + str;
		nbr = nbr / 10;
	}
	return (str);
}

int		ft_atoi(std::string const str)
{
	int	i = 0;
	int	s = 1;
	int	total = 0;

	while ((str[i] > 8 && str[i] < 14) || str[i] == 32)
		i++;
	if (str[i] == '+' || str[i] == '-')
	{
		if (str[i] == '-')
			s = -s;
		i++;
	}
	while (str[i] >= '0' && str[i] <= '9')
	{
		total = total * 10 + (str[i] - '0');
		i++;
	}
	return (total * s);
}

unsigned int		ft_atoiUnInt(std::string const str)
{
	int	i = 0;
	unsigned int	total = 0;

	while ((str[i] > 8 && str[i] < 14) || str[i] == 32)
		i++;
	while (str[i] >= '0' && str[i] <= '9')
	{
		total = total * 10 + (str[i] - '0');
		i++;
	}
	return (total);
}

bool	ft_atoiUnShortCheck(std::string const std_str)
{	
	int		i = 0;
	long long	s = 1;
	long long	total = 0;
	const char	*str = std_str.c_str();

	while ((str[i] > 8 && str[i] < 14) || str[i] == 32)
		i++;
	if (str[i] == '+' || str[i] == '-' || (str[i] >= '0' && str[i] <= '9'))
		i++;
	else
		return (false);
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (false);
		i++;
	}
	
	i = 0;
	while ((str[i] > 8 && str[i] < 14) || str[i] == 32)
		i++;
	if (str[i] == '+' || str[i] == '-')
	{
		if (str[i] == '-')
			s = -s;
		i++;
	}
	while (str[i] >= '0' && str[i] <= '9')
	{
		total = total * 10 + (str[i] - '0');
		i++;
	}
	total = total * s;

	if (total < std::numeric_limits<unsigned short>::min() || total > std::numeric_limits<unsigned short>::max())
	{
		return (false);
	}
	else
		return (true);
}

bool	ft_atoiUnIntCheck(std::string const std_str)
{	
	int		i = 0;
	long long	s = 1;
	long long	total = 0;
	const char	*str = std_str.c_str();

	while ((str[i] > 8 && str[i] < 14) || str[i] == 32)
		i++;
	if (str[i] == '+' || str[i] == '-' || (str[i] >= '0' && str[i] <= '9'))
		i++;
	else
		return (false);
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (false);
		i++;
	}
	
	i = 0;
	while ((str[i] > 8 && str[i] < 14) || str[i] == 32)
		i++;
	if (str[i] == '+' || str[i] == '-')
	{
		if (str[i] == '-')
			s = -s;
		i++;
	}
	while (str[i] >= '0' && str[i] <= '9')
	{
		total = total * 10 + (str[i] - '0');
		i++;
	}
	total = total * s;

	if (total < std::numeric_limits<unsigned int>::min() || total > std::numeric_limits<unsigned int>::max())
	{
		return (false);
	}
	else
		return (true);
}

bool	ft_atoiErrorPageCheck(std::string const std_str)
{	
	int		i = 0;
	long long	s = 1;
	long long	total = 0;
	const char	*str = std_str.c_str();

	while ((str[i] > 8 && str[i] < 14) || str[i] == 32)
		i++;
	if (str[i] == '+' || str[i] == '-' || (str[i] >= '0' && str[i] <= '9'))
		i++;
	else
		return (false);
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (false);
		i++;
	}
	
	i = 0;
	while ((str[i] > 8 && str[i] < 14) || str[i] == 32)
		i++;
	if (str[i] == '+' || str[i] == '-')
	{
		if (str[i] == '-')
			s = -s;
		i++;
	}
	while (str[i] >= '0' && str[i] <= '9')
	{
		total = total * 10 + (str[i] - '0');
		i++;
	}
	total = total * s;

	for (size_t it = 0; it < 26; it++)
	{
		if (error_codes[it] == total)
			return (true);
	}
	return (false);
}


std::string str_tolower(std::string str)
{		

	for (size_t i = 0; i < str.size(); i++)
	{
		if (str[i] > 64 && str[i] < 91)
			str[i] = str[i] + 32;
	}

    return str;
}

char	replaceCodeHtml(std::string charCode)
{
	if (str_tolower(charCode) == "%20")
		return (c_20);
	if (str_tolower(charCode) == "%22")
		return (c_22);
	if (str_tolower(charCode) == "%3c")
		return (c_3C);
	if (str_tolower(charCode) == "%3e")
		return (c_3E);
	if (str_tolower(charCode) == "%23")
		return (c_23);
	if (str_tolower(charCode) == "%25")
		return (c_25);
	if (str_tolower(charCode) == "%7C")
		return (c_20);
	return (0);
}

std::string ft_decodeHtmlChars(std::string url)
{
	std::string ret;
	std::string	charCode;
	size_t	i = 0;
	
	while (i < url.size())
	{
		if (url[i] == '%')
		{
			charCode = url.substr(i, 3);
			ret += replaceCodeHtml(charCode);
			i += 3;
		}
		else
		{
			ret += url[i];
			i++;
		}
	}

	return (ret);
}

char	**makeMatArgs(std::map<std::string, std::string> vars)
{
	char 		**mat = new char*[vars.size() + 1];
	std::string val;
	size_t 		i = 0;
	size_t		j;

	for (std::map<std::string, std::string>::const_iterator it = vars.begin(); it != vars.end(); ++it)
	{
		val = it->first + "=" + it->second;
		mat[i] = new char[val.size() + 1];
		j = 0;
		for (std::string::iterator itVal = val.begin(); itVal != val.end(); itVal++)
		{
			mat[i][j] = *itVal;
			j++;
		}
		mat[i][j] = '\0';
		i++;
	}
	mat[i] = NULL;
	
	return (mat);
}

void	freeMat(char **mat)
{
	size_t	i = 0;
	
	while (mat[i])
	{
		free(mat[i]);
		i++;
	}
	free(mat[i]);
	free(mat);
}
