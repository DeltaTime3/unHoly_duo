/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_utils2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppaula-d <ppaula-d@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 13:59:12 by ppaula-d          #+#    #+#             */
/*   Updated: 2025/07/31 23:30:11 by ppaula-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_env_value(t_env *head, const char *key)
{
	t_env	*curr;

	curr = head;
	while (curr)
	{
		if (ft_strcmp(curr->key, key) == 0)
		{
			return (curr->value);
		}
		curr = curr->next;
	}
	return (NULL);
}
