/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exect_helpers2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppaula-d <ppaula-d@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 13:59:12 by ppaula-d          #+#    #+#             */
/*   Updated: 2025/07/26 15:05:19 by ppaula-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_dotdot_cmd(t_shell *shell, int saved_stdout, int saved_stdin)
{
	ft_printf_fd(2, "minishell: ..: command not found\n");
	shell->exit_code = 127;
	restore_fds(saved_stdout, saved_stdin);
	return (127);
}

int	handle_pipes_n_restore(t_token *value, t_shell *shell, int out, int in)
{
	handle_pipes(value, shell);
	restore_fds(out, in);
	return (shell->exit_code);
}

int	handle_red_n_restore(t_token *value, t_shell *shell, int out, int in)
{
	if (redirect_handling(value, shell) == -1)
	{
		restore_fds(out, in);
		return (shell->exit_code);
	}
	return (0);
}

void	exec_cmd_or_b_in(t_shell *shell, t_token *cmd)
{
	if (is_builtin(cmd))
		choose_b_in(cmd, shell);
	else
		execute2(shell, cmd);
}

int	print_cmd_not_found(t_shell *shell, t_token *token)
{
	ft_printf_fd(2, "minishell: %s: command not found\n", token->value);
	shell->exit_code = 127;
	return (127);
}

int	has_pipe_after(t_token *token)
{
	t_token	*current;

	current = token;
	while (current)
	{
		if (current->type == PIPE)
			return (1);
		current = current->next;
	}
	return (0);
}
