#include "minishell.h"

// files
int	file_handling(const char *input, int *i, t_list **tokens)
{
	size_t	start;
	char	*value;

	while (input[*i] && ft_isspace(input[*i]))
		(*i)++;
	start = *i;
	while (input[*i] && !ft_isspace(input[*i]))
		(*i)++;
	value = ft_substr(input, start, *i - start);
	if (!value)
		return (1);
	ft_lstadd_back(tokens, ft_lstnew(create_token(FILES, value)));
	return (0);
}

// quotes 
int	quote_handling(const char *input, int *i, t_list **tokens, int *expect_command)
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
	if (*expect_command == 1)
    {
        type = COMMAND;
        *expect_command = 2;
    }
    else if (*expect_command == 2 )
    {
        type = ARGUMENT;
        *expect_command = 3;
    }
    else
    {
        type = ARGUMENT;
    }
	ft_lstadd_back(tokens, ft_lstnew(create_token(type, value)));
	(*i)++;
	return (0);
}

// operators
int	op_handling(const char *input, int *i, t_list **tokens)
{
	size_t	start;
	char	*value;
	t_cat	type;

	start = *i;
	value = NULL;
	if (operator_type(input, i, &type))
		return (1);
	value = ft_substr(input, start, *i - start + 1);
	if (!value)
		return (1);
	if (input[*i + 1] == '>' || input[*i + 1] == '<')
	{
		syntax_err_handling(value, tokens);
		return (1);
	}
	ft_lstadd_back(tokens, ft_lstnew(create_token(type, value)));
	(*i)++;
	while (input[*i] && ft_isspace(input[*i]))
		(*i)++;
	if (input[*i] && file_handling(input, i, tokens))
		return (1);
	return (0);
}

// words
int	word_handling(const char *input, int *i, t_list **tokens,
	int *expect_command)
{
	size_t	start;
	char	*value;
	t_cat	type;

	start = *i;
	while (input[*i] && !ft_isspace(input[*i]) && input[*i] != '\''
		&& input[*i] != '"' && input[*i] != '#' && input[*i] != '<'
		&& input[*i] != '>' && input[*i] != '|')
		(*i)++;
	value = ft_substr(input, start, *i - start);
	if (!value)
		return (1);
	if (*expect_command == 1)
    {
        type = COMMAND;
        *expect_command = 2;
    }
    else if (*expect_command == 2 )
        type = ARGUMENT;
    else
    {
        type = ARGUMENT;
    }
	ft_lstadd_back(tokens, ft_lstnew(create_token(type, value)));
	return (0);
}

// pipes
int	pipe_handling(const char *input, int *i, t_list **tokens)
{
	size_t	start;
	char	*value;

	start = *i;
	(*i)++;
	value = ft_substr(input, start, *i - start);
	if (!value)
		return (1);
	if (input[*i] && input[*i + 1] == '|')
	{
		syntax_err_handling(value, tokens);
		return (1);
	}
	ft_lstadd_back(tokens, ft_lstnew(create_token(PIPE, value)));
	return (0);
}

// token handling
int	token_handling(const char *input, int *i, t_list **tokens,
		int *expect_command)
{
	if (!input || !input[*i])
		return (1);
	if (special_tokens_handling(input, i, tokens, expect_command))
		return (1);
	if (input[*i] == '#')
		return (2);
	if (ft_isalpha(input[*i]) || input[*i] == '\'' || input[*i] == '"')
	{
		if (*expect_command)
		{
			if (word_handling(input, i, tokens, expect_command))
				return (1);
			*expect_command = 0;
		}
		else
		{
			if (word_handling(input, i, tokens, expect_command))
				return (1);
		}
	}
	return (0);
}
