
#include "minishell.h"

void	input_handling(char *input, t_token *tokens)
{
	(void)tokens;
	input = readline("minishell> ");
	if (!input)
		exit_handling(input);
	else if (ft_strlen(input) == 0)
		free(input);
	else if (ft_strcmp(input, "exit") == 0)
		exit_handling(input);
	else
		process_input(input);
}

void process_input(char *input)
{
    t_token *tokens;
    if (*input)
        add_history(input);
    if (validate_input(input))
    {
        free(input);
        return;
    }
    tokens = tokenize_input(input);
    if (!tokens)
    {
        free(input);
        return;
    }
    free(input);
    if (tokens)
        free_tokens(tokens);
}

void	exit_handling(char *input)
{
	printf("exit\n");
	free(input);
	rl_clear_history();
	exit(0);
}

void	add_token(t_token **head, t_token *new_token)
{
	t_token *current;
	
	if (!head || !new_token)
		return ;
	if (*head == NULL)
	{
		*head = new_token;
		return ;
	}
	current = *head;
	while (current->next)
		current = current->next;
	current->next = new_token;
}

// int	syntax_err_handling(char *value, t_list **tokens)
// {
// 	ft_printf_fd(UNEXPECTED_TOKEN, 2);
// 	if (tokens)
// 	{
// 		free_tokens(*tokens);
// 		free(value);
// 		*tokens = NULL;
// 	}
// 	return (1);
// }

// int	command_err_handling(char *value, t_list **tokens)
// {
// 	ft_printf_fd(COMMAND_NOT_FOUND, 2);
// 	if (tokens)
// 	{
// 		free_tokens(*tokens);
// 		free(value);
// 		*tokens = NULL;
// 	}
// 	return (1);
// }
