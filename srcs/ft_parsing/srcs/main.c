/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppaula-d <ppaula-d@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 11:39:22 by ppaula-d          #+#    #+#             */
/*   Updated: 2025/05/13 13:31:12 by ppaula-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int main(int ac, char **av, char **envp)
{
    char    *input;
    t_list  *tokens;
    
    (void)ac;
    (void)av;
    (void)envp;

    tokens = NULL;
    input = NULL;
    while(1)
        input_handling(input, tokens);
    rl_clear_history();
    return(0);
}
