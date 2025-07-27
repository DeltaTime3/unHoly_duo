/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_management_utils2.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppaula-d <ppaula-d@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 13:59:12 by ppaula-d          #+#    #+#             */
/*   Updated: 2025/07/26 15:51:08 by ppaula-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**env_list_to_array(t_env *head)
{
	char	**array;
	int		count;

	count = count_env(head);
	array = ft_calloc(count + 1, sizeof(char *));
	if (!array)
		return (NULL);
	fill_env_array(array, head);
	return (array);
}

int	count_env(t_env *head)
{
	int		count;
	t_env	*curr;

	count = 0;
	curr = head;
	while (curr)
	{
		if (curr->value)
			count++;
		curr = curr->next;
	}
	return (count);
}

void	fill_env_array(char **array, t_env *head)
{
	int		i;
	char	*temp;
	t_env	*curr;

	i = 0;
	curr = head;
	while (curr)
	{
		if (curr->value)
		{
			temp = ft_strjoin(curr->key, "=");
			array[i] = ft_strjoin(temp, curr->value);
			free(temp);
			i++;
		}
		curr = curr->next;
	}
	array[i] = NULL;
}

void	clean_all_resources(t_shell *shell)
{
	clean_env(shell);
	clean_directories(shell);
	clean_token_type_value(shell);
	shell->tail = NULL;
}

void	clean_env(t_shell *shell)
{
	if (shell->head)
	{
		free_env(shell->head);
		shell->head = NULL;
	}
}
