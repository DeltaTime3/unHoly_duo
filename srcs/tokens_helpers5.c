/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens_helpers5.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppaula-d <ppaula-d@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 13:59:12 by ppaula-d          #+#    #+#             */
/*   Updated: 2025/07/27 14:18:35 by ppaula-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	redirect_handling(t_token *tokens, t_shell *shell)
{
	int		fd_in;
	int		fd_out;

	fd_in = -1;
	fd_out = -1;
	if (process_redirects_loop(tokens, &fd_in, &fd_out, shell) == -1)
		return (-1);
	if (fd_in != -1)
	{
		if (dup2_and_close(fd_in, STDIN_FILENO, &fd_out) == -1)
			return (-1);
	}
	if (fd_out != -1)
	{
		if (dup2_and_close(fd_out, STDOUT_FILENO, NULL) == -1)
			return (-1);
	}
	return (0);
}

char	*parse_heredoc_delimiter(const char *input, int *i, int *expand)
{
	int		start;
	char	*delimiter;
	char	*temp;

	while (input[*i] && ft_isspace(input[*i]))
		(*i)++;
	start = *i;
	while (input[*i] && !ft_isspace(input[*i]) && input[*i] != '|'
		&& input[*i] != '<' && input[*i] != '>')
		(*i)++;
	delimiter = ft_substr(input, start, *i - start);
	if (!delimiter)
		return (NULL);
	*expand = 1;
	if ((delimiter[0] == '\'' && delimiter[ft_strlen(delimiter) - 1] == '\'')
		|| (delimiter[0] == '"' && delimiter[ft_strlen(delimiter) - 1] == '"'))
	{
		temp = ft_strtrim(delimiter, "\'\"");
		free(delimiter);
		delimiter = temp;
		*expand = 0;
	}
	return (delimiter);
}

int	heredoc_handling(const char *input, int *i, t_token **tokens)
{
	char	*delimiter;
	int		expand;
	t_token	*heredoc_token;
	t_token	*delimiter_token;

	(*i) += 2;
	delimiter = parse_heredoc_delimiter(input, i, &expand);
	if (!delimiter)
		return (1);
	heredoc_token = create_token(HERE_DOC, "<<");
	add_token(tokens, heredoc_token);
	delimiter_token = create_token(DELIMETER, delimiter);
	delimiter_token->expand_heredoc = expand;
	add_token(tokens, delimiter_token);
	free(delimiter);
	return (0);
}

int	token_handling(const char *input, int *i, t_token **tokens,
		int *expect_command)
{
	if (!input || !input[*i])
		return (1);
	if (input[*i] == '|' || input[*i] == '<' || input[*i] == '>')
	{
		if (special_tokens_handling(input, i, tokens, expect_command))
			return (1);
		if (input[*i - 1] == '|')
			*expect_command = 1;
	}
	else if (input[*i] == '#')
	{
		while (input[*i] && input[*i] != '\n')
			(*i)++;
	}
	else if (ft_isprint(input[*i]) && !ft_isspace(input[*i]))
	{
		if (word_handling(input, i, tokens, expect_command))
			return (1);
	}
	else
	{
		(*i)++;
	}
	return (0);
}
