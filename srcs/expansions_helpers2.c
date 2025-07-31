/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansions_helpers2.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppaula-d <ppaula-d@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 13:59:12 by ppaula-d          #+#    #+#             */
/*   Updated: 2025/07/31 18:08:33 by ppaula-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_exit_code_expansion(char **result, t_shell *shell, int *i)
{
	char	*var_value;

	var_value = ft_itoa(shell->exit_code);
	append_str(result, var_value);
	free(var_value);
	(*i)++;
}

void	handle_expansion(const char *value, int *i, char **result,
		t_shell *shell)
{
	int		start;
	char	*var_name;
	char	*var_value;

	(*i)++;
	if (value[*i] == '?')
	{
		handle_exit_code_expansion(result, shell, i);
		return ;
	}
	if (!ft_isalpha(value[*i]) && value[*i] != '_')
	{
		append_str(result, "$");
		return ;
	}
	start = *i;
	while (value[*i] && (ft_isalnum(value[*i]) || value[*i] == '_'))
		(*i)++;
	var_name = ft_substr(value, start, *i - start);
	var_value = get_env_value(shell->head, var_name);
	free(var_name);
	if (var_value)
		append_str(result, var_value);
	else
		append_str(result, "");
}

void	handle_single_quotes(const char *value, int *i, char **result)
{
	int		start;
	char	*literal;

	(*i)++;
	start = *i;
	while (value[*i] && value[*i] != '\'')
		(*i)++;
	literal = ft_substr(value, start, *i - start);
	append_str(result, literal);
	free(literal);
	if (value[*i] == '\'')
		(*i)++;
}

void	handle_double_quotes(const char *value, int *i, char **result,
			t_shell *shell)
{
	char	to_add[2];

	(*i)++;
	while (value[*i] && value[*i] != '"')
	{
		if (value[*i] == '$')
			handle_expansion(value, i, result, shell);
		else
		{
			to_add[0] = value[(*i)];
			to_add[1] = '\0';
			append_str(result, to_add);
			(*i)++;
		}
	}
	if (value[*i] == '"')
		(*i)++;
}

char	*expand_variables(char *value, t_shell *shell)
{
	char	*result;
	int		i;
	char	to_add[2];

	i = 0;
	result = ft_calloc(1, sizeof(char));
	if (!result || !value)
		return (result);
	while (value[i])
	{
		if (value[i] == '\'')
			handle_single_quotes(value, &i, &result);
		else if (value[i] == '"')
			handle_double_quotes(value, &i, &result, shell);
		else if (value[i] == '$')
			handle_expansion(value, &i, &result, shell);
		else
		{
			to_add[0] = value[i++];
			to_add[1] = '\0';
			append_str(&result, to_add);
		}
	}
	return (result);
}
