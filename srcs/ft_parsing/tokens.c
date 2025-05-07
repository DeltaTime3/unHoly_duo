#include "minishell.h"

t_token	*create_token(t_cat type, char *content)
{
	t_token	*token;

	token = malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->type = type;
	token->content = content;
	return (token);
}

t_list	*tokenize_input(const char *input)
{
	t_list	*tokens;
	size_t	i;
	int		expect_command;
	int		result;

	tokens = NULL;
	i = 0;
	while (input[i])
	{
		while (input[i] && ft_isspace(input[i]))
			i++;
		if (!input[i])
			break ;
		result = token_handling(input, &i, &tokens, &expect_command);
		if (result == 1)
			return (NULL);
		else if (result == 2)
			break ;
	}
	return (tokens);
}
