/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes_handlers2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppaula-d <ppaula-d@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 13:59:12 by ppaula-d          #+#    #+#             */
/*   Updated: 2025/07/27 14:33:01 by ppaula-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	init_pipe_data(t_pipe_data *data, t_token *tokens, t_shell *shell)
{
	data->pipe_count = count_pipes(tokens);
	data->saved_fds[0] = dup(STDIN_FILENO);
	data->saved_fds[1] = dup(STDOUT_FILENO);
	data->in_fd = data->saved_fds[0];
	data->current = tokens;
	data->tokens = tokens;
	data->i = 0;
	data->last_pid = -1;
	data->shell = shell;
}

int	execute_all_pipes(t_pipe_data *data, int *last_status, int *err)
{
	while (data->i <= data->pipe_count)
	{
		if (execute_pipe_command(data) == -1)
		{
			restore_and_close_stdio(data->saved_fds);
			*err = 1;
			return (-1);
		}
		data->i++;
	}
	*last_status = wait_for_children(data->last_pid);
	return (0);
}

int	handle_pipes(t_token *tokens, t_shell *shell)
{
	t_pipe_data	data;
	int			last_status;
	int			err;

	init_pipe_data(&data, tokens, shell);
	if (data.saved_fds[0] < 0 || data.saved_fds[1] < 0)
	{
		print_error("dup failed\n");
		if (data.saved_fds[0] >= 0)
			close(data.saved_fds[0]);
		if (data.saved_fds[1] >= 0)
			close(data.saved_fds[1]);
		return (-1);
	}
	err = 0;
	if (execute_all_pipes(&data, &last_status, &err) == -1)
		return (-1);
	set_exit_code(shell, last_status);
	restore_and_close_stdio(data.saved_fds);
	if (err)
		return (-1);
	return (0);
}
