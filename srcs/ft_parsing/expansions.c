
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

    len = ft_strlen(input);
    if ((input[0] == '\'' && input[len - 1] == '\'') ||
            (input[0] == '"' && input[len - 1] == '"'))
            return (ft_substr(input, 1, len - 2));
    return (ft_strdup(input));
}

void expand_tokens(t_token *token, t_shell *shell)
{
    char *expanded;
    int i;
    char    *final;
    
    i = 0;
    printf("Token value: '%s'\n", token->value);
    printf("Token args: ");
    if (token->args) {
        i = 0;
        while (token->args[i]) {
            printf("'%s' ", token->args[i]);
            i++;
        }
    } else {
        printf("(null)");
    }
    printf("\n");
    i = 0;
    if (token->value)
    {
        expanded = expand_token_value(token->value, shell);
        final = remove_quotes(expanded);
        free(token->value);
        free(expanded);
        token->value = expanded;
    }
    if (token->args)
    {
        while (token->args[i])
        {
            expanded = expand_token_value(token->args[i], shell);
            final = remove_quotes(expanded);
            free(token->args[i]);
            token->args[i] = expanded;
            i++;
        }
        token->args[i] = NULL;
    }
}

char    *expand_token_value(char *value, t_shell *shell)
{
    char *result;
    char *temp;
    char *home;

    printf("Expanding: '%s'\n", value);
    if (value[0] == '~' && (value[1] == '\0' || value[1] == '/'))
    {
        home = get_env_value(shell->head, "HOME");
        printf("HOME value: '%s'\n", home ? home : "NULL");
        if (home)
        {
            temp = ft_strjoin(home, value + 1);
            printf("Expanded to: '%s'\n", temp);
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
