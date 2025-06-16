
#include "../../include/minishell.h"

int	ft_echo(t_token *token)
{
	int		flag;
	int		first_arg;
	t_token	*curr;

	flag = 0;
	first_arg = 1;
	curr = token->next;
	while (curr && curr->value && ft_strcmp(curr->value, "-n") == 0)
	{
		flag = 1;
		curr = curr->next;
	}
	while (curr)
	{
		if (!first_arg)
			printf(" ");
		if (curr->type == ARGUMENT)
			printf("%s", curr->value);
		first_arg = 0;
		curr = curr->next;
	}
	if (!flag)
		printf("\n");
	return (0);
}
