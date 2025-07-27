/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansions_helpers.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppaula-d <ppaula-d@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 13:59:12 by ppaula-d          #+#    #+#             */
/*   Updated: 2025/07/26 18:06:30 by ppaula-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*remove_quotes(const char *input)
{
	size_t	len;
	char	*result;

	len = ft_strlen(input);
	if ((input[0] == '\'' && input[len - 1] == '\'')
		|| (input[0] == '"' && input[len - 1] == '"'))
	{
		result = ft_substr(input, 1, len - 2);
		if (!result)
			return (NULL);
	}
	else
	{
		result = ft_strdup(input);
		if (!result)
			return (NULL);
	}
	return (result);
}

void	expand_tokens(t_token *token, t_shell *shell)
{
	t_token	*current;
	char	*original_value;
	char	*expanded_value;

	current = token;
	while (current)
	{
		if (current->value)
		{
			original_value = current->value;
			expanded_value = expand_variables(original_value, shell);
			if (expanded_value[0] == '\0' && original_value[0] != '\0'
				&& ft_strchr(original_value, '$'))
				current->was_expanded = 1;
			current->value = expanded_value;
			free(original_value);
		}
		current = current->next;
	}
	adjust_command_after_expansion(token);
}

void	promote_next_command_if_expanded_empty(t_token *current)
{
	t_token	*next_cmd;

	if (current && current->type == COMMAND && current->was_expanded
		&& (!current->value || current->value[0] == '\0'))
	{
		next_cmd = current->next;
		while (next_cmd && (!next_cmd->value || next_cmd->value[0] == '\0'))
			next_cmd = next_cmd->next;
		if (next_cmd && next_cmd->type != PIPE && next_cmd->type != REDIRECT
			&& next_cmd->type != HERE_DOC)
		{
			next_cmd->type = COMMAND;
			current->type = OTHER;
		}
	}
}

void	adjust_command_after_expansion(t_token *tokens)
{
	t_token	*current;
	t_token	*next_cmd;

	current = tokens;
	promote_next_command_if_expanded_empty(current);
	while (current)
	{
		if (current->type == PIPE && current->next)
		{
			next_cmd = current->next;
			while (next_cmd && (!next_cmd->value || next_cmd->value[0] == '\0'))
				next_cmd = next_cmd->next;
			if (next_cmd && next_cmd->type != PIPE)
				next_cmd->type = COMMAND;
		}
		current = current->next;
	}
}

char	*expand_single_quoted_value(char *value, t_shell *shell)
{
	char	*inner;
	char	*expanded;
	size_t	len;
	char	*result;

	inner = ft_substr(value, 1, ft_strlen(value) - 2);
	expanded = expand_variables(inner, shell);
	len = ft_strlen(expanded);
	result = ft_calloc(len + 3, sizeof(char));
	if (result)
	{
		result[0] = '\'';
		ft_strcpy(result + 1, expanded);
		result[len + 1] = '\'';
		result[len + 2] = '\0';
	}
	free(inner);
	free(expanded);
	return (result);
}
