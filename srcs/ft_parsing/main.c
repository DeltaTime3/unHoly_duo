#include "minishell.h"

int	global_sig;

int	main(int ac, char **av, char **envp)
{
	char	*input;
    t_token	*tokens;
    t_shell	shell;
	char	**orig_envp;
	
	orig_envp = envp;
    (void)ac;
    (void)av;
    shell.head = init_shell_env(orig_envp);
	shell.exit_code = 0;
	shell.return_code = 0;
	shell.prev_dir = NULL;
	shell.curr_dir = NULL;
	shell.pwd = NULL;
	shell.token = NULL;
	shell.type = NULL;
	shell.tail = NULL;
	shell.value = NULL;
	shell.heredoc_fd = -1;
	ft_signals();
    while (1)
    {
        input = readline("minishell> ");
		signal_process(&shell);
        if (!input)
		{
    		write(STDOUT_FILENO, "exit\n", 5);
			break;
		}
        
        if (ft_strlen(input) != 0)
        {
            add_history(input);
            if (!validate_input(input))
            {
                tokens = tokenize_input(input);
                if (tokens)
                {
                    ft_execute(&shell, tokens);
                    free_tokens(tokens);
                    tokens = NULL;
                }
            }
        }
        free(input);
		input = NULL;
    }
	clean_all_resources(&shell);
	rl_clear_history();
	rl_cleanup_after_signal();
    return (shell.exit_code);
}

int	is_sp_expantion(t_token *token)
{
	const char	*value;
	const char	*end;
	int			i;

	value = token->value;
	if (!value || !*value)
		return (0);
	if (value[0] == '~' && (value[1] == '\0' || value[1] == '/'))
		return (1);
	if (value[0] == '$')
	{
		if (value[1] == '{')
		{
			end = ft_strchr(value + 2, '}');
			if (end && end[1] == '\0')
				return (1);
		}
		else if (ft_isalpha(value[1]) || value[1] == '_')
		{
			i = 2;
			while (value[i] && (ft_isalnum(value[i]) || value[i] == '_'))
				i++;
			if(value[i] == '\0')
				return(1);
		}
	}
	return (0);
}