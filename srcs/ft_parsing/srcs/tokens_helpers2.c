/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens_helpers2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppaula-d <ppaula-d@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 13:38:52 by ppaula-d          #+#    #+#             */
/*   Updated: 2025/05/08 09:59:35 by ppaula-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int syntax_err_handling(char unexpected_char)
{
    printf("minishell: syntax error near unexpected token `%c'", unexpected_char);
    return(1);
}

int syntax_err_handling_nl(void)
{
    printf("minishell: syntax error near unexpected token `newline'");
    return(1);
}

