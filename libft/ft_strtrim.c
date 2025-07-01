/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppaula-d <ppaula-d@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/23 13:56:46 by ppaula-d          #+#    #+#             */
/*   Updated: 2025/07/01 14:07:32 by ppaula-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strtrim(char *s1, char *set)
{
    size_t	i;
    size_t	j;
    char	*str;
    
    if (s1 == NULL || set == NULL)
        return (NULL);
    i = 0;
    j = ft_strlen(s1);
    while (s1[i] && ft_strchr(set, s1[i]))
        i++;
    while (j > i && ft_strchr(set, s1[j - 1]))
        j--;
    str = (char *)malloc(sizeof(char) * (j - i + 1));
    if (!str)
        return (NULL);
    ft_strlcpy(str, s1 + i, j - i + 1);
    return (str);
}
