#include "../../include/minishell.h"

long long	ft_atoll(char *str, int *error)
{
	int					i;
	int					sign;
	unsigned long long	res;

	i = 0;
	sign = 1;
	res = 0;
	*error = 0;
	while (ft_isspace(str[i]))
		i++;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			sign *= -1;
		i++;
	}
	if (!ft_isdigit(str[i]))
		return (*error = 1, 0);
	if (!handle_digits(&str[i], &res, sign, error))
		return (0);
	while (ft_isdigit(str[i]))
		i++;
	if (str[i] != '\0')
		return (*error = 1, 0);
	return (sign * (long long)res);
}

int	handle_digits(char *str, unsigned long long *res, int sign,
	int *error)
{
	*res = 0;
	while (ft_isdigit(*str))
	{
		*res = *res * 10 + (*str - '0');
		if ((sign == 1 && *res > LLONG_MAX) || (sign == -1 && *res
				> (unsigned long long)LLONG_MAX + 1))
		{
			*error = 1;
			return (0);
		}
		str++;
	}
	return (1);
}

void	print_export_list(t_env *head)
{
	t_env	*current;

	current = head;
	while (current)
	{
		if (current->value)
			printf("declare -x %s=\"%s\"\n", current->key, current->value);
		else
			printf("declare -x %s\n", current->key);
		current = current->next;
	}
}
