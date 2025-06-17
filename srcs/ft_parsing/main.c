
#include "minishell.h"

int	main(int ac, char **av, char **envp)
{
	char	*input;
    t_token	*tokens;
    t_shell	shell;
    int     is_valid;
	char	*expanded;
	char	*full_path;

    (void)ac;
    (void)av;
    shell.head = init_shell_env(envp);
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
        if (ft_strlen(input) != 0)
        {
            is_valid = !validate_input(input);
            add_history(input);
            if (is_valid)
            {
                tokens = tokenize_input(input);
                free(input);
                if (tokens)
                {
                	if (is_builtin(tokens))
					{
						ft_execute(&shell, tokens);
					}
					else if (is_sp_expantion(tokens))
					{
						expanded = expand_token_value(tokens->value, &shell);
						printf("%s\n", expanded);
						free(expanded);
					}
					else
					{
						full_path = get_cmd_path(tokens->value, shell.head);
						if (full_path)
						{
							ft_execute(&shell, tokens);
							free(full_path);
						}
						else
						{
							ft_printf_fd(2, "minishell: %s: command not found\n", tokens->value);
							shell.exit_code = 127;
						}                   
					}
					free_tokens(tokens);
                }
            }
            else
                free(input);
        }
        else
            free(input);
    }
    rl_clear_history();
    free_env(shell.head);
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
			end = strchr(value + 2, '}');
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
