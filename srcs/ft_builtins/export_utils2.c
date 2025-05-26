/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afilipe- <afilipe-@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 16:08:35 by afilipe-          #+#    #+#             */
/*   Updated: 2025/05/26 16:28:37 by afilipe-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"


t_env *find_env_node(t_env *head, char *key)
{
	while (head)
	{
		if (ft_strcmp(head->key, key) == 0)
			return head;
		head = head->next;
	}
	return NULL;
}

void update_env_value(t_env *env, char *value, int flag)
{
	if (env->value)
		free(env->value);
	if (value)
		env->value = ft_strdup(value);
	else
		env->value = NULL;
	env->flag = flag;
}

void	print_env(t_shell *type)
{
	sort_env(type->head);
	while (type->head)
	{
		if (type->head->value)
		{
			if (type->head->flag)
				ft_printf("declare -x %s=\"%s\"\n", type->head->key, type->head->value);
			else
				ft_printf("declare -x %s\n", type->head->key);
		}
		else
		{
			ft_printf("declare -x %s\n", type->head->key);
		}
		type->head = type->head->next;
	}
}
/**
 * 
    Memory management: Free key and value in process_export after use.
    Return values: Make sure all functions that should return a value do so.
    VAR+=value support: Not implemented (optional for full Bash mimic).
    Printing: Your print function should use the linked list and print in Bash’s declare -x style.
    Sorting: Call sort_env(type->env_head) before printing.
    Error output: Your ft_fprintf() should print the correct error message.
 */