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

int	is_dollar_expansion(const char *value)
{
	const char	*end;
	int			i;

	if (value[1] == '{')
	{
		end = ft_strchr(value + 2, '}');
		if (end && end[1] == '\0')
			return (1);
	}
	else if (ft_isalpha(value[1]) || value[1] == '_')
	{
		i = 2;
		while (value[i] && (ft_isalnum(value[i]) || value[i] == '_'))
			i++;
		if (value[i] == '\0')
			return (1);
	}
	return (0);
}

int	write_heredoc_to_pipe(const char *content)
{
	int	pipe_fd[2];

	if (pipe(pipe_fd) == -1)
		return (-1);
	if (content)
		write(pipe_fd[1], content, ft_strlen(content));
	close(pipe_fd[1]);
	return (pipe_fd[0]);
}

static ssize_t	read_line_to_buffer(char *buf, size_t bufsize)
{
	ssize_t	n;
	size_t	i;

	i = 0;
	while (1)
	{
		n = read(STDIN_FILENO, &buf[i], 1);
		if (n <= 0)
			break ;
		if (buf[i] == '\n')
		{
			buf[i] = '\0';
			break ;
		}
		i++;
		if (i == bufsize - 1)
			break ;
	}
	if (n <= 0 && i == 0)
		return (-1);
	return ((ssize_t)i);
}

char	*read_line(void)
{
	char	buf[BUFFER_SIZE];
	char	*line;
	ssize_t	len;
	size_t	j;

	len = read_line_to_buffer(buf, BUFFER_SIZE);
	if (len == -1)
		return (NULL);
	line = malloc(len + 1);
	if (!line)
		return (NULL);
	j = 0;
	while ((ssize_t)j < len)
	{
		line[j] = buf[j];
		j++;
	}
	line[len] = '\0';
	return (line);
}

char	*prompt_and_read_line(void)
{
	write(STDERR_FILENO, "> ", 2);
	return (read_line());
}
