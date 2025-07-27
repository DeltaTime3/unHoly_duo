/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   refractors.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppaula-d <ppaula-d@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 13:59:12 by ppaula-d          #+#    #+#             */
/*   Updated: 2025/07/27 13:50:12 by ppaula-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	operator_type(const char *input, int *i, t_cat *type)
{
	if (!input || !input[*i])
		return (1);
	if (input[*i] == '<' && input[*i + 1] == '<')
	{
		*type = HERE_DOC;
		(*i)++;
	}
	else if (input[*i] == '>' && input[*i + 1] == '>')
	{
		*type = REDIRECT;
		(*i)++;
	}
	else if ((input[*i] == '<' || input[*i] == '>'))
		*type = REDIRECT;
	else
		return (1);
	return (0);
}

int	handle_token_helper(t_token_handler_args *args)
{
	if (args->cond)
	{
		if (args->handler(args->input, args->i, args->tokens))
			return (1);
		if (args->expect_command)
			*(args->expect_command) = args->expect_val;
		return (0);
	}
	return (-1);
}

int	handle_quote_helper(t_quote_handler_args *args)
{
	if (quote_handling(args->input, args->i, args->tokens,
			args->expect_command))
		return (1);
	return (0);
}

int	handle_pipe_case(const char *input, int *i, t_token **tokens,
		int *expect_command)
{
	t_token_handler_args	args;
	int						ret;

	args.cond = (input[*i] == '|');
	args.handler = pipe_handling;
	args.input = input;
	args.i = i;
	args.tokens = tokens;
	args.expect_command = expect_command;
	args.expect_val = 1;
	ret = handle_token_helper(&args);
	return (ret);
}

int	handle_heredoc_case(const char *input, int *i, t_token **tokens,
		int *expect_command)
{
	t_token_handler_args	args;
	int						ret;

	args.cond = (input[*i] == '<' && input[*i + 1] == '<');
	args.handler = heredoc_handling;
	args.input = input;
	args.i = i;
	args.tokens = tokens;
	args.expect_command = expect_command;
	args.expect_val = 0;
	ret = handle_token_helper(&args);
	return (ret);
}
