
#include "minishell.h"

static const char	*get_token_type(t_cat type)
{
	if (type == COMMAND)
		return ("COMMAND");
	else if (type == ARGUMENT)
		return ("ARGUMENT");
	else if (type == OPERATOR)
		return ("OPERATOR");
	else if (type == QUOTE)
		return ("QUOTE");
	else if (type == FLAG)
		return ("FLAG");
	else if (type == COMMENT)
		return ("COMMENT");
	else if (type == PIPE)
		return ("PIPE");
	else if (type == REDIRECT)
		return ("REDIRECT");
	else if (type == HERE_DOC)
		return ("HERE_DOC");
	else if (type == FILES)
		return ("FILES");
	else
		return ("UNKNOWN");
}

void	print_tokens(t_list *tokens)
{
	t_list		*current;
	t_token	*token;

	current = tokens;
	while (current)
	{
		token = (t_token *)current->content;
		printf("Type: %s, Value: '%s'\n", get_token_type(token->type), token->content);
		current = current->next;
	}
}

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

void	process_input(char *input)
{
	t_token	*tokens;

	if (*input)
		add_history(input);
	if (validate_input(input))
	{
		free(input);
		return ;
	}
	tokens = tokenize_input(input);
	if (!tokens)
	{
		free(input);
		return ;
	}
	free(input);
	if (tokens)
	{
		print_tokens(tokens);
		free_tokens(tokens);
	}
}

void	exit_handling(char *input)
{
	printf("exit\n");
	free(input);
	rl_clear_history();
	exit(0);
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