#include "minishell.h"

t_token	*create_token(t_cat type, char *content)
{
	t_token	*token;

	token = malloc(sizeof(t_token));
	if (!token)
	{
		printf("Error: Memory allocation failed for token.\n");
		return (NULL);
	}
	token->type = type;
	token->content = content;
	return (token);
}

t_list	*tokenize_input(const char *input)
{
	t_list	**tokens;
	t_list	*final_tokens;
	int		i;
	int		expect_command;
	int		result;

	tokens = malloc(sizeof(t_list *));
	if (!tokens)
		return (NULL);
	*tokens = NULL;
	i = 0;
	expect_command = 1;
	if (!input || ft_strlen(input) == 0 || ft_isspace(input[0]))
	{
		free(tokens);
		return (NULL);
	}
	while (input[i])
	{
		result = token_helper(input, &i, tokens, &expect_command);
		if (result == 1)
		{
			free_tokens(*tokens);
			free(tokens);
			return (NULL);
		}
	}
	final_tokens = *tokens;
	free(tokens);
	return (final_tokens);
}
