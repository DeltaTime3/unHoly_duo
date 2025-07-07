
#include "minishell.h"

int	check_unmatched_quotes(const char *input)
{
	int		i;
	char	quote;

	i = 0;
	quote = '\0';
	while (input[i])
	{
		if (input[i] == '\'' || input[i] == '"')
		{
			if (quote == '\0')
				quote = input[i];
			else if (quote == input[i])
				quote = '\0';
		}
		i++;
	}
	if (quote != '\0')
	{
		ft_printf_fd(2, UNCLOSED_QUOTES);
		return (1);
	}
	return (0);
}

int	check_logical_operators(const char *input)
{
	int	i;
	char	quote;

	i = 0;
	quote = '\0';
	while (input[i])
	{
		if (input[i] == '\'' || input[i] == '"')
		{
			if (quote == '\0')
				quote = input[i];
			else if (quote == input[i])
				quote = '\0';
		}
		if (quote == '\0' && ((input[i] == '&' && input[i + 1] == '&')
            || (input[i] == '|' && input[i + 1] == '|')))
		{
			ft_printf_fd(2, LOGICAL_OPERATORS);
			return (1);
		}
		i++;
	}
	return (0);
}

int	check_unexpected_tokens(const char *input)
{
	int	i;

    i = ft_strlen(input);
    if (i == 0)
        return (0);
    i--;
    while (i >= 0 && ft_isspace(input[i]))
        i--;
    if (i >= 0 && (input[i] == '<' || input[i] == '>' || input[i] == '&'))
    {
        ft_printf_fd(2, UNEXPECTED_TOKEN);
        return (1);
    }
    return (0);
}

int	validate_input(const char *input)
{
	int	i;

	i = 0;
	if (*input)
		add_history(input);
	while (input[i] && ft_isspace(input[i]))
		i++;
	if (input[i] == '\0')
		return (0);
	if (input[i] == '\n' || ft_isspace(input[i]))
		printf("\n");
	if (check_unmatched_quotes(input))
		return (1);
	if (check_logical_operators(input))
		return (1);
	if (check_unexpected_tokens(input))
		return (1);
	if (check_token_sequence(input))
		return (1);
	return (0);
}

int	handle_quote_error(t_token **tokens)
{
	free_tokens(*tokens);
	*tokens = NULL;
	return (1);
}


// int is_builtin(const char *cmd)
// {
//     if (!cmd)
//         return (0);
//     if (ft_strcmp(cmd, "echo") == 0 || ft_strcmp(cmd, "cd") == 0 ||
//         ft_strcmp(cmd, "pwd") == 0 || ft_strcmp(cmd, "export") == 0 ||
//         ft_strcmp(cmd, "unset") == 0 || ft_strcmp(cmd, "env") == 0)
//         return (1);
//     return (0);
// }

// int execute_command(t_token *command_token)
// {
//     if (ft_isdigit(command_token->content[0]))
//     {
//         ft_printf_fd(2, "minishell: %s: command not found\n", command_token->content);
//         return (127);
//     }
//     if (!is_builtin(command_token->content))
//     {
//         ft_printf_fd(2, "minishell: %s: command not found\n", command_token->content);
//         return (127);
//     }
//     return (0);
// }
