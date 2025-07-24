#include "../../include/minishell.h"

int	ft_exit(t_token **token, t_shell *type)
{
	int		nbr_args;
	char	*sus_arg;
	char	*arg;

	arg = NULL;
	nbr_args = token_counter(*token);
	if (nbr_args > 1)
	{
		sus_arg = (*token)->next->value;
		if (is_valid_exit_arg(sus_arg))
			arg = sus_arg;
		else
			arg = NULL;
	}
	if (nbr_args == 2 && arg && ft_strcmp(arg, "--") == 0)
		ft_kill(type, *token, 0);
	return (exit_args(token, type, nbr_args, arg));
}

int	exit_args(t_token **token, t_shell *type, int nbr_args, char *arg)
{
	if (nbr_args == 1)
		ft_kill(type, *token, 0);
	if (arg == NULL || !ft_is_nbr(arg))
	{
		if (arg)
			ft_printf_fd(2, "exit1: %s: numeric argument required\n", arg);
		else
			ft_printf_fd(2, "%s numeric argument required\n");
		ft_kill(type, *token, 2);
	}
	if (nbr_args > 2)
	{
		ft_putstr_fd(" too many arguments\n", STDERR_FILENO);
		type->exit_code = 1;
		return (1);
	}
	ft_exit2(type, *token, nbr_args, arg);
	return (0);
}

int	ft_exit2(t_shell *type, t_token *tokens, int nbr_args, char *arg)
{
	long long	code;
	int			atol_error;

	atol_error = 0;
	if (nbr_args == 1)
		ft_kill(type, tokens, 0);
	if (nbr_args == 2)
	{
		code = ft_atoll(arg, &atol_error);
		if (atol_error != 0)
		{
			ft_printf_fd(2, "exit: %s: numeric argument required\n", arg);
			ft_kill(type, tokens, 2);
		}
		code = ((code % 256) + 256) % 256;
		ft_kill(type, tokens, (int)code);
	}
	return (0);
}

int	ft_is_nbr(char *str)
{
	int	i;

	i = 0;
	if ((!str) || (*str == '\0'))
		return (0);
	if (str[0] == '+' || str[0] == '-')
		++i;
	if (!str[i])
		return (0);
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	return (1);
}

void	ft_kill(t_shell *type, t_token *tokens, int e_code)
{
	free_tokens(tokens);
	clean_all_resources(type);
	rl_clear_history();
	rl_cleanup_after_signal();
	exit(e_code);
}
