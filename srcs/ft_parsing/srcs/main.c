/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppaula-d <ppaula-d@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 11:39:22 by ppaula-d          #+#    #+#             */
/*   Updated: 2025/05/08 10:10:50 by ppaula-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int main(int ac, char **av, char **envp)
{
    char    *input;
    
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
        free(input);
    }
    rl_clear_history();
    return(0);
}