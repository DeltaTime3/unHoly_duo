#include "minishell.h"

char	*expand_variables(char *value, t_shell *shell);
//static void	expand_token_va_aux(char *value, t_exp_state *state);
//static char	*append_qst(char *value, t_exp_state *state);
//static char	*append_norm(char *value, t_exp_state *state);
//static char	*append_bfr_dolar(char *value, int start, int i, char *result);
//static char	*append_aft_last(char *value, int start, int i, char *result);

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
    char *original;
    
    while (current)
    {
        if (current->value && current->value[0] != '\0')
        {
            original = current->value;
            
            // Check if the token contains a variable expansion
            if (ft_strchr(original, '$') != NULL)
            {
                expanded = expand_token_value(original, shell);
                
                // If the expansion resulted in an empty string, mark it as expanded
                if (expanded && expanded[0] == '\0')
                    current->was_expanded = 1;
                
                free(current->value);
                current->value = expanded;
            }
            else
            {
                // For non-variable tokens, just expand normally
                expanded = expand_token_value(original, shell);
                free(current->value);
                current->value = expanded;
            }
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
    
    // Only adjust if the first token is a command that became empty through expansion
    // Don't adjust if it was originally an empty string literal like ""
    if (current && current->type == COMMAND && 
        (!current->value || current->value[0] == '\0'))
    {
        // Check if this was an expansion that resulted in empty (like $EMPTY)
        // You'll need to add a flag to track this during expansion
        if (current->was_expanded) // Add this flag to t_token struct
        {
            // Look for the next non-empty token to promote to command
            next_cmd = current->next;
            while (next_cmd && (!next_cmd->value || next_cmd->value[0] == '\0'))
                next_cmd = next_cmd->next;
            
            if (next_cmd && next_cmd->type != PIPE && next_cmd->type != REDIRECT &&
                next_cmd->type != HERE_DOC && next_cmd->value && next_cmd->value[0] != '\0')
            {
                // Promote this token to be the command
                next_cmd->type = COMMAND;
                // Mark the empty token so it won't be executed
                current->type = OTHER;
            }
        }
        // If it wasn't expanded (was originally ""), leave it as COMMAND
        // so it will be executed and fail properly
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

char *expand_token_value(char *value, t_shell *shell)
{
    if (!value)
        return (NULL);
    
    // Handle simple tilde expansion case
    if (value[0] == '~' && (value[1] == '\0' || value[1] == '/'))
    {
        char *home = get_env_value(shell->head, "HOME");
        if (home)
            return (ft_strjoin(home, value + 1));
        return (ft_strdup(value));
    }
    
    // Process quotes and expansions
    return process_quotes(value, shell);
}

// New comprehensive quote handling function
char *process_quotes(char *input, t_shell *shell)
{
    char *result = ft_calloc(1, sizeof(char));
    int i = 0;
    char current_quote = 0;
    
    if (!result || !input)
        return (NULL);
    
    while (input[i])
    {
        // Starting a quoted section
        if ((input[i] == '\'' || input[i] == '"') && current_quote == 0)
        {
            current_quote = input[i];
            i++; // Skip the opening quote
            
            // Find the closing quote
            int start = i;
            while (input[i] && input[i] != current_quote)
                i++;
            
            if (input[i] == current_quote) // Found closing quote
            {
                if (current_quote == '\'')
                {
                    // Single quotes - preserve literally, no expansion
                    char *segment = ft_substr(input, start, i - start);
                    char *new_result = ft_strjoin(result, segment);
                    free(result);
                    free(segment);
                    result = new_result;
                }
                else // Double quotes
                {
                    // Expand variables inside double quotes
                    char *segment = ft_substr(input, start, i - start);
                    char *expanded = expand_variables_in_double_quotes(segment, shell);
                    char *new_result = ft_strjoin(result, expanded);
                    free(result);
                    free(segment);
                    free(expanded);
                    result = new_result;
                }
                i++; // Skip the closing quote
                current_quote = 0;
            }
            else // No closing quote found - should not happen with validated input
            {
                free(result);
                return (ft_strdup(input));
            }
        }
        else // Unquoted text
        {
            int start = i;
            // Find next quote or end of string
            while (input[i] && input[i] != '\'' && input[i] != '"')
                i++;
            
            // Process unquoted segment with expansions
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

// Helper to preserve quotes inside quotes
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
            // Append text before $
            if (i > 0)
            {
                char *before = ft_substr(value, 0, i);
                char *temp = ft_strjoin(result, before);
                free(result);
                free(before);
                result = temp;
            }
            
            if (value[i + 1] == '?')
            {
                // Handle $?
                char *exit_str = ft_itoa(shell->exit_code);
                char *temp = ft_strjoin(result, exit_str);
                free(result);
                free(exit_str);
                result = temp;
                
                // Skip $?
                value += (i + 2);
                i = 0;
            }
            else if (ft_isalnum(value[i + 1]) || value[i + 1] == '_')
            {
                // Handle $VAR
                int start = i + 1;
                i++;
                while (value[i] && (ft_isalnum(value[i]) || value[i] == '_'))
                    i++;
                
                char *var_name = ft_substr(value, start, i - start);
                char *var_value = get_env_value(shell->head, var_name);
                if (!var_value)
                    var_value = "";
                
                char *temp = ft_strjoin(result, var_value);
                free(result);
                free(var_name);
                result = temp;
                
                // Skip processed variable
                value += i;
                i = 0;
            }
            else
            {
                // Just a literal $
                char *temp = ft_strjoin(result, "$");
                free(result);
                result = temp;
                value++;
                i = 0;
            }
        }
        else if (value[i] == '\'' || value[i] == '"')
        {
            // Preserve quotes as literal characters
            char quote_str[2] = {value[i], '\0'};
            char *temp = ft_strjoin(result, quote_str);
            free(result);
            result = temp;
            i++;
        }
        else
        {
            // Regular character
            char char_str[2] = {value[i], '\0'};
            char *temp = ft_strjoin(result, char_str);
            free(result);
            result = temp;
            i++;
        }
    }
    
    return result;
}

// For unquoted segments
char *expand_variables(char *value, t_shell *shell)
{
    char *result = ft_calloc(1, sizeof(char));
    int i = 0;
    
    if (!result || !value)
        return (NULL);
    
    while (value[i])
    {
        if (value[i] == '$' && value[i + 1] && !ft_isspace(value[i + 1]))
        {
            // Append text before $
            if (i > 0)
            {
                char *before = ft_substr(value, 0, i);
                char *temp = ft_strjoin(result, before);
                free(result);
                free(before);
                result = temp;
            }
            
            if (value[i + 1] == '?')
            {
                // Handle $?
                char *exit_str = ft_itoa(shell->exit_code);
                char *temp = ft_strjoin(result, exit_str);
                free(result);
                free(exit_str);
                result = temp;
                
                // Skip $?
                value += (i + 2);
                i = 0;
            }
            else if (ft_isalnum(value[i + 1]) || value[i + 1] == '_')
            {
                // Handle $VAR
                int start = i + 1;
                i++;
                while (value[i] && (ft_isalnum(value[i]) || value[i] == '_'))
                    i++;
                
                char *var_name = ft_substr(value, start, i - start);
                char *var_value = get_env_value(shell->head, var_name);
                if (!var_value)
                    var_value = "";
                
                char *temp = ft_strjoin(result, var_value);
                free(result);
                free(var_name);
                result = temp;
                
                // Skip processed variable
                value += i;
                i = 0;
            }
            else
            {
                // Just a literal $
                char *temp = ft_strjoin(result, "$");
                free(result);
                result = temp;
                value++;
                i = 0;
            }
        }
        else
        {
            // Regular character
            char char_str[2] = {value[i], '\0'};
            char *temp = ft_strjoin(result, char_str);
            free(result);
            result = temp;
            i++;
        }
    }
    
    return result;
}

// char	*append_bfr_dolar(char *value, int start, int i, char *result)
// {
// 	char	*temp;
// 	char	*join;

// 	temp = ft_substr(value, start, i - start);
// 	join = ft_strjoin(result, temp);
// 	if (!join)
// 	{
// 		free(join);
// 		return(NULL);
// 	}
// 	if (!temp)
// 		return(NULL);
// 	free(result);
// 	result = join;
// 	free(temp);
// 	return(result);
// }

// char	*append_aft_last(char *value, int start, int i, char *result)
// {
// 	char	*temp;
// 	char	*join;

// 	temp = ft_substr(value, start, i - start);
// 	join = ft_strjoin(result, temp);
// 	if (!join)
// 	{
// 		free(join);
// 		return(NULL);
// 	}
// 	if (!temp)
// 		return(NULL);
// 	free(result);
// 	result = join;
// 	free(temp);
// 	return (result);
// }

// static char	*append_norm(char *value, t_exp_state *state)
// {
//     char	*temp;
//     char	*join;
//     char	*val;

//     temp = ft_substr(value, state->start, state->i - state->start);
//     val = get_env_value(state->shell->head, temp);
//     if (!val)
//         val = "";
//     join = ft_strjoin(state->result, val);
//     free(state->result);
//     free(temp);
//     state->start = state->i;
//     return (join);
// }

// 

// static char	*append_qst(char *value, t_exp_state *state)
// {
//     char	*code;
//     char	*join;

//     if (state->i > state->start)
//         state->result = append_bfr_dolar(value, state->start,
//                 state->i, state->result);
//     state->i += 2;
//     code = ft_itoa(state->shell->exit_code);
//     join = ft_strjoin(state->result, code);
//     free(state->result);
//     free(code);
//     state->start = state->i;
//     return (join);
// }
//static void	expand_token_va_aux(char *value, t_exp_state *state)
// {
//     if (value[state->i] == '\'' || value[state->i] == '"')
//     {
//         if (state->in_quote == 0)
//             state->in_quote = value[state->i];
//         else if (state->in_quote == value[state->i])
//             state->in_quote = 0;
//     }
//     // check for variable expansion only if not inside single quotes
//     if (value[state->i] == '$' && state->in_quote != '\'')
//     {
//         if (value[state->i + 1] == '?')
//         {
//             state->result = append_qst(value, state);
//         }
//         else if (ft_isalnum(value[state->i + 1]) || value[state->i + 1] == '_')
//         {
//             if (state->i > state->start)
//                 state->result = append_bfr_dolar(value, state->start, state->i, state->result);
//             (state->i)++;
//             state->start = state->i;
//             while (value[state->i] && (ft_isalnum(value[state->i]) || value[state->i] == '_'))
//                 (state->i)++;
//             state->result = append_norm(value, state);
//         }
//         else
//         {
//             (state->i)++;
//         }
//     }
//     else
//     {
//         (state->i)++;
//     }
// }