#include "minishell.h"

void	free_tokens(t_token *tokens)
{
    t_token *current;
    t_token *next;

    current = tokens;
    while (current)
    {
        next = current->next;
        free(current->value);
        free(current->content);
        free(current->args);
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
int	quote_handling(const char *input, int *i, t_token **tokens,
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
	add_token(tokens, create_token(type, value));
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
