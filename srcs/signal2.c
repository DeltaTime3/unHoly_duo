/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppaula-d <ppaula-d@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 13:59:12 by ppaula-d          #+#    #+#             */
/*   Updated: 2025/07/30 15:50:32 by ppaula-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_sigint_execution(int sig)
{
	if (sig == SIGINT)
	{
		g_global_sig = 130;
		write(STDOUT_FILENO, "\n", 1);
	}
}

void	set_execution_signals(void)
{
	signal(SIGINT, handle_sigint_execution);
	signal(SIGQUIT, handle_sigint_execution);
}
