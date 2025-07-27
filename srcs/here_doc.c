/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppaula-d <ppaula-d@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 13:59:12 by ppaula-d          #+#    #+#             */
/*   Updated: 2025/07/26 18:28:40 by ppaula-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_heredoc_interrupt(char *line, char **content)
{
	free(line);
	free(*content);
	signal(SIGINT, handle_sig_int);
	return (0);
}

int	read_heredoc_loop(char **content, const char *delimiter, int expand,
		t_shell *shell)
{
	char	*line;
	char	*expanded_line;

	while (1)
	{
		line = readline("> ");
		if (!line || g_global_sig == 1)
			return (handle_heredoc_interrupt(line, content));
		if (ft_strcmp(line, delimiter) == 0)
		{
			free(line);
			break ;
		}
		if (expand)
		{
			expanded_line = expand_token_value(line, shell);
			free(line);
			line = expanded_line;
		}
		if (!join_and_check(content, line, line)
			|| !join_and_check(content, "\n", line))
			return (0);
		free(line);
	}
	return (1);
}

int	join_and_check(char **content, const char *to_add, char *line)
{
	char	*temp;

	temp = ft_strjoin(*content, to_add);
	free(*content);
	if (!temp)
	{
		free(line);
		signal(SIGINT, handle_sig_int);
		return (0);
	}
	*content = temp;
	return (1);
}

char	*read_heredoc_input(const char *delimiter, int expand, t_shell *shell)
{
	char	*content;

	g_global_sig = 0;
	signal(SIGINT, handle_sig_heredoc);
	content = ft_strdup("");
	if (!content)
	{
		signal(SIGINT, handle_sig_int);
		return (NULL);
	}
	if (!read_heredoc_loop(&content, delimiter, expand, shell))
		return (NULL);
	signal(SIGINT, handle_sig_int);
	return (content);
}

int	is_sp_expantion(t_token *token)
{
	const char	*value;

	value = token->value;
	if (!value || !*value)
		return (0);
	if (value[0] == '~' && (value[1] == '\0' || value[1] == '/'))
		return (1);
	if (value[0] == '$')
		return (is_dollar_expansion(value));
	return (0);
}
