
#include "minishell.h"

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
