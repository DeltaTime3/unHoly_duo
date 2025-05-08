/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppaula-d <ppaula-d@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 11:39:22 by ppaula-d          #+#    #+#             */
/*   Updated: 2025/05/08 14:54:23 by ppaula-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
    else if (type == COMMENT)
        return "COMMENT";
    else if (type == PIPE)
        return "PIPE";
    else
        return "UNKNOWN";
}

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

int main(int ac, char **av, char **envp)
{
    char    *input;
    t_list  *tokens;
    
    (void)ac;
    (void)av;
    (void)envp;

    while(1)
    {
        input = readline("minishell> ");
        if (!input)
        {
            printf("exit\n");
            break;
        }
        if (ft_strcmp(input, "exit") == 0)
        {
            printf("exit\n");
            free(input);
            rl_clear_history();
            exit(0);
        }
        if (*input)
            add_history(input);
        tokens = tokenize_input(input);
        if (!tokens && *input != '#')
            return (1);
        else
        {
            print_tokens(tokens);
            free_tokens(tokens);
        }
    }
    rl_clear_history();
    return(0);
}