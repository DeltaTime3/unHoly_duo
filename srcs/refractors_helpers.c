/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   refractors_helpers.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppaula-d <ppaula-d@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 13:59:12 by ppaula-d          #+#    #+#             */
/*   Updated: 2025/07/27 13:50:14 by ppaula-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_cat	determine_token_type(const char *value, int *expect_command)
{
	t_cat	type;

	if (*expect_command == 1)
	{
		type = COMMAND;
		*expect_command = 2;
	}
	else if (value[0] == '-' && *expect_command == 2)
	{
		type = FLAG;
		*expect_command = 0;
	}
	else
	{
		type = ARGUMENT;
	}
	return (type);
}

int	handle_redirect_case(const char *input, int *i, t_token **tokens)
{
	t_token_handler_args	args;
	int						ret;

	args.cond = (input[*i] == '<' || input[*i] == '>');
	args.handler = op_handling;
	args.input = input;
	args.i = i;
	args.tokens = tokens;
	args.expect_command = NULL;
	args.expect_val = 0;
	ret = handle_token_helper(&args);
	return (ret);
}

int	special_tokens_handling(const char *input, int *i, t_token **tokens,
		int *expect_command)
{
	int						ret;
	t_quote_handler_args	q_args;

	if (!input || !input[*i])
		return (1);
	while (input[*i] && ft_isspace(input[*i]))
		(*i)++;
	ret = handle_pipe_case(input, i, tokens, expect_command);
	if (ret != -1)
		return (ret);
	ret = handle_heredoc_case(input, i, tokens, expect_command);
	if (ret != -1)
		return (ret);
	ret = handle_redirect_case(input, i, tokens);
	if (ret != -1)
		return (ret);
	if (input[*i] == '\'' || input[*i] == '"')
	{
		q_args.input = input;
		q_args.i = i;
		q_args.tokens = tokens;
		q_args.expect_command = expect_command;
		return (handle_quote_helper(&q_args));
	}
	return (0);
}

int	token_helper(const char *input, int *i, t_token **tokens,
		int *expect_command)
{
	int	result;

	while (input[*i])
	{
		while (input[*i] && ft_isspace(input[*i]))
			(*i)++;
		if (!input[*i])
			break ;
		result = token_handling(input, i, tokens, expect_command);
		if (result == 1)
		{
			free_tokens(*tokens);
			*tokens = NULL;
			return (1);
		}
		else if (result == 2)
			break ;
	}
	return (0);
}

void	skip_special_chars(const char *input, int *i)
{
	while (input[*i] && !ft_isspace(input[*i]) && input[*i] != '\''
		&& input[*i] != '"' && input[*i] != '#' && input[*i] != '<'
		&& input[*i] != '>' && input[*i] != '|')
		(*i)++;
}
