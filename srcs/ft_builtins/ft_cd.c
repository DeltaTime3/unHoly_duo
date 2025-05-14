/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afilipe- <afilipe-@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 14:29:58 by afilipe-          #+#    #+#             */
/*   Updated: 2025/05/14 10:43:36 by afilipe-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

/**
 * Handle the 'cd command.
 * @ token: arguments list for 'cd'
 * @ type: shell state (enviroment, dir, return code)
 * checks argument count for 'cd, determines and validates
 * target directory and attempts to change it.
 * If an error occurs (too many args or invadid dir), an error message is 
 * printed and return code is set to 1. 
 */

void	ft_cd(t_token *token, t_shell *type)
{
	char	*new_dir;
	int		is_cd_minus;

	is_cd_minus = 0;
	if (ct_nodes(token) > 2)
	{
		print_error(E_TARG);
		type->r_code = 1;
		return ;
	}
	if (token->next)
		expander(&token->next, type);
	new_dir = get_cd_target(token, type, &is_cd_minus);
	if (check_dir(new_dir))
	{
		change_dir(token, new_dir, type);
		if (is_cd_minus)
			printf("%s\n", type->curr_dir);
	}
	else
	{
		type->r_code = 1;
		free(new_dir);
	}
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

void	change_dir(t_token *token, char *new_dir, t_shell *type)
{
	if (chdir(new_dir) != 0)
	{
		free(new_dir);
		print_error(E_CD);
		type->r_code = 1;
	}
	else
		ft_cd_2(type, new_dir);
}

/**
 * Updates shell state after successfully changing directory;
 * frees prev working directory, updates both past and current 
 * directory paths;
 * updates PWD and OLDPWD;
 * frees new dir if it's not the same pointer as curr_dir
*/

void	ft_cd_2(t_shell *type, char *new_dir)
{
	free(type->prev_dir);
	type->prev_dir = ft_strdup(type->curr_dir);
	cd_env(type);
	if (new_dir != type->curr_dir && ft_strcmp(new_dir, "") != 0)
		free(new_dir);
}

/**
 * handle for specific cases:
 * no arg or "~", go to HOME;
 * "-" go to previous directory;
 * "." stay in current directory:
 * other values, treat as a path;
 * 
 * stes new_path. 
 */

char	*get_cd_target(t_token *token, t_shell *type, int *is_cd_minus)
{
	char	*temp;

	*is_cd_minus = 0;
	if (!token->next || !token->next->value || ft_strcmp(token->next->value,
			"~") == 0)
	{
		temp = get_env_value(type, "HOME");
		if (temp)
			return (ft_strdup(temp));
		else
			return (NULL);
	}
	else if (ft_strcmp(token->next->value, "-") == 0)
	{
		*is_cd_minus = 1;
		temp = get_env_value(type, "OLDPWD");
		if (temp)
			return (ft_strdup(temp));
		return (NULL);
	}
	else if (ft_strcmp(token->next->value, ".") == 0)
		return (ft_strdup(type->curr_dir));
	return (ft_strdup(token->next->value));
}

/**
 * helper to resolve directory if no args are given.
 * if no argument is passed, retrieves the HOME environment
 * variable and sets it a the new targuet dir.
 */

int	dir_val(t_token *token, t_shell *type, char **new_dir)
{
	char	*home;

	if (!token->next)
	{
		home = get_env_value(type, "HOME");
		if (home)
			*new_dir = ft_strdup(home);
		else
			*new_dir = NULL;
		return (1);
	}
	return (1);
}
