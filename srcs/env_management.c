/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_management.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppaula-d <ppaula-d@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 13:59:12 by ppaula-d          #+#    #+#             */
/*   Updated: 2025/07/26 15:47:33 by ppaula-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_env	*init_env(char **envp)
{
	t_env	*head;
	t_env	*new;
	t_env	*tail;

	head = NULL;
	tail = NULL;
	while (*envp)
	{
		new = create_env_node(*envp);
		if (!new)
		{
			free_env(head);
			return (NULL);
		}
		if (!head)
			head = new;
		else
			tail->next = new;
		tail = new;
		envp++;
	}
	return (head);
}

t_env	*create_env_node(char *str)
{
	t_env	*node;
	char	*match;

	if (!str)
		return (NULL);
	node = malloc(sizeof(t_env));
	if (!node)
		return (NULL);
	match = ft_strchr(str, '=');
	if (!match)
	{
		node->key = ft_strdup(str);
		node->value = NULL;
	}
	else
	{
		node->key = ft_substr(str, 0, match - str);
		node->value = ft_strdup(match + 1);
	}
	if (!node->key || (match && !node->value))
		return (free_env_node(node), NULL);
	node->next = NULL;
	return (node);
}

void	env_remove(t_env **head, char *key)
{
	t_env	*curr;
	t_env	*prev;

	curr = *head;
	prev = NULL;
	while (curr)
	{
		if (ft_strcmp(curr->key, key) == 0)
		{
			if (prev)
				prev->next = curr->next;
			else
				*head = curr->next;
			free_env_node(curr);
			return ;
		}
		prev = curr;
		curr = curr->next;
	}
}

void	update_env(t_env **head, char *key, char *value, int append)
{
	t_env	*curr;
	t_env	*tail;

	curr = *head;
	tail = NULL;
	while (curr)
	{
		if (ft_strcmp(curr->key, key) == 0)
		{
			update_env_value(curr, value, append);
			return ;
		}
		tail = curr;
		curr = curr->next;
	}
	add_new_node(head, tail, key, value);
}

t_env	*create_node_from_key(char *key, char *value)
{
	t_env	*node;

	node = malloc(sizeof(t_env));
	if (!node)
		return (NULL);
	node->key = ft_strdup(key);
	if (value)
		node->value = ft_strdup(value);
	else
		node->value = NULL;
	if (!node->key || (value && !node->value))
	{
		free_env_node(node);
		return (NULL);
	}
	node->next = NULL;
	return (node);
}
