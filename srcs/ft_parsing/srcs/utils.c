
#include "minishell.h"

int validate_input(const char *input)
{
    int     i;
    char    quote;

    i = 0;
    quote = '\0';
    if (*input)
			add_history(input);
    while (input[i] && ft_isspace(input[i]))
        i++;
    if (input[i] == '\n' || ft_isspace(input[i]))
        printf("\n");
    i = 0;
    while (input[i])
    {
        if (input[i] == '\'' || input[i] == '"')
        {
            if (quote == '\0')
                quote = input[i];
            else if (quote == input[i])
                quote = '\0';
        }
        i++;
    }
    if (quote != '\0')
    {
        ft_printf_fd(UNMATCHED_QUOTES, 2);
        return (1);
    }
    i = 0;
    while (input[i])
    {
        if ((input[i] == '&' && input[i + 1] == '&') ||
            (input[i] == '|' && input[i + 1] == '|') ||
            (input[i] == '!'))
        {
            ft_printf_fd(LOGICAL_OPERATORS, 2);
            return (1);
        }
        i++;
    }
    i = ft_strlen(input);
    if (i == 0)
        return (0);
    i--;
    while (i >= 0 && ft_isspace(input[i]))
    {
        i--;
        return (0);
    }
    if (input[i] == '<' || input[i] == '>' || input[i] == '&')
    {
        ft_printf_fd(UNEXPECTED_TOKEN, 2);
        return (1);
    }
    return (0);
}

void    free_tokens(t_list *tokens)
{
    t_list  *current;
    t_list  *next;
    t_token *token;

    current = tokens;
    while (current)
    {
        next = current->next;
        token = (t_token *)current->content;
        if (token)
        {
            free(token->content);
            free(token);
        }
        free(current);
        current = next;
    }
}
