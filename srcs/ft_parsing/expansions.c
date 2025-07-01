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

char    *expand_token_value(char *value, t_shell *shell)
{
    char *result;
    char *temp;
    char *home;

    if (value[0] == '~' && (value[1] == '\0' || value[1] == '/'))
    {
        home = get_env_value(shell->head, "HOME");
        if (home)
        {
            temp = ft_strjoin(home, value + 1);
            result = temp;
        }
        else
            return (ft_strdup(value));
    }
    else if (ft_strcmp(value, "$?") == 0)
        result = expand_exit_status(value, shell);
    else if (value[0] == '$' && value[1] != '\0')
        result = expand_env_var(value, shell);
    else
        result = ft_strdup(value);
    return (result);
}