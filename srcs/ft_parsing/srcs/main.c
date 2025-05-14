
#include "minishell.h"

int	main(int ac, char **av, char **envp)
{
	char	*input;
	t_list	*tokens;

	(void)ac;
	(void)av;
	(void)envp;
	tokens = NULL;
	input = NULL;
	while (1)
		input_handling (input, tokens);
	rl_clear_history ();
	return (0);
}
