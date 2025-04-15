/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppaula-d <ppaula-d@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/24 08:41:07 by ppaula-d          #+#    #+#             */
/*   Updated: 2025/02/12 00:06:55 by ppaula-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_wcount(char *str, char sep)
{
	int	counter;
	int	i;

	counter = 0;
	i = 0;
	while (str[i] != '\0')
	{
		while (str[i] == sep)
			i++;
		if (str[i] != '\0')
		{
			counter++;
			while (str[i] != sep && str[i] != '\0')
				i++;
		}
	}
	return (counter);
}

static int	ft_alphacount(char *str, char sep, int word)
{
	int	i;
	int	h;
	int	count;

	i = 0;
	h = 0;
	while (str[i] != '\0')
	{
		while (str[i] == sep)
			i++;
		if (str[i] != '\0')
		{
			if (h == word)
			{
				count = 0;
				while (str[i + count] != sep && str[i + count] != '\0')
					count++;
				return (count);
			}
			h++;
			while (str[i] != sep && str[i] != '\0')
				i++;
		}
	}
	return (0);
}

static int	ft_alphacountbegin(char *str, char sep, int word)
{
	int	i;
	int	h;

	i = 0;
	h = 0;
	while (str[i] != '\0')
	{
		while (str[i] == sep)
			i++;
		if (str[i] != '\0')
		{
			if (h == word)
				return (i);
			h++;
			while (str[i] != sep && str[i] != '\0')
				i++;
		}
	}
	return (i);
}

char	**ft_split(char *str, char c)
{
	char	**strs;
	int		i;
	int		wcounter;
	int		length;

	i = 0;
	wcounter = ft_wcount((char *)str, c);
	strs = (char **)malloc(sizeof(char *) * (wcounter + 1));
	if (strs == NULL)
		return (NULL);
	while (i < wcounter)
	{
		length = ft_alphacount((char *)str, c, i);
		strs[i] = (char *)malloc(sizeof(char) * (length + 1));
		if (strs[i] == NULL)
		{
			ft_free(strs);
			return (NULL);
		}
		ft_strlcpy(strs[i], str + ft_alphacountbegin((char *)str, c, i),
			length + 1);
		i++;
	}
	strs[wcounter] = NULL;
	return (strs);
}
