/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exect_helpers.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppaula-d <ppaula-d@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 13:59:12 by ppaula-d          #+#    #+#             */
/*   Updated: 2025/07/26 15:04:04 by ppaula-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	restore_fds(int saved_stdout, int saved_stdin)
{
	dup2(saved_stdout, STDOUT_FILENO);
	dup2(saved_stdin, STDIN_FILENO);
	close(saved_stdout);
	close(saved_stdin);
}

t_token	*find_valid_cmd(t_token *cmd)
{
	while (cmd && (cmd->type != COMMAND
			|| (cmd->value && cmd->value[0] == '\0' && cmd->was_expanded)))
		cmd = cmd->next;
	return (cmd);
}

int	handle_special_cmds(t_shell *shell, t_token *cmd, int out, int in)
{
	if (cmd && (!cmd->value || cmd->value[0] == '\0'))
		return (handle_empty_cmd(shell, out, in));
	if (!cmd || !cmd->value || cmd->value[0] == '\0')
	{
		restore_fds(out, in);
		shell->exit_code = 0;
		return (0);
	}
	if (ft_strcmp(cmd->value, ".") == 0)
		return (handle_dot_cmd(shell, out, in));
	if (ft_strcmp(cmd->value, "..") == 0)
		return (handle_dotdot_cmd(shell, out, in));
	return (-1);
}

int	handle_empty_cmd(t_shell *shell, int saved_stdout, int saved_stdin)
{
	ft_printf_fd(2, "minishell: : command not found\n");
	shell->exit_code = 127;
	restore_fds(saved_stdout, saved_stdin);
	return (127);
}

int	handle_dot_cmd(t_shell *shell, int saved_stdout, int saved_stdin)
{
	ft_printf_fd(2, "minishell: .: filename argument required\n");
	ft_printf_fd(2, ".: usage: . filename [arguments]\n");
	shell->exit_code = 2;
	restore_fds(saved_stdout, saved_stdin);
	return (2);
}
