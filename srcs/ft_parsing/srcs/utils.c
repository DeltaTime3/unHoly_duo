/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppaula-d <ppaula-d@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 12:03:04 by ppaula-d          #+#    #+#             */
/*   Updated: 2025/05/08 13:56:34 by ppaula-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
