/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc_helpers4.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppaula-d <ppaula-d@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 13:59:12 by ppaula-d          #+#    #+#             */
/*   Updated: 2025/07/31 23:37:41 by ppaula-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_delimiter_or_interrupt(char *line, const char *delimiter)
{
	int	check;

	check = is_interrupt_or_delimiter(line, delimiter);
	return (check);
}

static int	process_heredoc_line(char **content, char *line, int expand,
	t_shell *shell)
{
	char	*expanded;

	expanded = maybe_expand_line(line, expand, shell);
	if (!append_line_to_content(content, expanded))
	{
		free(expanded);
		return (0);
	}
	free(expanded);
	return (1);
}

static int	handle_heredoc_end(char **content)
{
	if (g_global_sig == 130)
	{
		free(*content);
		*content = NULL;
		return (0);
	}
	return (1);
}

static int	handle_heredoc_iteration(char **content, const char *delimiter,
	int expand, t_shell *shell)
{
	char	*line;
	int		check;

	line = prompt_and_read_line();
	if (!line)
	{
		if (g_global_sig != 130)
			ft_putstr_fd("minishell: warning: here-document delimited by"
				" end-of-file\n", 2);
		return (2);
	}
	check = is_delimiter_or_interrupt(line, delimiter);
	if (check == 1)
		return (handle_heredoc_interrupt(line, content));
	if (check == 2)
	{
		free(line);
		return (2);
	}
	if (!process_heredoc_line(content, line, expand, shell))
		return (0);
	return (1);
}

int	read_heredoc_loop(char **content, const char *delimiter, int expand,
	t_shell *shell)
{
	int	status;

	signal(SIGINT, handle_sig_heredoc);
	while (1)
	{
		status = handle_heredoc_iteration(content, delimiter, expand, shell);
		if (status == 0 || status == 1)
		{
			if (status == 0)
				return (0);
		}
		else
			break ;
	}
	signal(SIGINT, handle_sig_int);
	return (handle_heredoc_end(content));
}
