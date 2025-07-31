/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils4.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppaula-d <ppaula-d@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 13:59:12 by ppaula-d          #+#    #+#             */
/*   Updated: 2025/07/31 23:26:08 by ppaula-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	append_char(const char *input, int i, char **result)
{
	char	buf[2];
	char	*temp;

	buf[0] = input[i];
	buf[1] = '\0';
	temp = ft_strjoin(*result, buf);
	free(*result);
	*result = temp;
	return (i + 1);
}

char	*expand_command_arg(const char *input, t_shell *shell)
{
	char	*result;
	int		i;

	i = 0;
	result = ft_strdup("");
	while (input[i])
	{
		if (input[i] == '$' && input[i + 1] == '(')
			i = handle_command_substitution(input, i, &result);
		else if (input[i] == '$' && input[i + 1] != '\0' && input[i + 1] != ' ')
			i = handle_env_var(input, i, &result, shell);
		else
			i = append_char(input, i, &result);
	}
	return (result);
}

char	*fallback(char *cmd)
{
	char	cwd[_PC_PATH_MAX];
	char	*full_path;

	full_path = NULL;
	if (!cmd || !*cmd)
		return (NULL);
	if (getcwd(cwd, sizeof(cwd)) == NULL)
		return (NULL);
	full_path = build_cmb_path(cwd, cmd);
	if (full_path && access(full_path, X_OK) == 0)
		return (full_path);
	free(full_path);
	return (NULL);
}
