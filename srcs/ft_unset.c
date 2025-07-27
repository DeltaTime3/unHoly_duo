/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppaula-d <ppaula-d@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 13:59:12 by ppaula-d          #+#    #+#             */
/*   Updated: 2025/07/26 16:00:25 by ppaula-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_unset(t_shell *shell, t_token *args)
{
	int		error_code;
	t_token	*curr;

	error_code = 0;
	curr = args;
	while (curr)
	{
		if (!validate_unset_args(curr->value))
		{
			error_code = 0;
		}
		else
		{
			env_remove(&shell->head, curr->value);
		}
		curr = curr->next;
	}
	shell->return_code = error_code;
	return (error_code);
}

int	validate_unset_args(char *args)
{
	int	i;

	if (!args || !*args)
		return (0);
	if (!ft_isalpha(args[0]) && args[0] != '_')
		return (0);
	i = 1;
	while (args[i])
	{
		if (!ft_isalnum(args[i]) && args[i] != '_')
			return (0);
		i++;
	}
	return (1);
}
