#include "../../include/minishell.h"

int	token_counter(t_token *token)
{
	int		count;
	t_token	*curr;

	count = 0;
	curr = token;
	while (curr)
	{
		count++;
		curr = curr->next;
	}
	return (count);
}

int	is_valid_exit_arg(char *arg)
{
	if (!arg)
		return (0);
	if (ft_strcmp(arg, "|") == 0)
		return (0);
	return (1);
}
