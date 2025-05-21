/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_management.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afilipe- <afilipe-@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 12:44:12 by afilipe-          #+#    #+#             */
/*   Updated: 2025/05/21 15:59:20 by afilipe-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

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
			return (NULL);
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

	node = malloc(sizeof(t_env));
	if (!node)
		return (NULL);
	match = ft_strchr(str, '=');
	if(!match)
	{
		node->key = ft_strdup(str);
		node->value = NULL;
	}
	else
	{
		node->key = ft_substr(str, 0, match - str);
		node->value = ft_strdup(match + 1);
	}
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
			free(curr->key);
			free(curr->value);
			free(curr);
			return;
		}
	prev = curr;
	curr = curr->next;
	}
}

void	update_env(t_env **head, char *key, char *value, int append)
{
	t_env	*curr;
	char	*new_val;

	curr = *head;
	while (curr)
	{
		if(ft_strcmp(curr->key, key) == 0)
		{
			if (append && curr->value)
			{
				new_val = malloc(ft_strlen(curr->value) + ft_strlen(value) + 1);
				if (new_val)
				{
					ft_strlcpy(new_val, curr->value);
					ft_strlcat(new_val, value);
					free(curr->value);
					curr->value = new_val;
				}
			}
			else
			{
				free (curr->value);
			}
			return;
		}
		curr = curr->next;
	}
}