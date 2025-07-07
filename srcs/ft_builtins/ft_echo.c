
#include "../../include/minishell.h"

static void print_echo_args(t_token *token, int start_index)
{
	int first_arg;
	
	first_arg = 1;
	while (token->args && token->args[start_index])
	{
		if (token -> args[start_index][0] != '\0' ||
			(token->args[start_index][0] == '\0' &&
			token->args[start_index - 1] && token->args[start_index - 1][0] != '\0'))
			{
				if (!first_arg)
					ft_putstr_fd(" ", STDOUT_FILENO);
				ft_putstr_fd(token->args[start_index], STDOUT_FILENO);
				first_arg = 0;
			}
			start_index++;
	}
}

static int skip_n(t_token *token, int *i)
{
	int flag;

	flag = 0;
	while(token->args && token->args[*i] && n_flag_validator(token->args[*i]))
	{
		flag = 1;
		(*i)++;
	}
	return flag;
}	

int	ft_echo(t_token *token)
{
	int		flag;
    int		i;

    i = 1;
    flag = skip_n(token, &i);
    print_echo_args(token, i);
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
