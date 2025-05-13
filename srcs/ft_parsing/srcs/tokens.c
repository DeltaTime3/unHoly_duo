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
	t_list **tokens;
	t_list	*final_tokens;
    size_t i;
    int expect_command;
    int result;

    tokens = malloc(sizeof(t_list *));
    if (!tokens)
        return (NULL);
    *tokens = NULL;
    i = 0;
    expect_command = 1;
    if (!input)
    {
        free(tokens);
        return (NULL);
    }
    while (input[i])
    {
        while (input[i] && ft_isspace(input[i]))
            i++;
        if (!input[i])
            break;
        result = token_handling(input, &i, tokens, &expect_command);
        if (result == 1)
        {
            free_tokens(*tokens);
            free(tokens);
            return (NULL);
        }
        else if (result == 2)
            break ;
    }
    final_tokens = *tokens;
    free(tokens);
    return (final_tokens);
}
