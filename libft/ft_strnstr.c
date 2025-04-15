/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppaula-d <ppaula-d@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/17 14:29:30 by ppaula-d          #+#    #+#             */
/*   Updated: 2024/10/17 14:29:37 by ppaula-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *big, const char *little, size_t n)
{
	size_t	p;
	size_t	c;

	p = 0;
	c = 0;
	if (little[0] == '\0')
		return ((char *) big);
	while (big[p] && p < n)
	{
		while (big[p + c] == little[c] && big[p + c] && p + c < n)
		{
			c++;
			if (little[c] == '\0')
				return ((char *) big + p);
		}
		p++;
		c = 0;
	}
	return (0);
}
