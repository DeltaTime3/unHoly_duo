/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afilipe- <afilipe-@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 13:59:12 by ppaula-d          #+#    #+#             */
/*   Updated: 2025/07/29 14:02:07 by afilipe-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	token_counter(t_token *token)
{
	int		count;
	t_token	*curr;

	count = 0;
	curr = token;
	while (curr)
	{
		count++;
		curr = curr->next;
	}
	return (count);
}

int	is_valid_exit_arg(char *arg)
{
	if (!arg)
		return (0);
	if (ft_strcmp(arg, "|") == 0)
		return (0);
	return (1);
}

void	kill_exit(t_shell *shell, int exit_code)
{
	if (!shell)
	{
		ft_printf("kill_exit: shell is invalid");
		exit(exit_code);
	}
	if (shell->head_tokens)
		free_tokens(shell->head_tokens);
	clean_all_resources(shell);
	rl_clear_history();
	ft_rl_cleanup_after_signal();
	exit(exit_code);
}

void	ft_rl_cleanup_after_signal(void)
{
	struct termios	term;

	if (tcgetattr(STDIN_FILENO, &term) == 0)
	{
		term.c_cflag |= (ICANON | ECHO);
		tcsetattr(STDIN_FILENO, TCSANOW, &term);
	}
	write(STDOUT_FILENO, "\n", 1);
}
