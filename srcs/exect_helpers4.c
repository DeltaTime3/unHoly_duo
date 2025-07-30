/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exect_helpers4.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afilipe- <afilipe-@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 13:59:12 by ppaula-d          #+#    #+#             */
/*   Updated: 2025/07/29 13:58:37 by afilipe-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	pid_zero(char *full_path, char **env_array, t_token *token,
	t_shell *shell)
{
	int	err;

	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	execve(full_path, token->args, env_array);
	free(full_path);
	err = errno;
	free_env_array(env_array);
	handle_exec_error(err, token, shell);
}

int	pid_neg(char *full_path, char **env_array)
{
	perror("fork");
	free(full_path);
	free_env_array(env_array);
	return (-1);
}

int	pid_else(char *full_path, char **env_array, t_shell *shell, pid_t pid)
{
	int	sts;

	(void)full_path;
	waitpid(pid, &sts, 0);
	if (WIFSIGNALED(sts))
	{
		if (WTERMSIG(sts) == SIGINT)
			shell->exit_code = 130;
		else if (WTERMSIG(sts) == SIGQUIT)
		{
			ft_putstr_fd("Quit (core dumped)", 2);
			shell->exit_code = 131;
		}
		ft_putstr_fd("\n", 2);
	}
	else
		shell->exit_code = WEXITSTATUS(sts);
	free(full_path);
	free_env_array(env_array);
	return (0);
}

char	*get_next_path(char **path_temp)
{
	char	*start;
	char	*end;
	char	*dir;
	int		len;

	start = *path_temp;
	if (!start || !*start)
		return (NULL);
	end = ft_strchr(start, ':');
	if (end != NULL)
		len = end - start;
	else
		len = ft_strlen(start);
	dir = malloc(len + 1);
	if (!dir)
		return (NULL);
	ft_strlcpy(dir, start, len + 1);
	dir[len] = '\0';
	if (end)
		*path_temp = end + 1;
	else
		*path_temp = start + len;
	return (dir);
}

char	*build_cmb_path(char *dir, char *cmd)
{
	char	*full_path;
	size_t	dir_len;
	size_t	cmd_len;
	size_t	total_len;

	dir_len = ft_strlen(dir);
	cmd_len = ft_strlen(cmd);
	if (!dir[0])
		total_len = cmd_len + 3;
	else
		total_len = dir_len + cmd_len + 2;
	full_path = ft_calloc(1, total_len);
	if (!full_path)
		return (NULL);
	if (!dir[0])
		ft_strlcpy(full_path, "./", 3);
	else
	{
		ft_strlcpy(full_path, dir, total_len);
		ft_strlcat(full_path, "/", total_len);
	}
	ft_strlcat(full_path, cmd, total_len);
	return (full_path);
}
