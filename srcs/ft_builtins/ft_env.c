/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afilipe- <afilipe-@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 13:34:36 by afilipe-          #+#    #+#             */
/*   Updated: 2025/06/05 14:44:54 by afilipe-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void ft_env(t_shell *type, t_token *command)
{
	t_env *env;;

	env = type->head;
	if (ft_env_extra_args(command))
	{
		printf("env: %s: No such file or directory\n", command->args[0]);
	}
	while (env)
	{
		if(env->flag)
			printf("%s=%s\n", env->key, env->value);
		env = env->next;
	}
}

int ft_env_extra_args(t_token *command)
{
	if (command->args && command->args[0])
		return (1);
	return (0);
}