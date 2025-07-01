#include "minishell.h"

char *expand_env_var(const char *input, t_shell *shell)
{
    char    *var_name;
    char    *value;

    if (!input || input[0] != '$' || input[1] == '\0')
        return ft_strdup(input);
    // extract variable name
    var_name = ft_substr(input, 1, ft_strlen(input) - 1);
    // retrieve value using built function
    value = get_env_value(shell->head, var_name);
    free(var_name);
    if (value)
        return (ft_strdup(value));
    else
        return (ft_strdup(""));
}

char *expand_exit_status(const char *input, t_shell *shell)
{
    // convert exit code to string
    if (ft_strcmp(input, "$?") == 0)
        return (ft_itoa(shell->exit_code));
    return (ft_strdup(input));
}

char    *remove_quotes(const char *input)
{
    size_t  len;
    char    *result;

    len = ft_strlen(input);
    if ((input[0] == '\'' && input[len - 1] == '\'') ||
            (input[0] == '"' && input[len - 1] == '"'))
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

void expand_tokens(t_token *token, t_shell *shell)
{
    char *expanded;
    char *old_value;
    
    if (token && token->value)
    {
        expanded = expand_token_value(token->value, shell);
        if (expanded)
        {
            old_value = token->value;
            token->value = remove_quotes(expanded);
            free(old_value);  
            free(expanded);
        }
    }
    
    if (token && token->args)
    {
        int i = 0;
        while (token->args[i])
        {
            expanded = expand_token_value(token->args[i], shell);
            if (expanded)
            {
                old_value = token->args[i];
                token->args[i] = remove_quotes(expanded);
                free(old_value);
                free(expanded);
            }
            i++;
        }
    }
}

char	*expand_token_value(char *value, t_shell *shell)
{
	char	*result;
	int		start;
	int		i;
	int		len;

	i = 0;
	start = 0;
	len = ft_strlen(value);
	result = ft_calloc(1, sizeof(char));
	if(!result)
		return(NULL);
	while (i < len)
	{
		expand_token_va_aux(value, &start, &i, &result, shell);
	}
	if (i > start)
		result = append_aft_last(value, start, i, result);
	return (result);
}

char	*append_bfr_dolar(char *value, int start, int i, char *result)
{
	char	*temp;
	char	*join;

	temp = ft_substr(value, start, i - start);
	join = ft_strjoin(result, temp);
	if (!join)
	{
		free(join);
		return(NULL);
	}
	if (!temp)
		return(NULL);
	free(result);
	result = join;
	free(temp);
	return(result);
}

char	*append_aft_last(char *value, int start, int i, char *result)
{
	char	*temp;
	char	*join;

	temp = ft_substr(value, start, i - start);
	join = ft_strjoin(result, temp);
	if (!join)
	{
		free(join);
		return(NULL);
	}
	if (!temp)
		return(NULL);
	free(result);
	result = join;
	free(temp);
	return (result);
}

char	*append_norm(char *value, int *start, int i, char *result, t_shell *shell)
{
	char	*temp;
	char	*join;
	char	*val;
	
	temp = ft_substr(value, *start, i - *start);
	val = get_env_value(shell->head, temp);
	if (!val)
		val = "";
	join = ft_strjoin(result, val);
	if (!join)
	{
		free(join);
		return(NULL);
	}
	if (!temp)
		return(NULL);
	free(result);
	result = join;
	free(temp);
	*start = i;
	return(result);
}

void expand_token_va_aux(char *value, int *start, int *i, char **result, t_shell *shell)
{
	if (value[*i] == '$' && value[*i + 1] == '?')
	{
		*result = append_qst(value, start, i, *result, shell);
	}
	else if (value[*i] == '$' && value[*i + 1] != '\0')
	{
		if(*i > *start)
			*result = append_bfr_dolar(value, *start, *i, *result);
		(*i)++;
		*start = *i;
		while (value[*i] && (ft_isalnum(value[*i]) || value[*i] == '_'))
			(*i)++;
		*result = append_norm(value, start, *i, *result, shell);
	}
	else
			(*i)++;
}

char	*append_qst(char *value, int *start, int *i, char *result, t_shell *shell)
{
	char	*code;
	char	*join;

	if (value[*i] == '$' && value[*i + 1] == '?')
	{
		if (*i > *start)
			result = append_bfr_dolar(value, *start, *i, result);
		*i += 2;
		code = ft_itoa(shell->exit_code);
		join = ft_strjoin(result, code);
		free(result);
		result = join;
		free(code);
		*start = *i;
	}
	return (result);
}
