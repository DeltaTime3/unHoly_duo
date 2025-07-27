/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppaula-d <ppaula-d@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 13:59:12 by ppaula-d          #+#    #+#             */
/*   Updated: 2025/07/26 15:57:09 by ppaula-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
