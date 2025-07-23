#include "minishell.h"

char	*expand_variables(char *value, t_shell *shell);
//static void	expand_token_va_aux(char *value, t_exp_state *state);
//static char	*append_qst(char *value, t_exp_state *state);
//static char	*append_norm(char *value, t_exp_state *state);
//static char	*append_bfr_dolar(char *value, int start, int i, char *result);
//static char	*append_aft_last(char *value, int start, int i, char *result);

// static void debug_print_tokens(t_token *tokens)
// {
//     t_token *current = tokens;
//     int i = 0;
    
//     printf("=== TOKEN DEBUG ===\n");
//     while (current)
//     {
//         printf("Token %d: type=%d, value=\"%s\", in_single_quotes=%d\n", 
//                i++, current->type, current->value, current->in_single_quotes);
//         current = current->next;
//     }
//     printf("==================\n");
// }

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
    
    //debug_print_tokens(token);
    while (current)
    {
        if (current->value && current->value[0] != '\0')
        {
            original = current->value;
            
            // Check if token was in single quotes - if so, don't expand
            if (current->in_single_quotes)
            {
                // Just copy the value without any expansion
                expanded = ft_strdup(original);
            }
            else
            {
                // Process through expand_token_value to handle quotes properly
                expanded = expand_token_value(original, shell);
            }
            
            // Check if the expansion resulted in an empty string
            if (expanded && expanded[0] == '\0' && ft_strchr(original, '$') != NULL)
                current->was_expanded = 1;
            
            free(current->value);
            current->value = expanded;
        }
        current = current->next;
    }
    // printf("After expansion:\n");
    // debug_print_tokens(token);
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
    // Check if the value contains quotes
    // If it starts and ends with single quotes, it might be from "'$VAR'" input
    // In this case, we need to expand the content but preserve the quotes
    if (value[0] == '\'' && value[ft_strlen(value) - 1] == '\'')
    {
        char *inner = ft_substr(value, 1, ft_strlen(value) - 2);
        char *expanded = expand_variables(inner, shell);
        // Allocate space for: opening quote + expanded + closing quote + null terminator
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
    // Process quotes and expansions
    return process_quotes(value, shell);
}

// New comprehensive quote handling function
// ...existing code...
char *process_quotes(char *input, t_shell *shell)
{
    char *result = ft_calloc(1, sizeof(char));
    int i = 0;
    
    if (!result || !input)
        return (NULL);
    while (input[i])
    {
        // Check for quotes
        if (input[i] == '\'' || input[i] == '"')
        {
            char quote_type = input[i];
            i++; // Skip the opening quote
            
            // Find the closing quote
            int start = i;
            while (input[i] && input[i] != quote_type)
                i++;
            
            if (input[i] == quote_type) // Found closing quote
            {
                char *segment = ft_substr(input, start, i - start);
                char *processed;
                
                if (quote_type == '\'')
                {
                    // Single quotes - preserve everything literally, NO expansion
                    processed = segment;
                }
                else // Double quotes
                {
                    // Double quotes - expand variables but preserve other quotes
                    processed = expand_variables_in_double_quotes(segment, shell);
                    free(segment);
                }
                
                char *new_result = ft_strjoin(result, processed);
                free(result);
                free(processed);
                result = new_result;
                
                i++; // Skip the closing quote
            }
            else // No closing quote found
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
// ...existing code...

// Helper to preserve quotes inside quotes
// ...existing code...
char *expand_variables(char *value, t_shell *shell)
{
    char *result = ft_calloc(1, sizeof(char));
    int i = 0;
    
    if (!result || !value)
        return (NULL);
    
    while (value[i])
    {
        if (value[i] == '$')
        {
            // Check if the next character is valid for a variable name
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
                // Just a literal $ (not followed by a valid variable name)
                // This block is already correct for handling literal '$'
                char *temp = ft_strjoin(result, "$");
                free(result);
                result = temp;
                i++; // Move to the next character
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
        // Debug output to track expansion
    }
    
    return result;
}


// For unquoted segments
// ...existing code...
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
