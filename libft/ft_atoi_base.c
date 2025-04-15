/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi_base.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppaula-d <ppaula-d@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/17 14:29:45 by ppaula-d          #+#    #+#             */
/*   Updated: 2025/03/05 15:02:41 by ppaula-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_atoi_base(const char *str, int base)
{
	int	result;
	int	sign;

	result = 0;
	sign = 1;
	if (*str == '-')
	{
		sign = -1;
		str++;
	}
	while (*str)
	{
		if (*str >= '0' && *str <= '9')
			result = result * base + (*str - '0');
		else if (*str >= 'A' && *str <= 'F')
			result = result * base + (*str - 'A' + 10);
		else if (*str >= 'a' && *str <= 'f')
			result = result * base + (*str - 'a' + 10);
		else
			break ;
		str++;
	}
	return (result * sign);
}
