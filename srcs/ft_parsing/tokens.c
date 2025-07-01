#include "minishell.h"

t_token	*create_token(t_cat type, char *value)
{
	t_token	*token;

    token = ft_calloc(1, sizeof(t_token));
    if (!token)
        return (NULL);
    token->type = type;
    token->value = ft_strdup(value);
    token->content = NULL;
    token->args = NULL;
    token->next = NULL;
    token->command = NULL;
    return (token);
}

t_token	*tokenize_input(const char *input)
{
	t_token	**tokens;
	t_token	*final_tokens;
	int		i;
	int		expect_command;
	int		result;

	tokens = malloc(sizeof(t_token *));
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
			free(tokens);
			return (NULL);
		}
		else if (result == 2)
            break;
	}
	final_tokens = *tokens;
	free(tokens);
	print_tokens(final_tokens);
	prep_cmd_args(final_tokens);
	return (final_tokens);
}

void prep_cmd_args(t_token *head)
{
    t_token *current = head;
    
    while (current)
    {
        if (current->type == COMMAND)
        {
            // Count arguments for this command until we hit a pipe or end
            int arg_count = 1; // Start with 1 for the command itself
            t_token *temp = current->next;
            
            while (temp && temp->type != PIPE)
            {
                if (temp->type == ARGUMENT || temp->type == FLAG)
                    arg_count++;
                temp = temp->next;
            }
            
            // Allocate args array
            current->args = ft_calloc(arg_count + 1, sizeof(char *));
            if (!current->args)
                return;
            
            // Fill the args array
            current->args[0] = ft_strdup(current->value);
            int i = 1;
            temp = current->next;
            
            while (temp && temp->type != PIPE && i < arg_count)
            {
                if (temp->type == ARGUMENT || temp->type == FLAG)
                {
                    current->args[i] = ft_strdup(temp->value);
                    i++;
                }
                temp = temp->next;
            }
            current->args[i] = NULL; // Null terminate
        }
        current = current->next;
    }
}
