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
    token->expand_heredoc = 0;
    token->was_expanded = 0;
    token->in_single_quotes = 0;
    return (token);
}

static int  is_all_ws(const char *input)
{
    int i;

    i = 0;
    while(input[i])
    {
        if (!ft_isspace(input[i]))
            return (0);
        i++;
    }
    return (1);
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
	if (!input || ft_strlen(input) == 0 || is_all_ws(input))
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
	prep_cmd_args(final_tokens);
	return (final_tokens);
}

void prep_cmd_args(t_token *head) {
    t_token *current = head;
    while (current) {
        if (current->type == COMMAND && current->value && current->value[0] != '\0' && !current->was_expanded) {
            // Free old args if they exist
            if (current->args)
                free_args(current->args);
            
            // Count valid arguments (skip empty-expanded tokens)
            int arg_count = 1; // For the command itself
            t_token *temp = current->next;
            while (temp && temp->type != PIPE) {
                if ((temp->type == ARGUMENT || temp->type == FLAG) && 
                    temp->value && !(temp->value[0] == '\0' && temp->was_expanded))
                    arg_count++;
                temp = temp->next;
            }
            
            // Allocate args (arg_count + 1 for NULL terminator)
            current->args = (char **)ft_calloc(arg_count + 1, sizeof(char *));
            if (!current->args) return;
            
            // Copy command name
            current->args[0] = ft_strdup(current->value); // "grep"
            
            // Copy arguments (NO SPLITTING! Preserve token->value)
            int i = 1;
            temp = current->next;
            while (temp && temp->type != PIPE && i < arg_count) {
                if ((temp->type == ARGUMENT || temp->type == FLAG) && 
                    temp->value && !(temp->value[0] == '\0' && temp->was_expanded)) {
                    current->args[i] = ft_strdup(temp->value); // ");$" (single string)
                    i++;
                }
                temp = temp->next;
            }
        }
        current = current->next;
    }
}
