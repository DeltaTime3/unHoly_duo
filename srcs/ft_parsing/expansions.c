#include "minishell.h"

static char	*expand_variables(char *value, t_shell *shell);
static void	expand_token_va_aux(char *value, t_exp_state *state);
static char	*append_qst(char *value, t_exp_state *state);
static char	*append_norm(char *value, t_exp_state *state);
static char	*append_bfr_dolar(char *value, int start, int i, char *result);
static char	*append_aft_last(char *value, int start, int i, char *result);

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

void	expand_tokens(t_token *token, t_shell *shell)
{
    char	*expanded;
    char	*old_value;
    int		i;

    if (token && token->value)
    {
        expanded = expand_token_value(token->value, shell);
        if (expanded)
        {
            old_value = token->value;
            token->value = expanded;
            free(old_value);
        }
    }    
    if (token && token->args)
    {
        i = 0;
        while (token->args[i])
        {
            expanded = expand_token_value(token->args[i], shell);
            if (expanded)
            {
                old_value = token->args[i];
                token->args[i] = expanded;
                free(old_value);
            }
            i++;
        }
    }
}

static char	*expand_variables(char *value, t_shell *shell)
{
    t_exp_state	state;

    state.i = 0;
    state.start = 0;
    state.shell = shell;
    state.result = ft_calloc(1, sizeof(char));
    if (!state.result)
        return (NULL);
    while (value[state.i])
    {
        expand_token_va_aux(value, &state);
    }
    if (state.i > state.start)
        state.result = append_aft_last(value, state.start,
                state.i, state.result);
    return (state.result);
}

char	*expand_token_value(char *value, t_shell *shell)
{
    char	*result;
    char	*content;
    int		len;

    if (!value)
        return (NULL);
    len = ft_strlen(value);
    if (len >= 2 && value[0] == '\'' && value[len - 1] == '\'')
    {
        result = ft_substr(value, 1, len - 2);
    }
    else if (len >= 2 && value[0] == '"' && value[len - 1] == '"')
    {
        content = ft_substr(value, 1, len - 2);
        if (!content)
            return (NULL);
        result = expand_variables(content, shell);
        free(content);
    }
    else
    {
        result = expand_variables(value, shell);
    }
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

static char	*append_norm(char *value, t_exp_state *state)
{
    char	*temp;
    char	*join;
    char	*val;

    temp = ft_substr(value, state->start, state->i - state->start);
    val = get_env_value(state->shell->head, temp);
    if (!val)
        val = "";
    join = ft_strjoin(state->result, val);
    free(state->result);
    free(temp);
    state->start = state->i;
    return (join);
}

static void	expand_token_va_aux(char *value, t_exp_state *state)
{
    if (value[state->i] == '$' && value[state->i + 1] == '?')
    {
        state->result = append_qst(value, state);
    }
    else if (value[state->i] == '$' && (ft_isalnum(value[state->i + 1])
        || value[state->i + 1] == '_'))
    {
        if (state->i > state->start)
            state->result = append_bfr_dolar(value, state->start,
                    state->i, state->result);
        (state->i)++;
        state->start = state->i;
        while (value[state->i] && (ft_isalnum(value[state->i])
            || value[state->i] == '_'))
            (state->i)++;
        state->result = append_norm(value, state);
    }
    else
    {
        (state->i)++;
    }
}

static char	*append_qst(char *value, t_exp_state *state)
{
    char	*code;
    char	*join;

    if (state->i > state->start)
        state->result = append_bfr_dolar(value, state->start,
                state->i, state->result);
    state->i += 2;
    code = ft_itoa(state->shell->exit_code);
    join = ft_strjoin(state->result, code);
    free(state->result);
    free(code);
    state->start = state->i;
    return (join);
}
