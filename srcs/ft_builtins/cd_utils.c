/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afilipe- <afilipe-@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 11:14:00 by afilipe-          #+#    #+#             */
/*   Updated: 2025/05/14 10:40:50 by afilipe-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

/**
 * vadidates if path is a valid directory;
 * @new_dir : the path to validate.
 * 
 * Checks if path exists;
 * if it's a directory;
 * if user has permition to access it;
 * 
 * if chack fails, prints error message and returns 0 (failure)
 * return 1 on success
 */

int	check_dir(char *new_dir)
{
	struct stat	path_status;

	if (!new_dir)
	{
		print_error(E_HOME);
		return (0);
	}
	if (stat(new_dir, &path_status) == -1)
	{
		print_error(E_CD);
		return (0);
	}
	if (!S_ISDIR(path_status.st_mode))
	{
		print_error(E_CD);
		return (0);
	}
	if (access(new_dir, R_OK | X_OK) == -1)
	{
		print_error(E_PERMIT);
		return (0);
	}
	return (1);
}

/**
 * Updates the PWD environment variable and shell state
 * 
 * retrives the current working directory using getcwd,
 * updates the shell current directory and updates the PWD
 * with the new value.
 * 
 * Frees prev environment string to prevent leaks
 */

void	cd_env_pwd(t_shell *type)
{
	char	*new_pwd;
	char	*pwd;
	int		i;
	char	cwd[4096];

	pwd = get_env_key(type, "PWD");
	if (!pwd)
		return ;
	new_pwd = ft_strjoin("PWD=", getcwd(cwd, sizeof(cwd)));
	getcwd(type->curr_dir, sizeof(type->curr_dir));
	i = 0;
	while (type->env_var[i])
	{
		if (ft_strncmp(type->env_var[i], "PWD=", 4) == 0)
		{
			free(type->env_var[i]);
			type->env_var[i] = new_pwd;
		}
		i++;
	}
	free(pwd);
}
/**
 * Replaces OLDPWD with previous directory
 * updates PWD with cd_env_pwd
 * 
 * frees old values,
 */

void	cd_env(t_shell *type)
{
	char	*old_pwd;
	char	*temp;
	int		i;

	i = 0;
	old_pwd = get_env_key(type, "OLDPWD");
	if (!old_pwd)
	{
		free(old_pwd);
		return ;
	}
	while (type->env_var[i])
	{
		if (ft_strncmp(type->env_var[i], "OLDPWD=", 7) == 0)
		{
			free(type->env_var[i]);
			temp = ft_strjoin("OLDPWD=", type->prev_dir);
			type->env_var[i] = ft_strdup(temp);
			free(temp);
			break ;
		}
		i++;
	}
	cd_env_pwd(type);
	free(old_pwd);
}
