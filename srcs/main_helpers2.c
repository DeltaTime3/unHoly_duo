/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_helpers2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppaula-d <ppaula-d@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 13:59:12 by ppaula-d          #+#    #+#             */
/*   Updated: 2025/07/31 23:28:39 by ppaula-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_eof(char *input)
{
	if (!input)
	{
		write(STDOUT_FILENO, "exit\n", 5);
		return (1);
	}
	return (0);
}

void	main_loop(t_shell *shell)
{
	char	*input;

	input = NULL;
	while (1)
	{
		input = readline("minishell> ");
		signal_process(shell);
		if (handle_eof(input))
			break ;
		if (ft_strlen(input) != 0)
			handle_input(shell, input);
		free(input);
		input = NULL;
	}
}

void	cleanup_shell(t_shell *shell)
{
	clean_all_resources(shell);
	rl_clear_history();
	rl_cleanup_after_signal();
}
