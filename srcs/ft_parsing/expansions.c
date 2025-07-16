#include "minishell.h"

static char	*expand_variables(char *value, t_shell *shell);
static void	expand_token_va_aux(char *value, t_exp_state *state);
static char	*append_qst(char *value, t_exp_state *state);
static char	*append_norm(char *value, t_exp_state *state);
static char	*append_bfr_dolar(char *value, int start, int i, char *result);
static char	*append_aft_last(char *value, int start, int i, char *result);

char    *remove_all_quotes(const char *input)
{
    int len;
    char    *result;
    int     i;
    int     j;
    char    in_quote;

    if (!input)
        return (NULL);
    len = ft_strlen(input);
    result = ft_calloc(len + 1, sizeof(char));
    if (!result)
        return (NULL);
    i = 0;
    j = 0;
    in_quote = 0;
    while (input[i])
    {
        if (in_quote == 0 && (input[i] == '\'' || input[i] == '"'))
            in_quote = input[i];
        else if (in_quote != 0 && input[i] == in_quote)
            in_quote = 0;
        else
            result[j++] = input[i];
        i++;
    }
    result[j] = '\0';
    return (result);
}

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

// In expansions.c
void expand_tokens(t_token *token, t_shell *shell)
{
    t_token *current = token;
    char *expanded;
    
    while (current)
    {
        if (current->value && current->value[0] != '\0')
        {
            expanded = expand_token_value(current->value, shell);
            free(current->value);
            current->value = expanded;
        }
        current = current->next;
    }
    
    // After expanding all tokens, handle empty command tokens
    adjust_command_after_expansion(token);
}

// Add this new function to handle empty command tokens
void adjust_command_after_expansion(t_token *tokens)
{
    t_token *current = tokens;
    t_token *next_cmd = NULL;
    
    // If the first token is a command and is empty after expansion
    if (current && current->type == COMMAND && 
        (!current->value || current->value[0] == '\0'))
    {
        // Look for the next non-empty token to promote to command
        next_cmd = current->next;
        while (next_cmd && (!next_cmd->value || next_cmd->value[0] == '\0'))
            next_cmd = next_cmd->next;
        
        // If we found a non-empty token and it's not a pipe or redirection
        if (next_cmd && next_cmd->type != PIPE && next_cmd->type != REDIRECT)
        {
            next_cmd->type = COMMAND;
            
            // Also update the args array if it exists
            if (current->args)
            {
                free_args(current->args);
                current->args = NULL;
            }
        }
    }
    
    // Handle commands after pipes
    while (current)
    {
        if (current->type == PIPE && current->next)
        {
            // Skip any empty tokens after a pipe
            next_cmd = current->next;
            while (next_cmd && (!next_cmd->value || next_cmd->value[0] == '\0'))
                next_cmd = next_cmd->next;
            
            // If we found a non-empty token, make it a command
            if (next_cmd && next_cmd->type != PIPE && next_cmd->type != REDIRECT)
                next_cmd->type = COMMAND;
        }
        current = current->next;
    }
}

static char	*expand_variables(char *value, t_shell *shell)
{
    t_exp_state	state;

    state.i = 0;
    state.start = 0;
    state.shell = shell;
    state.result = ft_calloc(1, sizeof(char));
    state.in_quote = 0;
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
    char	*expanded_vars;
    char	*final_result;

    if (!value)
        return (NULL);
    if (value[0] == '~' && (value[1] == '\0' || value[1] == '/'))
    {
        char *home = get_env_value(shell->head, "HOME");
        if (home)
        {
            char *expanded = ft_strjoin(home, value + 1);
            return expanded;
        }
        return ft_strdup(value);
    }
    expanded_vars = expand_variables(value, shell);
    if (!expanded_vars)
        return (ft_strdup(""));
    final_result = remove_all_quotes(expanded_vars);
    free(expanded_vars);
    return (final_result);
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
    if (value[state->i] == '\'' || value[state->i] == '"')
    {
        if (state->in_quote == 0)
            state->in_quote = value[state->i];
        else if (state->in_quote == value[state->i])
            state->in_quote = 0;
    }
    // check for variable expansion only if not inside single quotes
    if (value[state->i] == '$' && state->in_quote != '\'')
    {
        if (value[state->i + 1] == '?')
        {
            state->result = append_qst(value, state);
        }
        else if (ft_isalnum(value[state->i + 1]) || value[state->i + 1] == '_')
        {
            if (state->i > state->start)
                state->result = append_bfr_dolar(value, state->start, state->i, state->result);
            (state->i)++;
            state->start = state->i;
            while (value[state->i] && (ft_isalnum(value[state->i]) || value[state->i] == '_'))
                (state->i)++;
            state->result = append_norm(value, state);
        }
        else
        {
            (state->i)++;
        }
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
