/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afilipe- <afilipe-@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 13:59:12 by ppaula-d          #+#    #+#             */
<<<<<<< HEAD
/*   Updated: 2025/07/30 17:17:08 by ppaula-d         ###   ########.fr       */
=======
/*   Updated: 2025/07/28 11:35:38 by afilipe-         ###   ########.fr       */
>>>>>>> 9420e9937aaad08a295e33abf2bfb30333432d20
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

#include "minishell.h"

int	handle_heredoc_interrupt(char *line, char **content)
{
	free(line);
	free(*content);
	*content = NULL;
<<<<<<< HEAD
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
=======
	signal(SIGINT, handle_sig_int);
	signal(SIGQUIT, SIG_IGN);
	return (0);
>>>>>>> 9420e9937aaad08a295e33abf2bfb30333432d20
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

int	read_heredoc_loop(char **content, const char *delimiter, int expand,
		t_shell *shell)
{
	char	*line;
	int		check;

	signal(SIGINT, handle_sig_heredoc);
	while (1)
	{
		line = prompt_and_read_line();
<<<<<<< HEAD
		if (!line)
		{
			if (g_global_sig != 130)
				ft_putstr_fd("minishell: warning: here-document "
					"delimited by end-of-file\n", 2);
			break ;
		}
=======
>>>>>>> 9420e9937aaad08a295e33abf2bfb30333432d20
		check = is_interrupt_or_delimiter(line, delimiter);
		if (check == 1)
			return (handle_heredoc_interrupt(line, content));
		if (check == 2)
		{
			free(line);
			break ;
		}
		line = maybe_expand_line(line, expand, shell);
		if (!append_line_to_content(content, line))
		{
			free(line);
			return (0);
		}
		free(line);
	}
<<<<<<< HEAD
	signal(SIGINT, handle_sig_int);
	if (g_global_sig == 130)
	{
		free(*content);
		*content = NULL;
		return (0);
	}
=======
>>>>>>> 9420e9937aaad08a295e33abf2bfb30333432d20
	return (1);
}
