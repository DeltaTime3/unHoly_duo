/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_management_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afilipe- <afilipe-@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 14:42:39 by afilipe-          #+#    #+#             */
/*   Updated: 2025/06/02 13:17:26 by afilipe-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

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
	if(!new_node)
		return ;
	if (!*head)
		*head = new_node;
	else
		tail->next = new_node;
}

void	free_env_node(t_env *node)
{
	if(!node)
		return ;
	free(node->key);
	free(node->value);
	free(node);
}

void	free_env(t_env *head)
{
	t_env	*temp;

	while (head)
	{
		temp = head;
		head = head->next;
		free_env_node(temp);
	}
}
