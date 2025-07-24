#include "../../include/minishell.h"

int	ft_export(t_shell *type, char **args)
{
	int		status;

	status = 0;
	if (!args[1])
	{
		sort_env(type->head);
		print_export_token(type->head);
		return (0);
	}
	status = export_while_args(type, args);
	type->exit_code = status;
	return (status);
}

int	export_while_args(t_shell *type, char **args)
{
	int		i;
	int		status;
	char	*exp_arg;

	i = 1;
	status = 0;
	while (args[i])
	{
		exp_arg = expand_token_arg_to_value(args[i], type);
		if (validate_args(args[i]))
		{
			ft_printf_fd(2, "minishell: export: `%s': %s\n",
				args[i], E_INVALID_ID);
			status = 1;
		}
		else
			process_export(type, args[i]);
		free(exp_arg);
		i++;
	}
	return (status);
}

int	validate_args(char *args)
{
	if (val_empt_operat(args) || val_fst_char(args)
		|| val_var_name(args))
	{
		return (1);
	}
	return (0);
}

int	val_fst_char(const char *args)
{
	if (!ft_isalpha(args[0]) && args[0] != '_')
		return (1);
	return (0);
}

int	val_var_name(const char *args)
{
	int	j;

	j = 0;
	while ((args[j] && args[j] != '=')
		&& !(args[j] == '+' && args[j + 1] == '='))
	{
		if (!ft_isalnum(args[j]) && args[j] != '_')
			return (1);
		j++;
	}
	return (0);
}
