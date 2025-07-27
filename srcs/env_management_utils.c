/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_management_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppaula-d <ppaula-d@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 13:59:12 by ppaula-d          #+#    #+#             */
/*   Updated: 2025/07/26 15:49:42 by ppaula-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	update_env_value(t_env *node, char *value, int append)
{
	char	*new_val;

	if (append)
	{
		new_val = append_env_value_man(node->value, value);
		if (!new_val)
			return ;
		free(node->value);
		node->value = new_val;
	}
	else
	{
		free(node->value);
		if (value)
		{
			node->value = ft_strdup(value);
		}
		else
		{
			node->value = NULL;
		}
	}
}

void	add_new_node(t_env **head, t_env *tail, char *key, char *value)
{
	t_env	*new_node;

	new_node = create_node_from_key(key, value);
	if (!new_node)
		return ;
	if (!*head)
	{
		*head = new_node;
		if (tail)
			tail = new_node;
	}
	else
	{
		if (tail && tail)
		{
			tail->next = new_node;
			tail = new_node;
		}
	}
}

void	free_env_node(t_env *node)
{
	if (!node)
		return ;
	free(node->key);
	free(node->value);
	free(node);
}

void	free_env_array(char **env)
{
	int	i;

	i = 0;
	if (!env)
		return ;
	while (env[i])
	{
		free(env[i]);
		i++;
	}
	free(env);
}

void	free_env(t_env *head)
{
	t_env	*temp;

	while (head)
	{
		temp = head->next;
		free(head->key);
		head->key = NULL;
		free(head->value);
		head->value = NULL;
		free(head);
		head = temp;
	}
}
