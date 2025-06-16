
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

// char    *expand_token_value(char *value, t_shell *shell)
// {
//     char    *result;
//     char    *temp;
//     char    *home;

//     if (value[0] == '~' && (value[1] == '\0' || value[1] == '/'))
//     {
//         home = get_env_value(shell->head, "HOME");
//         if (home)
//         {
//             temp = ft_strjoin(home, value + 1);
//             result = temp;
//         }
//     }
//     else if (ft_strcmp(value, "$?") == 0)
//         result = expand_exit_status()
// }
