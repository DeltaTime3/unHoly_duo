#include "../../include/minishell.h"

int	val_empt_operat(const char *args)
{
	if (args[0] == '\0' || !ft_strcmp(args, "=") || !ft_strcmp(args, "+="))
		return (1);
	return (0);
}

void	print_export_token(t_env *head)
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
