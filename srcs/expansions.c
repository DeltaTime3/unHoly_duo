/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansions.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppaula-d <ppaula-d@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 13:59:12 by ppaula-d          #+#    #+#             */
/*   Updated: 2025/07/26 18:06:07 by ppaula-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	append_str(char **dest, const char *src)
{
	char	*temp;

	if (!src)
		return ;
	if (!*dest)
	{
		*dest = ft_strdup(src);
		return ;
	}
	temp = ft_strjoin(*dest, src);
	free(*dest);
	*dest = temp;
}

int	skip_quotes(const char *input, int i, char *result, int *j)
{
	char	in_quote;

	in_quote = input[i];
	i++;
	while (input[i] && input[i] != in_quote)
		result[(*j)++] = input[i++];
	if (input[i] == in_quote)
		i++;
	return (i);
}

char	*remove_all_quotes(const char *input)
{
	int		len;
	char	*result;
	int		i;
	int		j;

	if (!input)
		return (NULL);
	len = ft_strlen(input);
	result = ft_calloc(len + 1, sizeof(char));
	if (!result)
		return (NULL);
	i = 0;
	j = 0;
	while (input[i])
	{
		if (input[i] == '\'' || input[i] == '"')
			i = skip_quotes(input, i, result, &j);
		else
			result[j++] = input[i++];
	}
	result[j] = '\0';
	return (result);
}

char	*expand_env_var(const char *input, t_shell *shell)
{
	char	*var_name;
	char	*value;

	if (!input || input[0] != '$' || input[1] == '\0')
		return (ft_strdup(input));
	var_name = ft_substr(input, 1, ft_strlen(input) - 1);
	value = get_env_value(shell->head, var_name);
	free(var_name);
	if (value)
		return (ft_strdup(value));
	else
		return (ft_strdup(""));
}

char	*expand_exit_status(const char *input, t_shell *shell)
{
	if (ft_strcmp(input, "$?") == 0)
		return (ft_itoa(shell->exit_code));
	return (ft_strdup(input));
}
