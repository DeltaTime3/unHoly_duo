/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansions_helpers3.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppaula-d <ppaula-d@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 13:59:12 by ppaula-d          #+#    #+#             */
/*   Updated: 2025/07/26 18:07:36 by ppaula-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*expand_token_value(char *value, t_shell *shell)
{
	char	*home;

	if (!value)
		return (NULL);
	if (value[0] == '~' && (value[1] == '\0' || value[1] == '/'))
	{
		home = get_env_value(shell->head, "HOME");
		if (home)
			return (ft_strjoin(home, value + 1));
		return (ft_strdup(value));
	}
	if (value[0] == '\'' && value[ft_strlen(value) - 1] == '\'')
		return (expand_single_quoted_value(value, shell));
	return (process_quotes(value, shell));
}

char	*process_quoted_segment(const char *input, int *i,
			char quote_type, t_shell *shell)
{
	int		start;
	char	*segment;
	char	*processed;

	(*i)++;
	start = *i;
	while (input[*i] && input[*i] != quote_type)
		(*i)++;
	if (input[*i] == quote_type)
	{
		segment = ft_substr(input, start, *i - start);
		if (quote_type == '\'')
			processed = segment;
		else
		{
			processed = exp_vars_dbl_quotes(segment, shell);
			free(segment);
		}
		(*i)++;
		return (processed);
	}
	return (NULL);
}

char	*process_unquoted_segment(const char *input, int *i, t_shell *shell)
{
	int		start;
	char	*segment;
	char	*expanded;

	start = *i;
	while (input[*i] && input[*i] != '\'' && input[*i] != '"')
		(*i)++;
	segment = ft_substr(input, start, *i - start);
	expanded = expand_variables(segment, shell);
	free(segment);
	return (expanded);
}

char	*join_and_free(char *s1, char *s2)
{
	char	*joined;

	joined = ft_strjoin(s1, s2);
	free(s1);
	free(s2);
	return (joined);
}

char	*handle_quoted(const char *input, int *i, t_shell *shell, char **result)
{
	char	quote_type;
	char	*processed;

	quote_type = input[*i];
	processed = process_quoted_segment(input, i, quote_type, shell);
	if (!processed)
	{
		free(*result);
		*result = ft_strdup(input);
		return (NULL);
	}
	*result = join_and_free(*result, processed);
	return (*result);
}
