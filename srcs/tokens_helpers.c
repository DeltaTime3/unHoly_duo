
#include "minishell.h"

// quotes 
int quote_handling(const char *input, size_t *i, t_list **tokens)
{
    char    quote;
    size_t  start;
    char    *value;
    int     unmatched_quote;

    unmatched_quote = 0;
    quote = input[(*i)++];
    start = *i;
    while(input[*i] && input[*i] != quote)
        (*i)++;
    if (input[*i] != quote)
    {
        unmatched_quote = 1;
        return (1);
    }
    value = ft_substr(input, start, *i - start);
    if (!value)
        return (1);
    ft_lstadd_back(tokens, ft_lstnew(create_token(ARGUMENT, value)));
    (*i)++;
    return (0);
}
// operators
int op_handling(const char *input, size_t *i, t_list **tokens)
{
    size_t  start;
    char    *value;

    start = *i;
    if (input[*i] == '>' && input[*i + 1] == '>')
        (*i)++;
    value = ft_substr(input, start, *i - start + 1);
    if (!value)
        return (1);
    ft_lstadd_back(tokens, ft_lstnew(create_token(OPERATOR, value)));
    (*i)++;
    return (0);
}

int word_handling(const char *input, size_t *i, t_list **tokens,
    int *expect_command)
{
    size_t      start;
    char        *value;
    t_cat    type;

    start = i;
    while(input[*i] && !ft_isspace(input[*i]) && input[*i] != '\'' &&
                input[*i] != '"' && input[*i] != '#' && input[*i] != '<' &&
                    input[*i] != '>')
                    (*i)++;
    value = ft_substr(input, start, *i - start);
    if (!value)
        return (1);
    if (*expect_command)
        type = COMMAND;
    else
        type = ARGUMENT;
    *expect_command = 0;
    ft_lstadd_back(tokens, ft_lstnew(create_token(type, value)));
    return (0);
}