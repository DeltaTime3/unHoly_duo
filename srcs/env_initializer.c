/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_initializer.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppaula-d <ppaula-d@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 13:59:12 by ppaula-d          #+#    #+#             */
/*   Updated: 2025/07/26 15:54:01 by ppaula-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_env	*init_shell_env(char **enviroment)
{
	t_env	*head;
	t_env	*new_node;

	head = NULL;
	while (*enviroment)
	{
		new_node = create_new_node(*enviroment, &head);
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

t_env	*create_new_node(char *env_str, t_env **head)
{
	char	*equal;

	equal = ft_strchr(env_str, '=');
	if (equal)
		return (create_env_with_val(env_str, equal, head));
	else
		return (create_env_without_val(env_str, head));
}

t_env	*create_env_with_val(char *env_str, char *equal, t_env **head)
{
	char	*key;
	char	*value;
	t_env	*node;

	key = ft_substr(env_str, 0, equal - env_str);
	value = ft_substr(equal + 1, 0, ft_strlen(equal + 1));
	node = NULL;
	if (!key || !value)
	{
		free_not_null(key);
		free_not_null(value);
		free_env(*head);
		return (NULL);
	}
	node = env_lstnew(key, value, 1);
	return (node);
}

t_env	*create_env_without_val(char *env_str, t_env **head)
{
	char	*key;
	t_env	*node;

	key = ft_strdup(env_str);
	node = NULL;
	if (!key)
	{
		free_env(*head);
		return (NULL);
	}
	node = env_lstnew(key, NULL, 0);
	return (node);
}

void	free_not_null(void *ptr)
{
	if (ptr)
		free(ptr);
}
