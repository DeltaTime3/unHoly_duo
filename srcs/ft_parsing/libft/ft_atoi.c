/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppaula-d <ppaula-d@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/17 14:29:45 by ppaula-d          #+#    #+#             */
/*   Updated: 2024/10/17 14:29:52 by ppaula-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_atoi(const char *str)
{
	int	p;
	int	sign;
	int	result;

	p = 0;
	sign = 1;
	result = 0;
	while ((9 <= str[p] && str[p] <= 13) || str[p] == 32)
		p++;
	if (str[p] == 43 || str[p] == 45)
	{
		if (str[p] == 45)
			sign *= -1;
		p++;
	}
	while (48 <= str[p] && str[p] <= 57 && str[p] != '\0')
	{
		result *= 10;
		result += str[p] - 48;
		p++;
	}
	result *= sign;
	return (result);
}
