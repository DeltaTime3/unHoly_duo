#include "minishell.h"

char	*expand_variables(char *value, t_shell *shell);

static void	append_str(char **dest, const char *src);
static void	handle_expansion(const char *value, int *i, char **result,
				t_shell *shell);

static void	append_str(char **dest, const char *src)
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
    var_name = ft_substr(input, 1, ft_strlen(input) - 1);
    value = get_env_value(shell->head, var_name);
    free(var_name);
    if (value)
        return (ft_strdup(value));
    else
        return (ft_strdup(""));
}

char *expand_exit_status(const char *input, t_shell *shell)
{
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
	t_token	*current;
	char	*original_value;
	char	*expanded_value;

	current = token;
	while (current)
	{
		if (current->value)
		{
			original_value = current->value;
			expanded_value = expand_variables(original_value, shell);
			if (expanded_value[0] == '\0' && original_value[0] != '\0'
				&& ft_strchr(original_value, '$'))
				current->was_expanded = 1;
			current->value = expanded_value;
			free(original_value);
		}
		current = current->next;
	}
	adjust_command_after_expansion(token);
}


void	adjust_command_after_expansion(t_token *tokens)
{
	t_token	*current;
	t_token	*next_cmd;

	current = tokens;
	if (current && current->type == COMMAND && current->was_expanded
		&& (!current->value || current->value[0] == '\0'))
	{
		next_cmd = current->next;
		while (next_cmd && (!next_cmd->value || next_cmd->value[0] == '\0'))
			next_cmd = next_cmd->next;
		if (next_cmd && next_cmd->type != PIPE && next_cmd->type != REDIRECT
			&& next_cmd->type != HERE_DOC)
		{
			next_cmd->type = COMMAND;
			current->type = OTHER;
		}
	}
	while (current)
	{
		if (current->type == PIPE && current->next)
		{
			next_cmd = current->next;
			while (next_cmd && (!next_cmd->value || next_cmd->value[0] == '\0'))
				next_cmd = next_cmd->next;
			if (next_cmd && next_cmd->type != PIPE)
				next_cmd->type = COMMAND;
		}
		current = current->next;
	}
}

char *expand_token_value(char *value, t_shell *shell)
{
    if (!value)
        return (NULL);
    if (value[0] == '~' && (value[1] == '\0' || value[1] == '/'))
    {
        char *home = get_env_value(shell->head, "HOME");
        if (home)
            return (ft_strjoin(home, value + 1));
        return (ft_strdup(value));
    }
    if (value[0] == '\'' && value[ft_strlen(value) - 1] == '\'')
    {
        char *inner = ft_substr(value, 1, ft_strlen(value) - 2);
        char *expanded = expand_variables(inner, shell);
        size_t len = ft_strlen(expanded);
        char *result = ft_calloc(len + 3, sizeof(char));
        if (result)
        {
            result[0] = '\'';
            ft_strcpy(result + 1, expanded);
            result[len + 1] = '\'';
            result[len + 2] = '\0';
        }
        free(inner);
        free(expanded);
        return result;
    }
    return process_quotes(value, shell);
}

char *process_quotes(char *input, t_shell *shell)
{
    char *result = ft_calloc(1, sizeof(char));
    int i = 0;
    
    if (!result || !input)
        return (NULL);
    while (input[i])
    {
        if (input[i] == '\'' || input[i] == '"')
        {
            char quote_type = input[i];
            i++; // Skip the opening quote
            int start = i;
            while (input[i] && input[i] != quote_type)
                i++;
            
            if (input[i] == quote_type) // Found closing quote
            {
                char *segment = ft_substr(input, start, i - start);
                char *processed;
                
                if (quote_type == '\'')
                {
                    processed = segment;
                }
                else
                {
                    processed = expand_variables_in_double_quotes(segment, shell);
                    free(segment);
                }
                
                char *new_result = ft_strjoin(result, processed);
                free(result);
                free(processed);
                result = new_result;
                
                i++;
            }
            else
            {
                free(result);
                return (ft_strdup(input));
            }
        }
        else
        {
            int start = i;
            while (input[i] && input[i] != '\'' && input[i] != '"')
                i++;
            char *segment = ft_substr(input, start, i - start);
            char *expanded = expand_variables(segment, shell);
            char *new_result = ft_strjoin(result, expanded);
            free(result);
            free(segment);
            free(expanded);
            result = new_result;
        }
    }
    return result;
}

static void	handle_expansion(const char *value, int *i, char **result,
		t_shell *shell)
{
	int		start;
	char	*var_name;
	char	*var_value;

	(*i)++;
	if (value[*i] == '?')
	{
		var_value = ft_itoa(shell->exit_code);
		append_str(result, var_value);
		free(var_value);
		(*i)++;
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
}


char	*expand_variables(char *value, t_shell *shell)
{
	char	*result;
	int		i;

	i = 0;
	result = ft_calloc(1, sizeof(char));
	if (!result || !value)
		return (result);
	while (value[i])
	{
		if (value[i] == '\'')
		{
			i++;
			int start = i;
			while (value[i] && value[i] != '\'')
				i++;
			char *literal = ft_substr(value, start, i - start);
			append_str(&result, literal);
			free(literal);
			if (value[i] == '\'')
				i++;
		}
		else if (value[i] == '"')
		{
			i++;
			while (value[i] && value[i] != '"')
			{
				if (value[i] == '$')
					handle_expansion(value, &i, &result, shell);
				else
				{
					char to_add[2] = {value[i++], '\0'};
					append_str(&result, to_add);
				}
			}
			if (value[i] == '"')
				i++;
		}
		else if (value[i] == '$')
			handle_expansion(value, &i, &result, shell);
		else
		{
			char to_add[2] = {value[i++], '\0'};
			append_str(&result, to_add);
		}
	}
	return (result);
}

char *expand_variables_in_double_quotes(char *value, t_shell *shell)
{
    char *result = ft_calloc(1, sizeof(char));
    int i = 0;
    
    if (!result || !value)
        return (NULL);
    
    while (value[i])
    {
        if (value[i] == '$' && value[i + 1] && !ft_isspace(value[i + 1]))
        {
            if (value[i + 1] == '?')
            {
                // Handle $?
                char *exit_str = ft_itoa(shell->exit_code);
                char *temp = ft_strjoin(result, exit_str);
                free(result);
                free(exit_str);
                result = temp;
                i += 2;  // Skip $?
            }
            else if (ft_isalnum(value[i + 1]) || value[i + 1] == '_')
            {
                // Handle $VAR
                int start = i + 1;
                int j = start;
                while (value[j] && (ft_isalnum(value[j]) || value[j] == '_'))
                    j++;
                
                char *var_name = ft_substr(value, start, j - start);
                char *var_value = get_env_value(shell->head, var_name);
                if (!var_value)
                    var_value = "";
                
                char *temp = ft_strjoin(result, var_value);
                free(result);
                free(var_name);
                result = temp;
                
                i = j;  // Move to end of variable
            }
            else
            {
                // Just a literal $
                char *temp = ft_strjoin(result, "$");
                free(result);
                result = temp;
                i++;
            }
        }
        else
        {
            // Regular character (including quotes inside double quotes)
            char char_str[2] = {value[i], '\0'};
            char *temp = ft_strjoin(result, char_str);
            free(result);
            result = temp;
            i++;
        }
    }
    
    return result;
}
