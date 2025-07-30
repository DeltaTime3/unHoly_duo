/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc_helpers.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppaula-d <ppaula-d@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 13:59:12 by ppaula-d          #+#    #+#             */
/*   Updated: 2025/07/26 18:26:57 by ppaula-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	process_delimiter_quotes(char *raw_delimiter, char *final_delimiter,
		int *expand)
{
	int		j;
	int		k;
	char	quote_char;

	j = 0;
	k = 0;
	while (raw_delimiter[j])
	{
		if (raw_delimiter[j] == '\'' || raw_delimiter[j] == '\"')
		{
			*expand = 0;
			quote_char = raw_delimiter[j];
			j++;
			while (raw_delimiter[j] && raw_delimiter[j] != quote_char)
				final_delimiter[k++] = raw_delimiter[j++];
			if (raw_delimiter[j] == quote_char)
				j++;
		}
		else
			final_delimiter[k++] = raw_delimiter[j++];
	}
	final_delimiter[k] = '\0';
}

char	*parse_heredoc_delimiter(const char *input, int *i, int *expand)
{
	int		start;
	char	*raw_delimiter;
	char	*final_delimiter;

	while (input[*i] && ft_isspace(input[*i]))
		(*i)++;
	start = *i;
	*expand = 1;
	scan_word_with_quotes(input, i);
	raw_delimiter = ft_substr(input, start, *i - start);
	if (!raw_delimiter)
		return (NULL);
	final_delimiter = malloc(ft_strlen(raw_delimiter) + 1);
	if (!final_delimiter)
	{
		free(raw_delimiter);
		return (NULL);
	}
	process_delimiter_quotes(raw_delimiter, final_delimiter, expand);
	free(raw_delimiter);
	return (final_delimiter);
}

static int	handle_heredoc_token(t_token *token, t_shell *shell)
{
	char	*content;
	int		pipe_fd;

	content = read_heredoc_input(token->next->value,
			token->next->expand_heredoc, shell);
	if (g_global_sig)
	{
		free(content);
		shell->exit_code = 130;
		g_global_sig = 0;
		return (1);
	}
	pipe_fd = write_heredoc_to_pipe(content);
	free(content);
	if (pipe_fd == -1)
		return (1);
	token->heredoc_fd = pipe_fd;
	return (0);
}

int	process_heredocs(t_token **tokens, t_shell *shell)
{
	t_token	*current;

	current = *tokens;
	while (current)
	{
		if (current->type == HERE_DOC)
		{
			if (!current->next || current->next->type != DELIMETER)
				return (1);
			if (handle_heredoc_token(current, shell))
				return (1);
		}
		current = current->next;
	}
	return (0);
}
