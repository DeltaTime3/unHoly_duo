#include "minishell.h"

void free_tokens(t_token *tokens)
{
    t_token *current = tokens;
    t_token *next;
    while (current)
    {
        next = current->next;
        if (current->value)
            free(current->value);
        if (current->token)
            free(current->token);
        if (current->command)
            free(current->command);
        if (current->args)
            free_args(current->args);
        if (current->content)
            free(current->content);
        free(current);
        current = next;
    }
}

void	free_args(char **args)
{
	int i = 0;
    if (!args)
        return;
    while (args[i])
        free(args[i++]);
    free(args);
}

// quotes 
int	quote_handling(const char *input, int *i, t_token **tokens,
	int *expect_command)
{
	char    quote;
    size_t  start;
    char   *raw;
    char  **words;
    int     j;
    t_cat   type;

    quote = input[(*i)++];                     // skip opening quote
    start = *i;
    while (input[*i] && input[*i] != quote)
        (*i)++;
    if (input[*i] != quote)
        return (handle_quote_error(tokens));
    raw = ft_substr(input, start, *i - start); // raw = inner text
    if (!raw)
        return (handle_quote_error(tokens));
    (*i)++;                                    // skip closing quote
    // split the inner text on spaces
    words = ft_split(raw, ' ');
    free(raw);
    if (!words)
        return (handle_quote_error(tokens));
    // for each word, determine its type and add a token
    j = 0;
    while (words[j])
    {
        type = determine_token_type(words[j], expect_command);
        add_token(tokens, create_token(type, words[j]));
        free(words[j++]);
    }
    free(words);
    return 0;
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

static int is_invalid_sequence(const char *input, int i)
{
	if (!input[i] || !input[i+1] || !input[i+2])
		return (0);
    if (input[i + 1] && input[i + 2]
     && (  (input[i] == '|' && input[i + 1] == '|')
        || (input[i] == '|' && input[i + 2] == '|')
        || (input[i] == '<' && input[i + 1] == '<')
        || (input[i] == '<' && input[i + 2] == '<')
        || (input[i] == '>' && input[i + 1] == '>')
        || (input[i] == '>' && input[i + 2] == '>') ) )
        return (1);
    return (0);
}

int	check_token_sequence(const char *input)
{
	int	i;

	i = 0;
	while (input[i])
	{
		while (input[i] && ft_isspace(input[i]))
			i++;
		if (input[i])
			break ;
		if (is_invalid_sequence(input, i))
        {
            ft_printf_fd(2, UNEXPECTED_TOKEN);
            return (1);
        }
		if ((input[i] == '|' || input[i] == '<' || input[i] == '>') &&
            (i == 0 || input[i + 1] == '\0'))
        {
            ft_printf_fd(2, UNEXPECTED_TOKEN);
            return (1);
        }
		i++;
	}
	return (0);
}
