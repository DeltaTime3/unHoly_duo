/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes_handlers.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppaula-d <ppaula-d@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 13:59:12 by ppaula-d          #+#    #+#             */
/*   Updated: 2025/07/27 14:29:52 by ppaula-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	parent_process_logic(t_pipe_data *data)
{
	if (data->i > 0)
		close(data->in_fd);
	if (data->i < data->pipe_count)
	{
		close(data->pipe_fd[1]);
		data->in_fd = data->pipe_fd[0];
	}
	data->last_pid = data->pid;
	if (data->i < data->pipe_count)
		data->current = find_next_cmd_after_pipe(data->current);
}

int	execute_pipe_command(t_pipe_data *data)
{
	if (data->i < data->pipe_count)
	{
		if (pipe(data->pipe_fd) < 0)
			return (print_error("Pipe creation failed\n"), -1);
	}
	data->pid = fork();
	if (data->pid < 0)
		return (print_error("Fork failed\n"), -1);
	else if (data->pid == 0)
		child_process_logic(data);
	else
		parent_process_logic(data);
	return (0);
}

int	wait_for_children(pid_t last_pid)
{
	pid_t	wpid;
	int		status;
	int		last_status;

	last_status = 0;
	wpid = wait(&status);
	while (wpid > 0)
	{
		if (wpid == last_pid)
			last_status = status;
		wpid = wait(&status);
	}
	return (last_status);
}

void	set_exit_code(t_shell *shell, int last_status)
{
	if (WIFEXITED(last_status))
		shell->exit_code = WEXITSTATUS(last_status);
	else if (WIFSIGNALED(last_status))
		shell->exit_code = 128 + WTERMSIG(last_status);
}

void	restore_and_close_stdio(int saved_fds[2])
{
	dup2(saved_fds[0], STDIN_FILENO);
	dup2(saved_fds[1], STDOUT_FILENO);
	close(saved_fds[0]);
	close(saved_fds[1]);
}
