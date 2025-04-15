/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atol.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppaula-d <ppaula-d@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 22:11:39 by ppaula-d          #+#    #+#             */
/*   Updated: 2025/02/12 00:08:19 by ppaula-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

long long	ft_atol(const char *str)
{
	int			i;
	int			signal;
	long long	count;

	i = 0;
	signal = 1;
	count = 0;
	while ((str[i] >= 9 && str[i] <= 13) || str[i] == 32)
		i++;
	if (str[i] == 43 || str[i] == 45)
	{
		if (str[i] == 45)
			signal *= -1;
		i++;
	}
	while ((str[i] >= 48 && str[i] <= 57) && str[i] != '\0')
	{
		count *= 10;
		count += str[i] - 48;
		i++;
	}
	if (str[i] > 57 || str[i] == 43 || str[i] == 45)
		return (-2147483649);
	count *= signal;
	return (count);
}
