/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppaula-d <ppaula-d@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 13:59:12 by ppaula-d          #+#    #+#             */
/*   Updated: 2025/07/27 14:08:26 by ppaula-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_tokens(t_token *tokens)
{
	t_token	*current;
	t_token	*next;

	current = tokens;
	while (current)
	{
		next = current->next;
		if (current->value)
			free(current->value);
		if (current->token)
			free(current->token);
		if (current->command)
			free(current->command);
		if (current->args)
			free_args(current->args);
		if (current->content)
			free(current->content);
		free(current);
		current = next;
	}
}

void	free_args(char **args)
{
	int	i;

	i = 0;
	if (!args)
		return ;
	while (args[i])
		free(args[i++]);
	free(args);
}

int	get_quoted_content(const char *input, int *i, char quote,
	char **quoted_content)
{
	size_t	start_content;
	size_t	end_quote_pos;

	start_content = *i + 1;
	(*i)++;
	while (input[*i] && input[*i] != quote)
		(*i)++;
	if (input[*i] != quote)
		return (0);
	end_quote_pos = *i;
	*quoted_content = ft_substr(input, start_content,
			end_quote_pos - start_content);
	if (!*quoted_content)
		return (0);
	return (1);
}

size_t	get_unquoted_len(const char *input, int i)
{
	size_t	len;

	len = 0;
	while (input[i] && !ft_isspace(input[i]) && input[i] != '|'
		&& input[i] != '<' && input[i] != '>' && input[i] != '\''
		&& input[i] != '"')
	{
		len++;
		i++;
	}
	return (len);
}

int	get_unquoted_suffix(const char *input, int *i, char **unquoted_suffix)
{
	size_t	start_unquoted;
	size_t	len;

	(*i)++;
	start_unquoted = *i;
	len = get_unquoted_len(input, *i);
	*i += len;
	*unquoted_suffix = ft_substr(input, start_unquoted, len);
	if (!*unquoted_suffix)
		return (0);
	return (1);
}
