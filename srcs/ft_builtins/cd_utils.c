/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afilipe- <afilipe-@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 11:14:00 by afilipe-          #+#    #+#             */
/*   Updated: 2025/05/26 11:02:55 by afilipe-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

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

	pwd = get_env_value(type, "PWD");
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
	char	*temp;
	int		i;

	i = 0;
	while (type->env_var[i])
	{
		if (ft_strncmp(type->env_var[i], "OLDPWD=", 7) == 0)
		{
			free(type->env_var[i]);
			temp = ft_strjoin("OLDPWD=", type->prev_dir);
			type->env_var[i] = temp;
			cd_env_pwd(type);
			return ;
		}
		i++;
	}
	add_old_pwd_to_env(type);
	cd_env_pwd(type);
}

/**
 * adds OLDPWD to the env variables if it does not exist
 * allocates a new env array, copies existing variables,
 * updates shell' env pointer. Print error if allocation fails.
 */
void add_old_pwd_to_env(t_shell *type)
{
	char	*temp;
	char	**new;
	int		i;
	int		j;

	i = 0;
	while (type->env_var[i])
		i++;
	new = malloc(sizeof(char *) * (i + 2));
	if (!new)
	{
		print_error("Memory allocation failed");
		return ;
	}
	j = 0;
	while (j < i)
	{
		new[j] = type->env_var[j];
		j++;
	}
	temp = ft_strjoin("OLDPWD=", type->prev_dir);
	new[j] = temp;
	new[j + 1] = NULL;
	free(type->env_var[j]);
	type->env_var = new;
}

/**
 * counts the nbr of nodes in a token list
 * iterates through the linked list of tokens, printing each
 * token's value and returns the total count,
 * returns 0 if token is NULL.
 */
int	ct_nodes(t_token *token)
{
	if (!token)
    {
        printf("Error: token is NULL in ct_nodes\n");
        return 0;
    }

    int i = 0;
    while (token)
    {
        printf("Token value: %s\n", token->value);
        i++;
        token = token->next;
    }
    return i;
}
