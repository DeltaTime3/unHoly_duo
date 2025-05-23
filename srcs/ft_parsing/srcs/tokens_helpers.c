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
    else if (value[0] == '-' && *expect_command == 2)
        type = FLAG;
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
    else if (value[0] == '-' && *expect_command == 2)
        type = FLAG;
    else
    {
        type = ARGUMENT;
    }
    ft_lstadd_back(tokens, ft_lstnew(create_token(type, value)));
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

// pipes
int	pipe_handling(const char *input, int *i, t_list **tokens)
{
	size_t	start;
	char	*value;

	while (input[*i] && ft_isspace(input[*i]))
        (*i)++;
	if (!input[*i] || (input[*i] == '|' && (input[*i + 1] == '\0' || input[*i + 2] == ' ')))
	{
		ft_printf_fd(2, OPEN_PIPE);
		return (1);
	}
	start = *i;
	(*i)++;
	value = ft_substr(input, start, *i - start);
	if (!value)
		return (1);
	ft_lstadd_back(tokens, ft_lstnew(create_token(PIPE, value)));
	while (input[*i] && ft_isspace(input[*i]))
        (*i)++;
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
	if (ft_isalpha(input[*i]) || input[*i] == '\'' || input[*i] == '"' || input[*i] == '-')
	{
		if (*expect_command && !ft_isdigit(input[*i]))
		{
			if (word_handling(input, i, tokens, expect_command))
				return (1);
			// *expect_command = 0;
		}
		else
		{
			if (word_handling(input, i, tokens, expect_command))
				return (1);
		}
		return (0);
	}
	else
	{	
		ft_printf_fd(2, COMMAND_NOT_FOUND);
		return (1);
	}
	return (0);
}