
#include "minishell.h"

int	main(int ac, char **av, char **envp)
{
	char	*input;
    t_token	*tokens;
    t_shell	shell;

    (void)ac;
    (void)av;
    shell.head = init_env(envp);
	shell.exit_code = 0;
	shell.return_code = 0;
	shell.prev_dir = NULL;
	shell.curr_dir = NULL;
	shell.pwd = NULL;
	shell.token = NULL;
	shell.type = NULL;
	shell.tail = NULL;
    while (1)
    {
        input = readline("minishell> ");
        if (!input)
            break;
        if (ft_strlen(input) == 0)
        {
            free(input);
        }
        else
        {
            add_history(input);
            tokens = tokenize_input(input);
            free(input);
            if (tokens)
            {
                ft_execute(&shell, tokens);
				print_tokens(tokens);
				free_tokens(tokens);
            }
        }
    }
    rl_clear_history();
    free_env(shell.head);
    return (shell.exit_code);
}
