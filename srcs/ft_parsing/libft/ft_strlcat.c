/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppaula-d <ppaula-d@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/17 14:27:15 by ppaula-d          #+#    #+#             */
/*   Updated: 2024/10/17 14:27:24 by ppaula-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dest, const char *src, size_t size)
{
	size_t	dest_len;
	size_t	src_len;
	size_t	total_len;
	size_t	p;

	dest_len = ft_strlen(dest);
	src_len = ft_strlen(src);
	total_len = (dest_len + src_len);
	if (size <= dest_len)
		return (size + src_len);
	p = dest_len;
	while (*src && (p + 1 < size))
	{
		dest[p] = *src;
		src++;
		p++;
	}
	dest[p] = '\0';
	return (total_len);
}
