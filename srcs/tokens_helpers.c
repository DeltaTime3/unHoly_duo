/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens_helpers.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppaula-d <ppaula-d@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 13:59:12 by ppaula-d          #+#    #+#             */
/*   Updated: 2025/07/27 13:19:58 by ppaula-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*ft_strjoin_free(char *s1, char *s2)
{
	char	*res;

	res = ft_strjoin(s1, s2);
	free(s1);
	return (res);
}

char	*parse_token_part(const char *input, int *i)
{
	char	*tmp;
	size_t	start;
	char	quote;

	tmp = NULL;
	if (input[*i] == '"' || input[*i] == '\'')
	{
		quote = input[(*i)++];
		start = *i;
		while (input[*i] && input[*i] != quote)
			(*i)++;
		tmp = ft_substr(input, start, *i - start);
		if (input[*i] == quote)
			(*i)++;
	}
	else
	{
		start = *i;
		while (input[*i] && !ft_isspace(input[*i])
			&& input[*i] != '"' && input[*i] != '\''
			&& input[*i] != '|' && input[*i] != '<' && input[*i] != '>')
			(*i)++;
		tmp = ft_substr(input, start, *i - start);
	}
	return (tmp);
}

int	file_handling(const char *input, int *i, t_token **tokens)
{
	char	*value;
	char	*tmp;

	while (input[*i] && ft_isspace(input[*i]))
		(*i)++;
	value = ft_strdup("");
	if (!value)
		return (1);
	while (input[*i] && !ft_isspace(input[*i]) && input[*i] != '|'
		&& input[*i] != '<' && input[*i] != '>')
	{
		tmp = parse_token_part(input, i);
		if (!tmp)
			return (free(value), 1);
		value = ft_strjoin_free(value, tmp);
		free(tmp);
	}
	add_token(tokens, create_token(FILES, value));
	free(value);
	return (0);
}

int	op_handling(const char *input, int *i, t_token **tokens)
{
	int		start;
	char	*value;
	t_cat	type;

	start = *i;
	value = NULL;
	if (operator_type(input, i, &type))
		return (1);
	value = ft_substr(input, start, *i - start + 1);
	if (!value || ft_strlen(value) == 0)
	{
		free(value);
		return (1);
	}
	add_token(tokens, create_token(type, value));
	free(value);
	(*i)++;
	while (input[*i] && ft_isspace(input[*i]))
		(*i)++;
	if (type == REDIRECT || type == HERE_DOC)
	{
		if (input[*i] && file_handling(input, i, tokens))
			return (1);
	}
	return (0);
}

void	scan_word_with_quotes(const char *input, int *i)
{
	char	quote_char;

	quote_char = 0;
	while (input[*i])
	{
		if (quote_char)
		{
			if (input[*i] == quote_char)
				quote_char = 0;
		}
		else
		{
			if (input[*i] == '\'' || input[*i] == '"')
				quote_char = input[*i];
			else if (ft_isspace(input[*i]) || input[*i] == '|'
				|| input[*i] == '<' || input[*i] == '>')
				break ;
		}
		(*i)++;
	}
}
