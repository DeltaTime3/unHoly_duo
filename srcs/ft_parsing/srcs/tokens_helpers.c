#include "minishell.h"

// quotes 
int	quote_handling(const char *input, size_t *i, t_list **tokens)
{
	char	quote;
	size_t	start;
	char	*value;
	int		unmatched_quote;

	unmatched_quote = 0;
	quote = input[(*i)++];
	start = *i;
	while (input[*i] && input[*i] != quote)
		(*i)++;
	if (input[*i] != quote)
	{
		unmatched_quote = 1;
		return (1);
	}
	value = ft_substr(input, start, *i - start);
	if (!value)
		return (1);
	ft_lstadd_back(tokens, ft_lstnew(create_token(ARGUMENT, value)));
	(*i)++;
	return (0);
}

// operators
int	op_handling(const char *input, size_t *i, t_list **tokens)
{
	size_t	start;
    char	*value;
    t_cat	type;

    start = *i;
    if (input[*i] == '<' && input[*i + 1] == '<')
    {
        type = REDIRECT;
        (*i)++; // Move past the first '>'
    }
    else if (input[*i] == '>' && input[*i + 1] == '>')
    {
        type = HERE_DOC;
		(*i)++; // Move past the first '<'
    }
    else if (input[*i] == '>' || input[*i] == '<')
        type = REDIRECT;
    else
        return (1); // Not a valid operator
    value = ft_substr(input, start, *i - start + 1); // Extract the operator
    if (!value)
        return (1);
    ft_lstadd_back(tokens, ft_lstnew(create_token(type, value))); // Add token
    printf("Added redirect token: '%s'\n", value);
    (*i)++; // Move past the operator
    return (0);
}

// words
int	word_handling(const char *input, size_t *i, t_list **tokens,
	int *expect_command)
{
	size_t	start;
	char	*value;
	t_cat	type;

	start = *i;
	*expect_command = 1;
	while (input[*i] && !ft_isspace(input[*i]) && input[*i] != '\''
		&& input[*i] != '"' && input[*i] != '#' && input[*i] != '<'
		&& input[*i] != '>')
		(*i)++;
	value = ft_substr(input, start, *i - start);
	if (!value)
		return (1);
	if (*expect_command)
		type = COMMAND;
	else
		type = ARGUMENT;
	*expect_command = 0;
	ft_lstadd_back(tokens, ft_lstnew(create_token(type, value)));
	return (0);
}

// pipes
int	pipe_handling(const char *input, size_t *i, t_list **tokens)
{
	size_t	start;
	char	*value;

	start = *i;
	value = ft_substr(input, start, *i - start + 1);
	if (!value)
		return (1);
	ft_lstadd_back(tokens, ft_lstnew(create_token(PIPE, value)));
	printf("Added operator token: '%s'\n",value);
	(*i)++;
	return (0);
}

// token handling
int	token_handling(const char *input, size_t *i, t_list **tokens,
		int *expect_command)
{
	if (input[*i] == '\'' || input[*i] == '"')
	{
		if (quote_handling(input, i, tokens))
			return (1);
	}
	else if (input[*i] == '#')
		return (2);
	else if (input[*i] == '|')
	{
		if (pipe_handling(input, i, tokens))
			return (1);
	}
	else if (input[*i] == '<' || input[*i] == '>')
	{
		if (op_handling(input, i, tokens))
			return (1);
		*expect_command = 1;
	}
	else if (ft_isalnum(input[*i]))
	{
		if (word_handling(input, i, tokens, expect_command))
			return (1);
	}
	return (0);
}
