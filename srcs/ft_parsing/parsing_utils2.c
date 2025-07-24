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
#include "minishell.h"

// ... (other functions) ...

// quotes 
int quote_handling(const char *input, int *i, t_token **tokens, int *expect_command)
{
    char    quote;
    size_t  start_content; // Start of content inside quotes
    size_t  end_quote_pos; // Position of the closing quote
    char    *quoted_content;
    char    *full_word_content; // To hold the combined quoted + unquoted part
    t_cat   type;
    t_token *new_token;

    quote = input[*i];
    (*i)++; // Move past opening quote
    start_content = *i;

    // Find the closing quote
    while (input[*i] && input[*i] != quote)
        (*i)++;
    
    if (input[*i] != quote) // Unmatched quote
        return handle_quote_error(tokens);
    
    end_quote_pos = *i; // Position of the closing quote
    quoted_content = ft_substr(input, start_content, end_quote_pos - start_content);
    if (!quoted_content)
        return handle_quote_error(tokens);
    
    (*i)++; // Move past closing quote

    // Now, consume any immediately following unquoted characters that are part of the same word
    size_t start_unquoted = *i;
    while (input[*i] && !ft_isspace(input[*i]) && input[*i] != '|' && input[*i] != '<' && input[*i] != '>' && input[*i] != '\'' && input[*i] != '"')
        (*i)++;
    
    char *unquoted_suffix = ft_substr(input, start_unquoted, *i - start_unquoted);
    if (!unquoted_suffix)
    {
        free(quoted_content);
        return handle_quote_error(tokens);
    }

    // Combine the quoted content and the unquoted suffix
    full_word_content = ft_strjoin(quoted_content, unquoted_suffix);
    free(quoted_content);
    free(unquoted_suffix);
    if (!full_word_content)
        return handle_quote_error(tokens);

    type = determine_token_type(full_word_content, expect_command);
    
    new_token = create_token(type, full_word_content);
    
    // Set the in_single_quotes flag if this was a single-quoted string
    // This flag is for expansion, not for the tokenizer's internal logic
    if (quote == '\'')
        new_token->in_single_quotes = 1;
    
    add_token(tokens, new_token);
    free(full_word_content); // Free the combined string after creating the token
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
