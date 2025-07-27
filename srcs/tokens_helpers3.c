/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens_helpers3.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppaula-d <ppaula-d@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 13:59:12 by ppaula-d          #+#    #+#             */
/*   Updated: 2025/07/27 13:23:44 by ppaula-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	pipe_handling(const char *input, int *i, t_token **tokens)
{
	size_t	start;
	char	*value;

	while (input[*i] && ft_isspace(input[*i]))
		(*i)++;
	if (!input[*i] || (input[*i] == '|' && (input[*i + 1] == '\0')))
	{
		ft_printf_fd(2, OPEN_PIPE);
		return (1);
	}
	start = *i;
	(*i)++;
	value = ft_substr(input, start, *i - start);
	if (!value)
		return (1);
	add_token(tokens, create_token(PIPE, value));
	free(value);
	while (input[*i] && ft_isspace(input[*i]))
		(*i)++;
	return (0);
}

int	dup2_and_close(int fd, int target_fd, int *other_fd)
{
	if (dup2(fd, target_fd) == -1)
	{
		if (target_fd == STDIN_FILENO)
			perror("dup2 for stdin");
		else
			perror("dup2 for stdout");
		close(fd);
		if (other_fd && *other_fd != -1)
			close(*other_fd);
		return (-1);
	}
	close(fd);
	return (0);
}

int	handle_pipe_error(char *heredoc_content, int fd_out, t_shell *shell)
{
	free(heredoc_content);
	perror("pipe");
	shell->exit_code = 1;
	if (fd_out != -1)
		close(fd_out);
	return (-1);
}

int	handle_heredoc_redirect(t_token *temp, int *fd_in, int fd_out,
	t_shell *shell)
{
	char	*heredoc_content;
	int		heredoc_fd[2];

	if (*fd_in != -1)
		close(*fd_in);
	heredoc_content = read_heredoc_input(temp->next->value,
			temp->next->expand_heredoc, shell);
	if (!heredoc_content)
	{
		shell->exit_code = 130;
		if (fd_out != -1)
			close(fd_out);
		return (-1);
	}
	if (pipe(heredoc_fd) == -1)
		return (handle_pipe_error(heredoc_content, fd_out, shell));
	write(heredoc_fd[1], heredoc_content, ft_strlen(heredoc_content));
	close(heredoc_fd[1]);
	free(heredoc_content);
	*fd_in = heredoc_fd[0];
	return (0);
}

int	open_redirect_file(const char *filename, int *fd, int flags,
	t_shell *shell)
{
	if (*fd != -1)
		close(*fd);
	*fd = open(filename, flags, 0644);
	if (*fd == -1)
	{
		perror(filename);
		shell->exit_code = 1;
		return (-1);
	}
	return (0);
}
