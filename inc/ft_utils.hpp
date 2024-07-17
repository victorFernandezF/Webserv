/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_utils.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fortega- <fortega-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/21 12:29:37 by fortega-          #+#    #+#             */
/*   Updated: 2024/07/17 08:31:58 by fortega-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_UTILS_HPP
# define FT_UTILS_HPP

# include <iostream>
# include <limits>
# include <vector>
# include <algorithm>

bool			ft_isBeginStr(std::string reqPath, std::string locPath);
std::string		ft_itoa(int nbr);
int				ft_atoi(std::string const str);
unsigned int	ft_atoiUnInt(std::string const str);
bool			ft_atoiCheck(std::string const std_str);
bool			ft_atoiUnShortCheck(std::string const std_str);
bool			ft_atoiUnIntCheck(std::string const std_str);
bool			ft_atoiErrorPageCheck(std::string const std_str);
bool			ft_atodCheck(const char *str);
bool			ft_atofCheck(const char *str);
std::string 	str_tolower(std::string s);

#endif