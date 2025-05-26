/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_utils2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afilipe- <afilipe-@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 09:43:03 by afilipe-          #+#    #+#             */
/*   Updated: 2025/05/26 11:07:13 by afilipe-         ###   ########.fr       */
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
	int		i;
	size_t	key_len;

	if(!type || !type->env_var || !key)
		return (NULL);
	key_len = ft_strlen(key);
	i = 0;
	while (type->env_var[i])
	{
		if (ft_strncmp(type->env_var[i], key, key_len) == 0 
			&& type->env_var[i][key_len] == '=')
			{
				return (type->env_var[i] + key_len + 1);
			}
			i++;
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
 