/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afilipe- <afilipe-@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 13:59:12 by ppaula-d          #+#    #+#             */
/*   Updated: 2025/07/28 11:38:27 by afilipe-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_signals(void)
{
	signal(SIGINT, handle_sig_int);
	signal(SIGQUIT, SIG_IGN);
}

void	handle_sig_int(int sig)
{
	if (sig == SIGINT)
	{
		write(STDOUT_FILENO, "\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
		g_global_sig = 130;
	}
}

void	handle_sig_heredoc(int sig)
{
	if (sig == SIGINT)
	{
		write(STDOUT_FILENO, "\n", 1);
		g_global_sig = 1;
		close(STDIN_FILENO);
	}
}

void	signal_process(t_shell *shell)
{
	if (g_global_sig == 130)
	{
		shell->exit_code = 130;
		g_global_sig = 0;
	}
}

void	clean_exit(char *input)
{
	if (input)
		free(input);
	rl_clear_history();
	exit(0);
}
