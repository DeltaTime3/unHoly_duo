/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppaula-d <ppaula-d@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 13:59:12 by ppaula-d          #+#    #+#             */
/*   Updated: 2025/07/26 15:57:35 by ppaula-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	sort_env(t_env *head)
{
	t_env	*curr;
	int		i;

	if (!head)
		return ;
	i = 1;
	while (i)
	{
		i = 0;
		curr = head;
		while (curr->next)
		{
			if (ft_strcmp(curr->key, curr->next->key) > 0)
			{
				swap_env(curr, curr->next);
				i = 1;
			}
			curr = curr->next;
		}
	}
}

void	swap_env(t_env *arg1, t_env *arg2)
{
	char	*temp_key;
	char	*temp_value;
	int		temp_flag;

	temp_key = arg1->key;
	temp_value = arg1->value;
	temp_flag = arg1->flag;
	arg1->key = arg2->key;
	arg1->value = arg2->value;
	arg1->flag = arg2->flag;
	arg2->key = temp_key;
	arg2->value = temp_value;
	arg2->flag = temp_flag;
}

int	process_export(t_shell *type, char *args)
{
	char	*key;
	char	*value;
	int		flag;
	t_env	*env;

	key = NULL;
	value = NULL;
	flag = 0;
	parse_exp_args(args, &key, &value, &flag);
	env = find_env_node(type->head, key);
	if (env)
	{
		if (flag)
			update_env_value_bi(env, value, flag);
	}
	else
		add_env_node(&type->head, key, value, flag);
	if (key)
		free(key);
	if (value)
		free(value);
	return (0);
}

void	add_env_node(t_env **head, const char *key, char *value, int flag)
{
	t_env	*new;
	t_env	*curr;

	new = ft_calloc(1, sizeof(t_env));
	new->key = ft_strdup(key);
	if (value)
		new->value = ft_strdup(value);
	else
		new->value = NULL;
	new->flag = flag;
	if (!*head)
	{
		*head = new;
	}
	else
	{
		curr = *head;
		while (curr->next)
			curr = curr->next;
		curr->next = new;
	}
}

void	parse_exp_args(const char *args, char **key, char **value, int *flag)
{
	int	i;

	i = 0;
	*flag = 0;
	*value = NULL;
	while (args[i] && args[i] != '=' && !(args[i] == '+' && args[i + 1] == '='))
		i++;
	if (args[i] == '+' && args[i + 1] == '=')
	{
		*flag = 2;
		*key = extract_key(args, i);
		*value = ft_strdup(args + i + 2);
	}
	else if (args[i] == '=')
	{
		*flag = 1;
		*key = extract_key(args, i);
		*value = ft_strdup(args + i + 1);
	}
	else
	{
		*key = ft_strdup(args);
		*flag = 0;
	}
}
