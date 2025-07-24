#include "../../include/minishell.h"

static void	print_arg_with_quotes(char *arg, int quote_type)
{
	if (!arg)
		return ;
	if (quote_type == 0)
	{
		ft_putstr_fd(arg, STDOUT_FILENO);
	}
	else
	{
		ft_putstr_fd(arg, STDOUT_FILENO);
	}
}

static int	print_echo_args(t_token *token, int start_index)
{
	int	first_arg;
	int	print;

	first_arg = 1;
	print = 0;
	while (token->args && token->args[start_index])
	{
		if (!first_arg)
			ft_putstr_fd(" ", STDOUT_FILENO);
		print_arg_with_quotes(token->args[start_index], token->quote_type);
		first_arg = 0;
		print = 1;
		start_index++;
	}
	return (print);
}

static int	skip_n(t_token *token, int *i)
{
	int	flag;

	flag = 0;
	while (token->args && token->args[*i] && n_flag_validator(token->args[*i]))
	{
		flag = 1;
		(*i)++;
	}
	return (flag);
}

int	ft_echo(t_token *token)
{
	int	flag;
	int	i;
	int	print;

	i = 1;
	flag = skip_n(token, &i);
	print = print_echo_args(token, i);
	if (!flag || !print)
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
