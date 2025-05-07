/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_free_str.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppaula-d <ppaula-d@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 22:12:42 by ppaula-d          #+#    #+#             */
/*   Updated: 2025/02/12 00:08:24 by ppaula-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	free_str(char **list)
{
	char	*current_str;

	if (!list)
		return ;
	while (*list)
	{
		current_str = *list;
		list++;
		free(current_str);
	}
	*list = NULL;
}
