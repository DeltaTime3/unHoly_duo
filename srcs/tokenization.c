
#include "minishell.h"

t_token *create_token(t_tokens type, char *content)
{
    t_token *token;
    
    token = malloc(sizeof(t_token));
    if (!token)
        return (NULL);
    token->type = type;
    token->content = content;
    return (token);
}

t_list  *tokenize_input(const char *input)
{
    t_list  *tokens;
    size_t  i;
    char    quote;
    size_t  start;
    char    *value;
    int     unmatched_quote;
    int     expect_command;
    t_tokens    type;

    tokens = NULL;
    i = 0;
    unmatched_quote = 0;
    expect_command = 1;
    while (input[i])
    {
        while (input[i] && ft_isspace(input[i]))
            i++;
        if (!input[i])
            break;
        if (input[i] == '\'' || input[i] == '"')
        {
            quote = input[i++];
            start = i;
            while(input[i] && input[i] != quote)
                i++;
            if (input[i] != quote)
            {
               unmatched_quote = 1;
               // printf("Error: unmatched quote\n");
               break;
            }
            value = ft_substr(input, start, i - start);
            if (!value)
                return (NULL);
            ft_lstadd_back(&tokens, ft_lstnew(create_token(ARGUMENT, value)));
            i++; // skip the closing quote
        }
        else if (input[i] == '#')
            break;
        else if (input[i] == '|' || input[i] == '<' || input[i] == '>')
        {
            start = i;
            if((input[i] == '>' && input[i + 1] == '>'))
                i++;
            value = ft_substr(input, start, i - start + 1);
            if (!value)
                return (NULL);
            ft_lstadd_back(&tokens, ft_lstnew(create_token(OPERATOR, value)));
            i++;
            expect_command = 1;
        }
        else
        {
            start = i;
            while(input[i] && !ft_isspace(input[i]) && input[i] != '\'' &&
                input[i] != '"' && input[i] != '#' && input[i] != '<' &&
                    input[i] != '>')
                    i++;
            value = ft_substr(input, start, i - start);
            if (!value)
                return (NULL);
            if (expect_command)
                type = COMMAND;
            else
                type = ARGUMENT;
            expect_command = 0;
            ft_lstadd_back(&tokens, ft_lstnew(create_token(type, value)));
        }
    }
    return (tokens);
}
// DON'T FORGET TO FREE THE TOKEN CONTENT, THE TOKEN ITSELF AND THE LIST NODE

// Helper function to print token types
static const char *get_token_type(t_tokens type)
{
    if (type == COMMAND)
        return "COMMAND";
    else if (type == ARGUMENT)
        return "ARGUMENT";
    else if (type == OPERATOR)
        return "OPERATOR";
    else if (type == QUOTE)
        return "QUOTE";
    else
        return "UNKNOWN";
}

// Function to print the tokens
static void print_tokens(t_list *tokens)
{
    t_list *current = tokens;
    while (current)
    {
        t_token *token = (t_token *)current->content;
        printf("Type: %s, Value: '%s'\n", get_token_type(token->type), token->content);
        current = current->next;
    }
}

int main(void)
{
    const char *input = "cat file.txt | grep \"hello world\" > output.txt # This is a comment";
    t_list *tokens = tokenize_input(input);

    if (!tokens)
    {
        printf("Error: Tokenization failed\n");
        return (1);
    }
    printf("Input: %s\n", input);
    printf("Tokens:\n");
    print_tokens(tokens);

    // Free the tokens (optional, but recommended)
    t_list *current = tokens;
    while (current)
    {
        t_list *next = current->next;
        t_token *token = (t_token *)current->content;
        free(token->content); // Free the token content
        free(token);          // Free the token itself
        free(current);        // Free the list node
        current = next;
    }

    return (0);
}