/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afilipe- <afilipe-@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 16:08:35 by afilipe-          #+#    #+#             */
/*   Updated: 2025/05/27 15:42:40 by afilipe-         ###   ########.fr       */
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
	char *temp;
	
	if (flag == 2)
	{
		append_env_value(env, value);
	}
	else
	{
		if(env->value)
			free(env->value);
		if (value)
			env->value = ft_strdup(value);
		else
			env->value = NULL;
	}
}

void	print_env(t_shell *type)
{
	t_env	*temp;
	

	sort_env(type->head);
	temp = type->head;
	while (temp)
	{
		if (temp->flag)
		{
			if (temp->value)
				ft_printf("declare -x %s=\"%s\"\n", temp->key, temp->value);
			else
				ft_printf("declare -x %s=\"\"\n", temp->key);
		}
		else
			ft_printf("declare -x %s\n", temp->key);
		temp = temp->next;
	}
}

char	*extract_key(const char *args, int len)
{
	char *key;
	
	key = malloc(len + 1);
	if (!key)
		return NULL;
	ft_strlcpy(key, args, len + 1);
	return key;
}

void append_env_value(t_env *env, char *value)
{
	char	*temp;
	
	if (env->value && value)
	{
		temp = ft_strjoin(env->value, value);
		if (!temp)
			return;
		free(env->value);
		env->value = temp;
	}
	else if (value)
	{
		free(env->value);
		env->value = ft_strdup(value);
	}
}
/**
 * 
    Memory management: Free key and value in process_export after use.
    Return values: Make sure all functions that should return a value do so.
    VAR+=value support: Not implemented (optional for full Bash mimic).
    Error output: Your ft_fprintf() should print the correct error message.
 */