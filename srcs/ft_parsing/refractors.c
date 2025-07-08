
#include "minishell.h"

int	operator_type(const char *input, int *i, t_cat *type)
{
	if (!input || !input[*i])
		return (1);
	if (input[*i] == '<' && input[*i + 1] == '<')
	{
		*type = HERE_DOC;
		(*i)++;
	}
	else if (input[*i] == '>' && input[*i + 1] == '>')
	{
		*type = REDIRECT;
		(*i)++;
	}
	else if ((input[*i] == '<' || input[*i] == '>'))
		*type = REDIRECT;
	else
		return (1);
	return (0);
}

int	special_tokens_handling(const char *input, int *i, t_token **tokens,
        int *expect_command)
{
    if (!input || !input[*i])
        return (1);
    while (input[*i] && ft_isspace(input[*i]))
        (*i)++;
    if (input[*i] == '|')
    {
        if (pipe_handling(input, i, tokens))
            return (1);
        *expect_command = 1;
        return (0);
    }
	if (input[*i] == '<' && input[*i + 1] == '<')
	{
		if (heredoc_handling(input, i, tokens))
			return (1);
		*expect_command = 0;
		return (0);
	}
    if (input[*i] == '<' || input[*i] == '>')
    {
        if (op_handling(input, i, tokens))
            return (1);
        return (0);
    }
    if (input[*i] == '\'' || input[*i] == '"')
    {
        if (quote_handling(input, i, tokens, expect_command))
            return (1);
        return (0);
    }
    return (0);
}

int	token_helper(const char *input, int *i, t_token **tokens,
		int *expect_command)
{
	int	result;

	while (input[*i])
	{
		while (input[*i] && ft_isspace(input[*i]))
			(*i)++;
		if (!input[*i])
			break ;
		result = token_handling(input, i, tokens, expect_command);
		if (result == 1)
		{
			free_tokens(*tokens);
			*tokens = NULL;
			return (1);
		}
		else if (result == 2)
			break ;
	}
	return (0);
}

void	skip_special_chars(const char *input, int *i)
{
	while (input[*i] && !ft_isspace(input[*i]) && input[*i] != '\''
		&& input[*i] != '"' && input[*i] != '#' && input[*i] != '<'
		&& input[*i] != '>' && input[*i] != '|')
		(*i)++;
}

t_cat	determine_token_type(const char *value, int *expect_command)
{
	t_cat	type;

	if (*expect_command == 1)
	{
		type = COMMAND;
		*expect_command = 2;
	}
	else if (value[0] == '-' && *expect_command == 2)
	{
		type = FLAG;
		*expect_command = 0;
	}
	else
	{
		type = ARGUMENT;
	}
	return (type);
}


