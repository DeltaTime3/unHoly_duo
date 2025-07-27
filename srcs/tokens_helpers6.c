/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens_helpers6.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppaula-d <ppaula-d@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 13:59:12 by ppaula-d          #+#    #+#             */
/*   Updated: 2025/07/27 14:18:30 by ppaula-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	count_cmd_args(t_token *current)
{
	int		arg_count;
	t_token	*temp;

	arg_count = 1;
	temp = current->next;
	while (temp && temp->type != PIPE)
	{
		if ((temp->type == ARGUMENT || temp->type == FLAG)
			&& temp->value && !(temp->value[0] == '\0' && temp->was_expanded))
			arg_count++;
		temp = temp->next;
	}
	return (arg_count);
}

void	fill_cmd_args(t_token *current, int arg_count)
{
	t_token	*temp;
	int		i;

	current->args = (char **)ft_calloc(arg_count + 1, sizeof(char *));
	if (!current->args)
		return ;
	current->args[0] = ft_strdup(current->value);
	i = 1;
	temp = current->next;
	while (temp && temp->type != PIPE && i < arg_count)
	{
		if ((temp->type == ARGUMENT || temp->type == FLAG)
			&& temp->value && !(temp->value[0] == '\0' && temp->was_expanded))
		{
			current->args[i] = ft_strdup(temp->value);
			i++;
		}
		temp = temp->next;
	}
}

void	prep_cmd_args(t_token *head)
{
	t_token	*current;
	int		arg_count;

	current = head;
	while (current)
	{
		if (current->type == COMMAND && current->value
			&& current->value[0] != '\0' && !current->was_expanded)
		{
			if (current->args)
				free_args(current->args);
			arg_count = count_cmd_args(current);
			fill_cmd_args(current, arg_count);
		}
		current = current->next;
	}
}
