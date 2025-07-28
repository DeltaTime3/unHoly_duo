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

int	is_interrupt_or_delimiter(char *line, const char *delimiter)
{
	if (!line || g_global_sig == 1)
		return (1);
	if (ft_strcmp(line, delimiter) == 0)
		return (2);
	return (0);
}

static void	*handle_null_content(char *content, int stdin_backup)
{
	if (!content)
	{
		close(stdin_backup);
		signal(SIGINT, handle_sig_int);
		return (NULL);
	}
	return ((void *)content);
}

char	*read_heredoc_input(const char *delimiter, int expand, t_shell *shell)
{
	char	*content;
	int		stdin_backup;
	int		read_loop_ret;

	stdin_backup = dup(STDIN_FILENO);
	if (stdin_backup == -1)
		return (NULL);
	g_global_sig = 0;
	signal(SIGINT, handle_sig_heredoc);
	signal(SIGQUIT, SIG_IGN);
	content = ft_strdup("");
	if (!content)
		return (handle_null_content(content, stdin_backup));
	read_loop_ret = read_heredoc_loop(&content, delimiter, expand, shell);
	dup2(stdin_backup, STDIN_FILENO);
	close(stdin_backup);
	signal(SIGINT, handle_sig_int);
	signal(SIGQUIT, SIG_IGN);
	if (!read_loop_ret)
		return (NULL);
	g_global_sig = 0;
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
