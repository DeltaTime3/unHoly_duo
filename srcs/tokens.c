
#include "minishell.h"

t_token *create_token(t_cat type, char *content)
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
    int     expect_command;
    int     result;

    tokens = NULL;
    i = 0;
    while (input[i])
    {
        while (input[i] && ft_isspace(input[i]))
            i++;
        if (!input[i])
            break;
        result = token_handling(input, &i, &tokens, &expect_command);
        if (result == 1)
            return (NULL);
        else if (result == 2)
            break;
    }
    return (tokens);
}
// DON'T FORGET TO FREE THE TOKEN CONTENT, THE TOKEN ITSELF AND THE LIST NODE







// Helper function to print token types
static const char *get_token_type(t_cat type)
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