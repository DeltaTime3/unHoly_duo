/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils3.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppaula-d <ppaula-d@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 13:59:12 by ppaula-d          #+#    #+#             */
/*   Updated: 2025/07/31 23:26:06 by ppaula-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*handle_dolar(char *value, t_shell *shell)
{
	char	*home;
	char	*temp;

	if (ft_strchr(value, '$'))
		return (expand_command_arg(value, shell));
	if (value[0] == '~' && (value[1] == '\0' || value[1] == '/'))
	{
		home = get_env_value(shell->head, "HOME");
		if (home)
		{
			temp = ft_strjoin(home, value + 1);
			return (temp);
		}
		else
			return (ft_strdup(value));
	}
	return (NULL);
}

char	*expand_token_arg_to_value(char *value, t_shell *shell)
{
	char	*result;

	result = handle_dolar(value, shell);
	if (result)
		return (result);
	if (ft_strcmp(value, "$?") == 0)
		result = expand_exit_status(value, shell);
	else if (value[0] == '$' && value[1] != '\0')
		result = expand_env_var(value, shell);
	else
		result = ft_strdup(value);
	return (result);
}

int	append_exp_var(const char *input, int i, char **result,
	t_shell *shell)
{
	char	*var_value;
	char	*temp;

	var_value = expand_env_var(&input[i], shell);
	temp = ft_strjoin(*result, var_value);
	free(*result);
	free(var_value);
	*result = temp;
	while (input[i] && !ft_isspace(input[i]) && input[i] != '=')
		i++;
	return (i);
}

int	handle_command_substitution(const char *input, int i, char **result)
{
	int		start;
	int		paren_level;
	int		len;
	char	*temp;
	char	*substr;

	start = i;
	paren_level = 1;
	i += 2;
	while (input[i] && paren_level > 0)
	{
		if (input[i] == '(')
			paren_level++;
		else if (input[i] == ')')
			paren_level--;
		i++;
	}
	len = i - start;
	substr = ft_substr(input, start, len);
	temp = ft_strjoin(*result, substr);
	free(*result);
	free(substr);
	*result = temp;
	return (i);
}

int	handle_env_var2(const char *input, int i, char **result, t_shell *shell)
{
	char	*var_value;
	char	*temp;

	var_value = expand_env_var(&input[i], shell);
	temp = ft_strjoin(*result, var_value);
	free(*result);
	free(var_value);
	*result = temp;
	while (input[i] && !ft_isspace(input[i]) && input[i] != '=')
		i++;
	return (i);
}
