/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_initializer.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afilipe- <afilipe-@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/09 10:28:42 by afilipe-          #+#    #+#             */
/*   Updated: 2025/07/07 10:38:00 by afilipe-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

t_env *init_shell_env(char **enviroment)
{
	t_env *head;
	char *equal;
	t_env	*new_node;
	char	*key;
	char	*value;

	head = NULL;
	while (*enviroment)
	{
		equal = ft_strchr(*enviroment, '=');
		if (equal)
		{
			key = ft_substr(*enviroment, 0, equal - *enviroment);
			value = ft_substr(equal + 1, 0, ft_strlen(equal + 1));
			if (!key || !value)
			{
				if (key)
					free(key);
				if (value)
					free(value);
				free_env(head);
				return (NULL);
			}
			new_node = env_lstnew(key, value, 1);
		}
		else
		{
			key = ft_strdup(*enviroment);
			if (!key)
			{
				free_env(head);
				return (NULL);
			}
			new_node = env_lstnew(key, NULL, 0);
		}
		if (!new_node)
		{
			free_env(head);
			return (NULL);
		}
		env_back(&head, new_node);
		enviroment++;
	}
	return (head);
}

t_env *env_lstnew(char *key, char *value, int flag)
{
	t_env	*new;
	
	new = malloc(sizeof(t_env));
	if (!new)
		return (NULL);
	new->key = key;
	new->flag = flag;
	new->value = value;
	new->next = NULL;
	return(new);
}

void env_back(t_env **lst, t_env *new)
{
	if (!*lst)
	{
		*lst = new;
		return ;
	}
	t_env	*curr;

	curr = *lst;
	while (curr->next)
		curr = curr->next;
	curr->next = new;
}

void	env_freelst(t_env *node)
{
	free(node->key);
	free(node->value);
	free(node);
}

void	clear_env_list(t_env *env)
{
	t_env	*temp;

	while (env)
	{
		temp = env->next;
		env_freelst(env);
		env = temp;
	}
}
