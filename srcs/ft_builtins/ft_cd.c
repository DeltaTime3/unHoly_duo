#include "../../include/minishell.h"

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
	int		arg_count;

	is_cd_minus = 0;
	if (!token || !token->args || !token->args[0])
		return ;
	arg_count = ct_nodes(token);
	if (arg_count > 2)
	{
		ft_printf_fd(2, "cd: too many arguments\n");
		type->exit_code = 1;
		type->r_code = 1;
		return ;
	}
	new_dir = get_cd_target(token, type, &is_cd_minus);
	if (!new_dir)
	{
		ft_printf_fd(2, "cd: Failed to change directory.\n", 1);
		type->r_code = 1;
		return ;
	}
	cd_change_dir(new_dir, type, is_cd_minus);
}

void	cd_change_dir(char *new, t_shell *type, int is_cd_minus)
{
	if (check_dir(new))
	{
		change_dir(new, type);
		if (is_cd_minus)
			printf("%s\n", type->curr_dir);
	}
	else
	{
		type->r_code = 1;
		free(new);
	}
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
	char	cwd[MAX_PATH];

	free(type->prev_dir);
	type->prev_dir = ft_strdup(type->curr_dir);
	free(type->curr_dir);
	type->curr_dir = ft_strdup(new_dir);
	if (getcwd(cwd, sizeof(cwd)))
	{
		free(type->pwd);
		type->pwd = ft_strdup(cwd);
	}
	else
		print_error("error retrieving current directory.\n");
	cd_env(type);
	if (new_dir && new_dir != type->curr_dir)
	{
		free(new_dir);
		new_dir = NULL;
	}
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
	if (!token->args[1] || ft_strcmp(token->args[1], "~") == 0)
	{
		temp = get_env_value(type->head, "HOME");
		if (temp)
			return (ft_strdup(temp));
		else
			return (NULL);
	}
	else if (ft_strcmp(token->args[1], "-") == 0)
	{
		*is_cd_minus = 1;
		temp = get_env_value(type->head, "OLDPWD");
		if (temp)
			return (ft_strdup(temp));
		return (NULL);
	}
	else if (ft_strcmp(token->args[1], ".") == 0)
		return (ft_strdup(type->curr_dir));
	return (ft_strdup(token->args[1]));
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
		home = get_env_value(type->head, "HOME");
		if (home)
			*new_dir = ft_strdup(home);
		else
			*new_dir = NULL;
		return (1);
	}
	return (1);
}
