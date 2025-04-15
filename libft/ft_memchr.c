/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppaula-d <ppaula-d@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/17 14:29:02 by ppaula-d          #+#    #+#             */
/*   Updated: 2024/10/17 14:29:11 by ppaula-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memchr(const void *s, int c, size_t n)
{
	unsigned char	*str;
	size_t			p;
	unsigned char	uc;

	str = (unsigned char *) s;
	uc = (unsigned char) c;
	p = 0;
	while (p < n)
	{
		if (str[p] == uc)
			return ((void *) &str[p]);
		p++;
	}
	return (NULL);
}
