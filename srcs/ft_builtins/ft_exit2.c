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
