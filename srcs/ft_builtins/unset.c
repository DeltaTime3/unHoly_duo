/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afilipe- <afilipe-@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 10:24:06 by afilipe-          #+#    #+#             */
/*   Updated: 2025/05/19 10:54:35 by afilipe-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	free_unset(t_token *token, t_shell *type)
{
	char	*str;
	int		i;

	i = 0;
	if (!token->value || !type->env_var)
	{
		return(1);
	}
	if(token->args[1][0] = '-' && token->args[1][1] != '\0')
	{
		return(print_error);
	}
	while (token->args[i])
		{
			str = token->args[i];
			if (ft_strchr(str, '=') == NULL)
				if()
		}
}
