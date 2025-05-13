/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afilipe- <afilipe-@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 11:51:52 by afilipe-          #+#    #+#             */
/*   Updated: 2025/05/13 16:31:38 by afilipe-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

handle_exit(t_token **token, t_shell *type)
{
	int	n_token;
	int	flag;

	n_token = 0;
	flag = handle_code(token, type, n_token);
	free_exit(type);
	if (flag)
		exit (type->return_code);
}

int	handle_code(t_token ** token, t_shell type, int n_token)
{
	if (token != NULL && *token != NULL)
	{
		if((*token)->next)
		{
			
		}
	}
}