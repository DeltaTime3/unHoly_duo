
#include "../../include/minishell.h"

int	ft_echo(t_token *token)
{
	int		flag;
    int		first_arg;
    int		i;

    i = 1;
    flag = 0;
    first_arg = 1;
    while (token->args && token->args[i] && ft_strcmp(token->args[i], "-n") == 0)
    {
        flag = 1;
        i++;
    }
    while (token->args && token->args[i])
    {
        if (token->args[i][0] != '\0')
        {
            if (!first_arg)
                ft_putstr_fd(" ", STDOUT_FILENO);
            ft_putstr_fd(token->args[i], STDOUT_FILENO);
            first_arg = 0;
        }
        i++;
    }
    if (!flag)
        ft_putstr_fd("\n", STDOUT_FILENO);
    return (0);
}
