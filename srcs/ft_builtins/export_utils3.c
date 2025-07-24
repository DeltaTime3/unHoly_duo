#include "../../include/minishell.h"

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

static int	append_exp_var(const char *input, int i, char **result,
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

char	*expand_command_arg(const char *input, t_shell *shell)
{
	char	*result;
	char	*temp;
	int		i;

	i = 0;
	result = ft_strdup("");
	while (input[i])
	{
		if (input[i] == '$' && input[i + 1] != '\0' && input[i + 1] != ' ')
		{
			i = append_exp_var(input, i, &result, shell);
		}
		else
		{
			temp = ft_strjoin(result, &input[i]);
			free(result);
			result = temp;
			i++;
		}
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
