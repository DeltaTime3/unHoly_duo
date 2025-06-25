#include "minishell.h"

t_token	*create_token(t_cat type, char *content)
{
	t_token	*token;

    token = malloc(sizeof(t_token));
    if (!token)
        return (NULL);
    token->type = type;
    token->content = content;
    if (content)
        token->content= ft_strdup(content);
    else
        token->content= NULL;
    if (content)
        token->value= ft_strdup(content);
    else
        token->value= NULL;
    token->args = NULL;
    token->next = NULL;
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
            free_tokens(*tokens);
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

void	prep_cmd_args(t_token *head)
{
	t_token	*command;
	int		arg_count;
	t_token	*arg;
	int		i;

	command = head;
	while (command)
	{
		if (command->type == COMMAND)
		{
			arg_count = 1;
            arg = command->next;
            while (arg && arg->type != PIPE && arg->type != COMMAND)
            {
                if (arg->type == ARGUMENT || arg->type == FLAG)
                    arg_count++;
                arg = arg->next;
            }
            command->args = malloc(sizeof(char *) * (arg_count + 1));
            if (!command->args)
                return;
            command->args[0] = ft_strdup(command->value);
            i = 1;
            arg = command->next;
            while (arg && arg->type != PIPE && arg->type != COMMAND && i < arg_count)
            {
                if (arg->type == ARGUMENT || arg->type == FLAG)
                {
                    command->args[i] = ft_strdup(arg->value);
                    if (!command->args[i])
                    {
                        while (--i >= 0)
                            free (command->args[i]);
                        free(command->args);
                        return ;
                    }
                    i++;
                }
                arg = arg->next;
            }
            command->args[i] = NULL;
			arg = command->next;
            while (arg && arg->type != PIPE)
                arg = arg->next;
			if (arg && arg->type == PIPE)
                command = arg->next;
            else
                command = NULL;
        }
        else
            command = command->next;
	}
}
