/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_initializer.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afilipe- <afilipe-@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/09 10:28:42 by afilipe-          #+#    #+#             */
/*   Updated: 2025/06/09 10:58:53 by afilipe-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void init_shell_env(char **enviroment)
{
	t_env *head;
	char *equal;

	head = NULL;
	while (*enviroment)
	{
		equal = ft_strchr(*enviroment, '=');
		if (equal)
		{
			env_back(&head, env_lstnew(ft_substr(*enviroment, 0, equal - *enviroment),
			ft_substr(equal + 1, 0, ft_strlen(equal + 1)), 1));
		}
		else
		{
			env_back(&head, env_lstnew(ft_strdup(*enviroment), NULL, 0));
		}
		enviroment++;
	}
	return (head);
}

t_env *env_lstnew(char *key, char *value, int flag)
{
	t_env	*new;
	
	new = malloc(sizeof(t_env));
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

	curr = lst;
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