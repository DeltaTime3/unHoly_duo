/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppaula-d <ppaula-d@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/17 14:28:33 by ppaula-d          #+#    #+#             */
/*   Updated: 2024/10/17 14:28:40 by ppaula-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *s, int c)
{
	unsigned int	p;
	char			*res;
	char			cc;

	cc = (char) c;
	res = NULL;
	p = 0;
	while (s[p])
	{
		if (s[p] == cc)
			res = (char *) &s[p];
		p++;
	}
	if (s[p] == cc)
		res = (char *) &s[p];
	return (res);
}
