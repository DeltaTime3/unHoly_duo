/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstadd_back.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppaula-d <ppaula-d@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 11:44:33 by ppaula-d          #+#    #+#             */
/*   Updated: 2025/04/15 11:47:46 by ppaula-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void    ft_lstadd_back(t_list **lst, t_list *new)
{
    t_list  *last;

    if (!lst || !new)
        return;
    if (*lst == NULL)
    {
        *lst = new;
        return;
    }
    last = *lst;
    while(last->next)
        last = last->next;
    last->next = new;
}
