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

// int validate_syntax(t_list *tokens)
// {
//     t_token *current;
//     t_token *next;

//     while (tokens)
//     {
//         current = (t_token *)tokens->content;

//         // Get the next token if it exists
//         if (tokens->next)
//             next = (t_token *)tokens->next->content;
//         else
//             next = NULL;

//         // Check for invalid sequences
//         if (current->type == OPERATOR)
//         {
//             // Case 1: Operator at the end of the input
//             if (!next)
//                 return syntax_err_handling_nl(current->content[0]);

//             // Case 2: Invalid operator combinations
//             if (next->type == OPERATOR)
//             {
//                 if (ft_strcmp(next->content, "|") == 0 || ft_strcmp(next->content, "||") == 0)
//                     return syntax_err_handling('|');
//                 else if (ft_strcmp(next->content, "<") == 0 || ft_strcmp(next->content, "<<") == 0)
//                     return syntax_err_handling('<');
//                 else if (ft_strcmp(next->content, ">") == 0 || ft_strcmp(next->content, ">>") == 0)
//                     return syntax_err_handling('>');
//                 else if (ft_strcmp(next->content, "&") == 0 || ft_strcmp(next->content, "&&") == 0)
//                     return syntax_err_handling('&');
//                 else
//                     return syntax_err_handling(next->content[0]);
//             }
//         }

//         tokens = tokens->next;
//     }
//     return 0; // No syntax errors
// }

// t_list *tokenize_and_validate(const char *input)
// {
//     t_list *tokens;

//     tokens = tokenize_input(input);
//     if (!tokens)
//     {
//         ft_error("Tokenization failed");
//         return NULL;
//     }

//     if (validate_syntax(tokens))
//     {
//         ft_lstclear(&tokens, free);
//         return NULL;
//     }

//     return tokens;
// }