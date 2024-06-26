/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_utils.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fortega- <fortega-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/21 12:30:32 by fortega-          #+#    #+#             */
/*   Updated: 2024/05/30 15:22:05 by fortega-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_utils.hpp"

unsigned short	error_codes[] = {400, 401, 402, 403, 404, 405, 406, 407, 408, 409,
								410, 411, 412, 413, 414, 415, 416, 417, 421, 422,
								426, 500, 501, 503, 504, 505};

bool	ft_isBeginStr(std::string orig, std::string toCheck)
{
	size_t i = 0;

	if (orig.size() >= toCheck.size())
	{
		while (i < toCheck.size())
		{
			if (orig[i] != toCheck[i])
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
	
	if (nbr < 0)
	{
		str += '-';
		nbr = -nbr;
	}
	if (nbr == 0)
	{
		str += '0';
		//str += '\0';
	}
	while (nbr > 0)
	{
		str += (nbr % 10) + '0';
		nbr = nbr / 10;
	}
	//str += '\0';
	return (str);
}

int		ft_strlen(const char *str)
{
	int i = 0;
	while (str[i])
		i++;
	return (i);
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

bool	ft_atoiCheck(std::string const std_str)
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

	if (total < std::numeric_limits<int>::min() || total > std::numeric_limits<int>::max())
	{
		return (false);
	}
	else
		return (true);
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

bool	ft_atodCheck(const char *str)
{	
	int		i = 0;
	int		ndot = 0;
	const char	dot = '.';

	while ((str[i] > 8 && str[i] < 14) || str[i] == 32)
		i++;
	if (str[i] == '+' || str[i] == '-' || (str[i] >= '0' && str[i] <= '9'))
		i++;
	else
	{
		if (str[i] != dot)
			return (false);
		else
			ndot++;
		i++;
	}
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
		{
			if (str[i] != dot || ndot > 1)
				return (false);
			else
				ndot++;
		}
		i++;
	}
	return (true);
}

bool	ft_atofCheck(const char *str)
{	
	int			i = 0;
	int			ndot = 0;
	const char	dot = '.';

	while ((str[i] > 8 && str[i] < 14) || str[i] == 32)
		i++;
	if (str[i] == '+' || str[i] == '-' || (str[i] >= '0' && str[i] <= '9'))
		i++;
	else
	{
		if (str[i] != dot)
			return (false);
		else
			ndot++;
		i++;
	}
	while (str[i] && i < ft_strlen(str) - 1)
	{ 
		if (str[i] < '0' || str[i] > '9')
		{
			if (str[i] != dot || ndot > 1)
				return (false);
			else
				ndot++;
		}
		i++;
	}
	return (true);
}
