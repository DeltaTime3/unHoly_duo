/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_helpers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppaula-d <ppaula-d@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 13:59:12 by ppaula-d          #+#    #+#             */
/*   Updated: 2025/07/30 16:32:19 by ppaula-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	init_shell_struct(t_shell *shell, char **envp)
{
	shell->head = init_shell_env(envp);
	shell->exit_code = 0;
	shell->return_code = 0;
	shell->prev_dir = NULL;
	shell->curr_dir = NULL;
	shell->pwd = NULL;
	shell->token = NULL;
	shell->type = NULL;
	shell->tail = NULL;
	shell->value = NULL;
	shell->heredoc_fd = -1;
}

void	handle_input(t_shell *shell, char *input)
{
	t_token	*tokens;

	tokens = NULL;
	if (!validate_input(input))
	{
		tokens = tokenize_input(input);
		if (process_heredocs(&tokens, shell))
		{
			free_tokens(tokens);
			return ;
		}
		if (tokens)
		{
			ft_execute(shell, tokens);
			free_tokens(tokens);
		}
	}
}

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
