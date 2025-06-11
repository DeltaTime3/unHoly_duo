/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_utils2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afilipe- <afilipe-@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 09:43:03 by afilipe-          #+#    #+#             */
/*   Updated: 2025/06/11 12:18:24 by afilipe-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/**
 * retrieves the value of an environment variable, from the shellstate,
 * searches the shell env for the var to search
 * if found, returns a pointer to the value part of the env variable.
 * returns pointer to 
 */
char	*get_env_value(t_shell *type, const char *key)
{
	t_env	*curr;

	if(!type || !type->head || !key)
		return (NULL);
	curr = type->head;
	while (curr)
	{
		if (ft_strcmp(curr, key) == 0)
			{
				return (curr->value);
			}
			curr = curr->next;
	}
	return (NULL);
}

/**
 * attempts to change the working directory.
 * @ token: command tokens.
 * @ new_dir: targuet directory to change to.
 * @ type: shell state.
 * calls chdir to switch to a new directory.
 * On error prints error message and sets return code.
 * On success, calls ft_cd_2 for cleanup and env update. 
 */

void	change_dir(char *new_dir, t_shell *type)
{
	if (chdir(new_dir) != 0)
	{
		free(new_dir);
		new_dir = NULL;
		print_error(E_CD);
		type->r_code = 1;
	}
	else
		ft_cd_2(type, new_dir);
}
 