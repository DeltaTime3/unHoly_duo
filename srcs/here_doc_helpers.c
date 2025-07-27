/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc_helpers.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppaula-d <ppaula-d@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 13:59:12 by ppaula-d          #+#    #+#             */
/*   Updated: 2025/07/26 18:26:57 by ppaula-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_dollar_expansion(const char *value)
{
	const char	*end;
	int			i;

	if (value[1] == '{')
	{
		end = ft_strchr(value + 2, '}');
		if (end && end[1] == '\0')
			return (1);
	}
	else if (ft_isalpha(value[1]) || value[1] == '_')
	{
		i = 2;
		while (value[i] && (ft_isalnum(value[i]) || value[i] == '_'))
			i++;
		if (value[i] == '\0')
			return (1);
	}
	return (0);
}
