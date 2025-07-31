/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_helpers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppaula-d <ppaula-d@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 13:59:12 by ppaula-d          #+#    #+#             */
/*   Updated: 2025/07/31 23:29:32 by ppaula-d         ###   ########.fr       */
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

static int	is_leading_pipe(char *input)
{
	int	i;

	i = 0;
	while (input[i] == ' ')
		i++;
	return (input[i] == '|');
}

static int	handle_leading_pipe_error(t_shell *shell)
{
	ft_putstr_fd("minishell: syntax error near unexpected token `|'\n", 2);
	shell->exit_code = 2;
	return (1);
}

static int	handle_heredocs_and_execute(t_shell *shell, t_token *tokens)
{
	if (process_heredocs(&tokens, shell))
	{
		free_tokens(tokens);
		return (1);
	}
	if (tokens)
	{
		ft_execute(shell, tokens);
		free_tokens(tokens);
	}
	return (0);
}

void	handle_input(t_shell *shell, char *input)
{
	t_token	*tokens;

	tokens = NULL;
	if (!validate_input(input))
	{
		if (is_leading_pipe(input))
		{
			handle_leading_pipe_error(shell);
			return ;
		}
		tokens = tokenize_input(input);
		handle_heredocs_and_execute(shell, tokens);
	}
}
