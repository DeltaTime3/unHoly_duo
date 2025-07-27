/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppaula-d <ppaula-d@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 13:59:12 by ppaula-d          #+#    #+#             */
/*   Updated: 2025/07/26 15:59:54 by ppaula-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	token_counter(t_token *token)
{
	int		count;
	t_token	*curr;

	count = 0;
	curr = token;
	while (curr)
	{
		count++;
		curr = curr->next;
	}
	return (count);
}

int	is_valid_exit_arg(char *arg)
{
	if (!arg)
		return (0);
	if (ft_strcmp(arg, "|") == 0)
		return (0);
	return (1);
}
