/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils3.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppaula-d <ppaula-d@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 13:59:12 by ppaula-d          #+#    #+#             */
/*   Updated: 2025/07/27 14:08:30 by ppaula-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	create_and_add_token_struct(t_token_create_args *args)
{
	char	*full_word_content;
	t_cat	type;
	t_token	*new_token;

	full_word_content = ft_strjoin(args->quoted_content, args->unquoted_suffix);
	if (!full_word_content)
		return (0);
	type = determine_token_type(full_word_content, args->expect_command);
	new_token = create_token(type, full_word_content);
	if (args->quote == '\'')
		new_token->in_single_quotes = 1;
	add_token(args->tokens, new_token);
	free(full_word_content);
	return (1);
}

int	init_quote_args(const char *input, int *i, char **quoted_content,
	char **unquoted_suffix)
{
	char	quote;
	int		ok;

	quote = input[*i];
	ok = get_quoted_content(input, i, quote, quoted_content);
	if (!ok)
		return (0);
	ok = get_unquoted_suffix(input, i, unquoted_suffix);
	if (!ok)
	{
		free(*quoted_content);
		return (0);
	}
	return (1);
}

int	handle_token_creation(t_token_create_args *args)
{
	int	ok;

	ok = create_and_add_token_struct(args);
	return (ok);
}

int	quote_handling(const char *input, int *i, t_token **tokens,
	int *expect_command)
{
	char				quote;
	char				*quoted_content;
	char				*unquoted_suffix;
	int					ok;
	t_token_create_args	args;

	quote = input[*i];
	ok = init_quote_args(input, i, &quoted_content, &unquoted_suffix);
	if (!ok)
		return (handle_quote_error(tokens));
	args.quoted_content = quoted_content;
	args.unquoted_suffix = unquoted_suffix;
	args.quote = quote;
	args.tokens = tokens;
	args.expect_command = expect_command;
	ok = handle_token_creation(&args);
	free(quoted_content);
	free(unquoted_suffix);
	if (!ok)
		return (handle_quote_error(tokens));
	return (0);
}

bool	open_pipe(const char *input, int i)
{
	if (input[i] == '|')
	{
		i++;
		while (input[i] == ' ')
			i++;
		if (input[i] == '\0')
			return (true);
	}
	return (false);
}
