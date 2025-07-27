/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansions_helpers4.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppaula-d <ppaula-d@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 13:59:12 by ppaula-d          #+#    #+#             */
/*   Updated: 2025/07/26 18:19:41 by ppaula-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_env_var(const char *value, int i, char **result, t_shell *shell)
{
	int		start;
	int		j;
	char	*var_name;
	char	*var_value;
	char	*temp;

	start = i + 1;
	j = start;
	while (value[j] && (ft_isalnum(value[j]) || value[j] == '_'))
		j++;
	var_name = ft_substr(value, start, j - start);
	var_value = get_env_value(shell->head, var_name);
	if (!var_value)
		var_value = "";
	temp = ft_strjoin(*result, var_value);
	free(*result);
	free(var_name);
	*result = temp;
	return (j);
}

int	handle_dbl_quote_var(const char *value, int i, char **result,
		t_shell *shell)
{
	char	*exit_str;
	char	*temp;

	if (value[i + 1] == '?')
	{
		exit_str = ft_itoa(shell->exit_code);
		temp = ft_strjoin(*result, exit_str);
		free(*result);
		free(exit_str);
		*result = temp;
		return (i + 2);
	}
	else if (ft_isalnum(value[i + 1]) || value[i + 1] == '_')
		return (handle_env_var(value, i, result, shell));
	else
	{
		temp = ft_strjoin(*result, "$");
		free(*result);
		*result = temp;
		return (i + 1);
	}
}

char	*exp_vars_dbl_quotes(char *value, t_shell *shell)
{
	char	*result;
	int		i;
	char	*temp;
	char	char_str[2];

	result = ft_calloc(1, sizeof(char));
	i = 0;
	if (!result || !value)
		return (NULL);
	while (value[i])
	{
		if (value[i] == '$' && value[i + 1] && !ft_isspace(value[i + 1]))
			i = handle_dbl_quote_var(value, i, &result, shell);
		else
		{
			char_str[0] = value[i];
			char_str[1] = '\0';
			temp = ft_strjoin(result, char_str);
			free(result);
			result = temp;
			i++;
		}
	}
	return (result);
}

char	*process_quotes(char *input, t_shell *shell)
{
	char	*result;
	int		i;
	char	*expanded;

	i = 0;
	result = ft_calloc(1, sizeof(char));
	if (!result || !input)
		return (NULL);
	while (input[i])
	{
		if (input[i] == '\'' || input[i] == '"')
		{
			if (!handle_quoted(input, &i, shell, &result))
				return (result);
		}
		else
		{
			expanded = process_unquoted_segment(input, &i, shell);
			result = join_and_free(result, expanded);
		}
	}
	return (result);
}
