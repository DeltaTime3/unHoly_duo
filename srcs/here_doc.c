/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppaula-d <ppaula-d@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 13:59:12 by ppaula-d          #+#    #+#             */
/*   Updated: 2025/07/31 23:38:44 by ppaula-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_heredoc_interrupt(char *line, char **content)
{
	free(line);
	free(*content);
	*content = NULL;
	g_global_sig = 130;
	signal(SIGINT, handle_sig_int);
	signal(SIGQUIT, SIG_IGN);
	return (2);
}

int	join_and_check(char **content, const char *to_add)
{
	char	*temp;

	temp = ft_strjoin(*content, to_add);
	if (!temp)
	{
		free(*content);
		*content = NULL;
		signal(SIGINT, handle_sig_int);
		return (0);
	}
	free(*content);
	*content = temp;
	return (1);
}

char	*maybe_expand_line(char *line, int expand, t_shell *shell)
{
	char	*expanded_line;

	if (expand)
	{
		expanded_line = expand_variables(line, shell);
		free(line);
		return (expanded_line);
	}
	return (line);
}

int	append_line_to_content(char **content, char *line)
{
	if (!join_and_check(content, line) || !join_and_check(content, "\n"))
		return (0);
	return (1);
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
