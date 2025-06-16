/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afilipe- <afilipe-@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 11:51:55 by afilipe-          #+#    #+#             */
/*   Updated: 2025/06/16 13:11:38 by afilipe-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	ft_echo(t_token *token)
{
	int		flag;
	int		first_arg;
	t_token	*curr;

	flag = 0;
	first_arg = 1;
	curr = token;
	while (curr && curr->value && ft_strcmp(curr->value, "-n") == 0)
		{
			flag = 1;
			curr = curr->next;
		}
		curr = curr->next;
		while (curr)
		{
			if (!first_arg)
				printf(" ");
			printf("%s", curr->content);
			first_arg = 0;
			curr = curr->next;
		}
		if (!flag)
			printf("\n");
		return (0);
}
