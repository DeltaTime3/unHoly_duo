#include "minishell.h"

void	free_tokens(t_list *tokens)
{
	t_list	*current;
	t_list	*next;
	t_token	*token;

	current = tokens;
	while (current)
	{
		next = current->next;
		token = (t_token *)current->content;
		if (token)
		{
			if (token->content)
				free(token->content);
			if (token->args)
				free_args(token->args);
			free(token);
		}
		free(current);
		current = next;
	}
}

void	free_args(char **args)
{
	int	i;

	i = 0;
	while (args[i])
		free(args[i++]);
	free(args);
}

// quotes 
int	quote_handling(const char *input, int *i, t_list **tokens,
	int *expect_command)
{
	char	quote;
	size_t	start;
	char	*value;
	t_cat	type;

	quote = input[(*i)++];
	start = *i;
	while (input[*i] && input[*i] != quote)
		(*i)++;
	if (input[*i] != quote)
		return (1);
	value = ft_substr(input, start, *i - start);
	if (!value)
		return (1);
	type = determine_token_type(value, expect_command);
	ft_lstadd_back(tokens, ft_lstnew(create_token(type, value)));
	(*i)++;
	return (0);
}

bool	open_pipe(const char *input, int i)
{
	if (input[i] == '|')
	{
		i++;
		while (input[i] == ' ')
			i++;
		if (input[i] == '\0')
			return (true);
	}
	return (false);
}

static int	is_invalid_sequence(const char *input, int i)
{
	if (input[i + 1] && input[i + 2])
    {
        if ((input[i] == '|' && input[i + 2] == '|') ||
            (input[i] == '>' && input[i + 2] == '>') ||
            (input[i] == '<' && input[i + 2] == '<') ||
            (input[i] == '&' && input[i + 2] == '&'))
	            return (1);
    }
    return (0);
}

int	check_token_sequence(const char *input)
{
	int	i;

	i = 0;
	while (input[i])
	{
		while (input[i] && ft_isspace(input[i]))
			i++;
		if (is_invalid_sequence(input, i))
        {
            ft_printf_fd(2, UNEXPECTED_TOKEN);
            return (1);
        }
		if ((input[i] == '|' || input[i] == '<' || input[i] == '>') &&
            (i == 0 || input[i + 1] == '\0'))
        {
            ft_printf_fd(2, UNEXPECTED_TOKEN);
            return (1);
        }
		i++;
	}
	return (0);
}
