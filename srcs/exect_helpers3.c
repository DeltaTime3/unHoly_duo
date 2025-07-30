/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exect_helpers3.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afilipe- <afilipe-@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 13:59:12 by ppaula-d          #+#    #+#             */
/*   Updated: 2025/07/29 12:13:20 by afilipe-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_exe2_err(t_shell *shell, t_token *token, char *full_path)
{
	if (!token || !token->value)
		return (0);
	if (token->value[0] == '\0')
		return (print_cmd_not_found(shell, token));
	if (!full_path)
	{
		if (ft_strchr(token->value, '/'))
			ft_printf_fd(2, "minishell: %s: No such file or directory\n",
				token->value);
		else
			ft_printf_fd(2, "minishell: %s: command not found\n", token->value);
		shell->exit_code = 127;
		return (127);
	}
	if (ft_strcmp(full_path, ":permission_denied:") == 0)
	{
		ft_printf_fd(2, "minishell: %s: Permission denied\n", token->value);
		shell->exit_code = 126;
		free(full_path);
		return (126);
	}
	return (-1);
}

void	handle_exec_error(int err, t_token *token, t_shell *shell )
{
	if (err == EACCES || err == EISDIR)
	{
		ft_printf_fd(2, " Is a directory\n");
		kill_exit(shell, 126);
	}
	if (err == ENOENT)
	{
		if (ft_strchr(token->value, '/'))
			ft_printf_fd(2, "minishell: No such file of directory\n");
		else
			ft_printf_fd(2, "minishell: command not found\n");
		kill_exit(shell, 127);
	}
	perror(token->value);
	kill_exit(shell, 1);
}

char	*handle_cmd_slash(char *cmd)
{
	if (access(cmd, F_OK) == 0)
	{
		if (access(cmd, X_OK) == 0)
			return (ft_strdup(cmd));
		else
			return (ft_strdup(":permission_denied:"));
	}
	return (NULL);
}

char	*handle_no_path(char *cmd)
{
	char	*fallback_path;

	fallback_path = fallback(cmd);
	if (fallback_path)
		return (fallback_path);
	if (access(cmd, X_OK) == 0)
		return (ft_strdup(cmd));
	return (NULL);
}

char	*check_path_dir(char *pth_cpy, char *cmd)
{
	char	*dir;
	char	*full_path;
	char	*res;

	res = NULL;
	dir = get_next_path(&pth_cpy);
	while (dir)
	{
		full_path = build_cmb_path(dir, cmd);
		free(dir);
		if (full_path && access(full_path, X_OK) == 0)
		{
			res = full_path;
			break ;
		}
		free(full_path);
		dir = get_next_path(&pth_cpy);
	}
	return (res);
}
