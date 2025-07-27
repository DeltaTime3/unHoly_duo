/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppaula-d <ppaula-d@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 13:59:12 by ppaula-d          #+#    #+#             */
/*   Updated: 2025/07/27 14:29:56 by ppaula-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token	*find_next_cmd_after_pipe(t_token *current)
{
	while (current && current->type != PIPE)
		current = current->next;
	if (current && current->type == PIPE)
		return (current->next);
	return (NULL);
}

int	count_pipes(t_token *tokens)
{
	int		count;
	t_token	*current;

	count = 0;
	current = tokens;
	while (current)
	{
		if (current->type == PIPE)
			count++;
		current = current->next;
	}
	return (count);
}

void	child_process_exit(t_shell *shell, t_token *tokens, int exit_code)
{
	int	code;

	code = exit_code;
	if (shell)
	{
		code = shell->exit_code;
		clean_all_resources(shell);
	}
	if (tokens)
		free_tokens(tokens);
	exit(code);
}

t_token	*find_command_start(t_token *tokens)
{
	t_token	*current;

	current = tokens;
	while (current && current->type != PIPE)
	{
		if (current->type == REDIRECT || current->type == HERE_DOC)
		{
			if (current->next)
				current = current->next->next;
			else
				return (NULL);
		}
		else
		{
			return (current);
		}
	}
	return (NULL);
}

void	child_process_logic(t_pipe_data *data)
{
	t_token	*command_start;

	if (data->i > 0)
	{
		dup2(data->in_fd, STDIN_FILENO);
		close(data->in_fd);
	}
	if (data->i < data->pipe_count)
	{
		close(data->pipe_fd[0]);
		dup2(data->pipe_fd[1], STDOUT_FILENO);
		close(data->pipe_fd[1]);
	}
	if (redirect_handling(data->current, data->shell) == -1)
		child_process_exit(data->shell, data->tokens, 1);
	command_start = find_command_start(data->current);
	if (command_start)
	{
		if (is_builtin(command_start))
			choose_b_in(command_start, data->shell);
		else
			execute2(data->shell, command_start);
	}
	child_process_exit(data->shell, data->tokens, data->shell->exit_code);
}
