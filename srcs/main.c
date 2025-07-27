/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppaula-d <ppaula-d@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 13:59:12 by ppaula-d          #+#    #+#             */
/*   Updated: 2025/07/27 14:39:08 by ppaula-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	g_global_sig;

int	main(int ac, char **av, char **envp)
{
	t_shell	shell;
	char	**orig_envp;

	(void)ac;
	(void)av;
	orig_envp = envp;
	init_shell_struct(&shell, orig_envp);
	ft_signals();
	main_loop(&shell);
	cleanup_shell(&shell);
	return (shell.exit_code);
}
