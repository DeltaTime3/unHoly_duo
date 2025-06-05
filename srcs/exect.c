/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exect.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afilipe- <afilipe-@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 11:07:06 by afilipe-          #+#    #+#             */
/*   Updated: 2025/06/05 16:34:04 by afilipe-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	choose_b_in(t_token *token, t_shell *shell)
{
	if (ft_strcmp(token->command, "cd") == 0)
		ft_cd(token->command, shell);
	else if (ft_strcmp(token->command, "exit") == 0)
		ft_exit(&token->command, shell);
	else if (ft_strcmp(token->command, "export") == 0)
		ft_export(shell, token->args);
	else if (ft_strcmp(token->command, "env") == 0)
		ft_env(shell, token->command);
	else if (ft_strcmp(token->command, "echo") == 0)
		ft_echo(token->command, shell);
	else if (ft_strcmp(token->command, "pwd") == 0)
		ft_pwd(shell);
	else if (ft_strcmp(token->command, "unset") == 0)
		ft_unset(shell, token->command);
}

int	is_builtin(t_token *token)
{
	char	*builtin[7];
	int		i;

	*builtin = (char *[]){
		"cd",
		"exit",
		"export",
		"env",
		"echo",
		"pwd",
		"unset"
	};
	i = 0;
	while (builtin[i])
	{
		if (ft_strcmp(token->command, builtin[i]) == 0)
			return (1);
		i++;
	}
	return (0);
}

int	ft_execute(t_shell *shell, t_token *command)
{
	if (is_builtin(command))
	{
		choose_b_in(command, shell);
		return (0);
	}
	else
	{
		execute2(shell, command);
		return (0);
	}
}
//here I have to use child processes;
int	execute2(t_shell *shell, t_token *token)
{
	pid_t	pid;
	int		sts;

	pid = fork();
	if (pid == 0)
	{
		execve(token->command, token->args, shell->env_var);
		perror("execve");
		exit(EXIT_FAILURE);
	}
	else if (pid < 0)
	{
		perror("fork");
		return (-1);
	}
	else
	{
		waitpid(pid, &sts, 0);
		shell->exit_code = WEXITSTATUS(sts);
		return (0);
	}
}
