/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppaula-d <ppaula-d@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 13:34:36 by afilipe-          #+#    #+#             */
/*   Updated: 2025/07/07 15:58:59 by ppaula-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void ft_env(t_shell *type, t_token *command)
{
	t_env *env;;

	env = type->head;
	if (command->args && command->args[1])
	{
		ft_printf_fd(2, "env: '%s': No such file or directory\n", command->args[1]);
		type->exit_code = 1;
		return ;
	}
	while (env)
	{
		if(env->flag)
			printf("%s=%s\n", env->key, env->value);
		env = env->next;
	}
}
