
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

int	special_tokens_handling(const char *input, int *i, t_list **tokens,
		int *expect_command)
{
	if (!input || !input[*i])
        return (1);
    while (input[*i] && ft_isspace(input[*i]))
        (*i)++;
    if (input[*i] == '|')
    {
        *expect_command = 1;
        if (pipe_handling(input, i, tokens))
            return (1);
        return (0);
    }
    if (input[*i] == '<' || input[*i] == '>')
    {
        *expect_command = 1;
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
	printf("special_tokens_handling: Current char: %c\n", input[*i]);
 	return (0);
}

int	token_helper(const char *input, int *i, t_list **tokens,
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
			free(tokens);
			return (1);
		}
		else if (result == 2)
			break ;
	}
	return (0);
}