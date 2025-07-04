
#include "../../include/minishell.h"

int	ft_echo(t_token *token)
{
	int		flag;
    int		first_arg;
    int		i;

    i = 1;
    flag = 0;
    first_arg = 1;
    while (token->args && token->args[i] && n_flag_validator(token->args[i]))
    {
		flag = 1;
        i++;
		for (int j = 0; token->args && token->args[j]; j++)
  			printf("args[%d]: '%s'\n", j, token->args[j]);

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

int	n_flag_validator(const char *arg)
{
	int	i;

	i = 0;
	if (arg[0] != '-')
		return (0);
	i = 1;
	if (arg[i] == '\0')
		return (0);
	while (arg[i])
	{
		if (arg[i] != 'n')
			return (0);
		i++;
	}
	return (1);
}