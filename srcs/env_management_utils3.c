/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_management_utils3.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppaula-d <ppaula-d@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 13:59:12 by ppaula-d          #+#    #+#             */
/*   Updated: 2025/07/26 15:51:37 by ppaula-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	clean_directories(t_shell *shell)
{
	free_and_null((void **)&(shell->prev_dir));
	free_and_null((void **)&(shell->curr_dir));
	free_and_null((void **)&(shell->pwd));
}

void	clean_token_type_value(t_shell *shell)
{
	free_and_null((void **)&(shell->token));
	free_and_null((void **)&(shell->type));
	free_and_null((void **)&(shell->value));
}

void	free_and_null(void **ptr)
{
	if (ptr && *ptr)
	{
		free(*ptr);
		*ptr = NULL;
	}
}

void	clean_command_resources(t_shell *shell)
{
	if (shell->token)
	{
		free(shell->token);
		shell->token = NULL;
	}
	if (shell->type)
	{
		free(shell->type);
		shell->type = NULL;
	}
	if (shell->value)
	{
		free(shell->value);
		shell->value = NULL;
	}
}

char	*append_env_value_man(char *old_val, char *add_val)
{
	size_t	len_old;
	size_t	len_add;
	char	*new_val;

	len_old = 0;
	len_add = 0;
	if (old_val)
		len_old = ft_strlen(old_val);
	if (add_val)
		len_add = ft_strlen(add_val);
	new_val = malloc(len_old + len_add + 1);
	if (!new_val)
		return (NULL);
	if (old_val)
		ft_strlcpy(new_val, old_val, len_old + 1);
	else
		new_val[0] = '\0';
	if (add_val)
		ft_strlcat(new_val, add_val, len_old + len_add + 1);
	return (new_val);
}
