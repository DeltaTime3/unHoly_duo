/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afilipe- <afilipe-@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 10:24:06 by afilipe-          #+#    #+#             */
/*   Updated: 2025/06/03 11:59:36 by afilipe-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	free_unset(t_token *token, t_shell *type)
{
	char	*str;
	int		i;

	i = 0;
	if (!token || !token->args || !type || !type->env_var)
	{
		return;
	}
	if(token->args[1] && token->args[1][0] == '-' && token->args[1][1] != '\0')
	{
		print_error(E_MSH);
		type->return_code = 2;
		return;
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
