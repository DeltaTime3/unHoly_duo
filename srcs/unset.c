/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppaula-d <ppaula-d@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 13:59:12 by ppaula-d          #+#    #+#             */
/*   Updated: 2025/07/26 15:56:32 by ppaula-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_unset(t_token *token, t_shell *type)
{
	char	*str;
	int		i;

	i = 0;
	if (!token || !token->args || !type || !type->head)
	{
		return ;
	}
	if (token->args[1] && token->args[1][0] == '-' && token->args[1][1] != '\0')
	{
		print_error(E_MSH);
		type->return_code = 2;
		return ;
	}
	while (token->args[i])
	{
		str = token->args[i];
		if (str)
			free(str);
		i++;
	}
	free(token->args);
}
