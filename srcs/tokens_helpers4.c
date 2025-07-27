/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens_helpers4.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppaula-d <ppaula-d@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 13:59:12 by ppaula-d          #+#    #+#             */
/*   Updated: 2025/07/27 13:22:32 by ppaula-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_redirect_file(const char *filename, int *fd, int flags,
		t_shell *shell)
{
	if (open_redirect_file(filename, fd, flags, shell) == -1)
		return (-1);
	return (0);
}

int	handle_redirection(t_token *temp, int *fd, int flags, t_shell *shell)
{
	if (handle_redirect_file(temp->next->value, fd, flags, shell) == -1)
		return (-1);
	return (0);
}

int	close_and_return(int *fd, int ret)
{
	if (fd && *fd != -1)
		close(*fd);
	return (ret);
}

int	process_redirect_token(t_token *temp, int *fd_in, int *fd_out,
	t_shell *shell)
{
	int	flags;

	if (ft_strcmp(temp->value, "<") == 0)
	{
		if (handle_redirection(temp, fd_in, O_RDONLY, shell) == -1)
			return (close_and_return(fd_out, -1));
	}
	else if (ft_strcmp(temp->value, ">") == 0)
	{
		flags = O_WRONLY | O_CREAT | O_TRUNC;
		if (handle_redirection(temp, fd_out, flags, shell) == -1)
			return (close_and_return(fd_in, -1));
	}
	else if (ft_strcmp(temp->value, ">>") == 0)
	{
		flags = O_WRONLY | O_CREAT | O_APPEND;
		if (handle_redirection(temp, fd_out, flags, shell) == -1)
			return (close_and_return(fd_in, -1));
	}
	return (0);
}

int	process_redirects_loop(t_token *tokens, int *fd_in, int *fd_out,
	t_shell *shell)
{
	t_token	*temp;

	temp = tokens;
	while (temp && temp->type != PIPE)
	{
		if (temp->type == REDIRECT && temp->next && temp->next->type == FILES)
		{
			if (process_redirect_token(temp, fd_in, fd_out, shell) == -1)
				return (-1);
			temp = temp->next;
		}
		else if (temp->type == HERE_DOC && temp->next
			&& temp->next->type == DELIMETER)
		{
			if (handle_heredoc_redirect(temp, fd_in, *fd_out, shell) == -1)
				return (-1);
			temp = temp->next;
		}
		if (temp)
			temp = temp->next;
	}
	return (0);
}
