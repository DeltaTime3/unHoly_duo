/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppaula-d <ppaula-d@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 13:59:12 by ppaula-d          #+#    #+#             */
/*   Updated: 2025/07/27 14:18:46 by ppaula-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token	*create_token(t_cat type, char *value)
{
	t_token	*token;

	token = ft_calloc(1, sizeof(t_token));
	if (!token)
		return (NULL);
	token->type = type;
	token->value = ft_strdup(value);
	token->content = NULL;
	token->args = NULL;
	token->next = NULL;
	token->command = NULL;
	token->expand_heredoc = 0;
	token->was_expanded = 0;
	token->in_single_quotes = 0;
	return (token);
}

static int	is_all_ws(const char *input)
{
	int	i;

	i = 0;
	while (input[i])
	{
		if (!ft_isspace(input[i]))
			return (0);
		i++;
	}
	return (1);
}

int	init_tokenize_vars(const char *input, t_token ***tokens,
	int *i, int *expect_command)
{
	*tokens = malloc(sizeof(t_token *));
	if (!*tokens)
		return (0);
	**tokens = NULL;
	*i = 0;
	*expect_command = 1;
	if (!input || ft_strlen(input) == 0 || is_all_ws(input))
	{
		free(*tokens);
		return (0);
	}
	return (1);
}

int	tokenize_loop(const char *input, int *i, t_token **tokens,
	int *expect_command)
{
	int	result;

	while (input[*i])
	{
		result = token_helper(input, i, tokens, expect_command);
		if (result == 1)
			return (1);
		else if (result == 2)
			break ;
	}
	return (0);
}

t_token	*tokenize_input(const char *input)
{
	t_token	**tokens;
	t_token	*final_tokens;
	int		i;
	int		expect_command;

	if (!init_tokenize_vars(input, &tokens, &i, &expect_command))
		return (NULL);
	if (tokenize_loop(input, &i, tokens, &expect_command))
	{
		free(tokens);
		return (NULL);
	}
	final_tokens = *tokens;
	free(tokens);
	prep_cmd_args(final_tokens);
	return (final_tokens);
}
