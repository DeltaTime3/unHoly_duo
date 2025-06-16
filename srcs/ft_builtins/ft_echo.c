
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
        if (!first_arg)
            printf(" ");
        printf("%s", token->args[i]);
        first_arg = 0;
        i++;
    }
	if (!flag)
		printf("\n");
	return (0);
}
