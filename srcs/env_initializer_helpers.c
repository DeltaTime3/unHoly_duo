/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_initializer_helpers.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppaula-d <ppaula-d@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 13:59:12 by ppaula-d          #+#    #+#             */
/*   Updated: 2025/07/26 15:55:50 by ppaula-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_env	*env_lstnew(char *key, char *value, int flag)
{
	t_env	*new;

	new = malloc(sizeof(t_env));
	if (!new)
		return (NULL);
	new->key = key;
	new->flag = flag;
	new->value = value;
	new->next = NULL;
	return (new);
}

void	env_back(t_env **lst, t_env *new)
{
	t_env	*curr;

	if (!*lst)
	{
		*lst = new;
		return ;
	}
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
