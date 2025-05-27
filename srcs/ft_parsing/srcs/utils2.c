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
			free(token->content);
			free(token);
		}
		free(current);
		current = next;
	}
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
