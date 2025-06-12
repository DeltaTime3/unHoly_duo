/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exect.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppaula-d <ppaula-d@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 11:07:06 by afilipe-          #+#    #+#             */
/*   Updated: 2025/06/12 13:29:31 by ppaula-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	choose_b_in(t_token *token, t_shell *shell)
{
	if (ft_strcmp(token->value, "cd") == 0)
		ft_cd(token, shell);
	else if (ft_strcmp(token->value, "exit") == 0)
		ft_exit(&token, shell);
	else if (ft_strcmp(token->value, "export") == 0)
		ft_export(shell, token->args);
	else if (ft_strcmp(token->value, "env") == 0)
		ft_env(shell, token);
	else if (ft_strcmp(token->value, "echo") == 0)
		ft_echo(token, shell);
	else if (ft_strcmp(token->value, "pwd") == 0)
		ft_pwd(shell);
	else if (ft_strcmp(token->value, "unset") == 0)
		ft_unset(shell, token);
}

int	is_builtin(t_token *token)
{
	char	*builtin[8]; 
	int		i;

	*builtin = (char *[]){
		"cd",
		"exit",
		"export",
		"env",
		"echo",
		"pwd",
		"unset",
		NULL
	};
	i = 0;
	while (builtin[i])
	{
		if (ft_strcmp(token->value, builtin[i]) == 0)
			return (1);
		i++;
	}
	return (0);
}

int	ft_execute(t_shell *shell, t_token *value)
{
	if (is_builtin(value))
	{
		choose_b_in(value, shell);
		return (0);
	}
	else
	{
		execute2(shell, value);
		return (0);
	}
}
//here I have to use child processes;
//not sure if i need to go get the full PATH here and perform a NULL check
//if so it will somethimg like
//ft_fprintf(2, "minishell comand not found\n", token->value)
//return (127)
//will also need a get comand path function
int	execute2(t_shell *shell, t_token *token)
{
	pid_t	pid;
	int		sts;
	char	*full_path;
	char	**env_array;

	full_path = get_cmd_path(token->value, shell->head);
	if (!full_path)
	{
		ft_printf_fd(2, "minishell: %s: command not found\n", token->value);
		return (127);
	}
	env_array = env_list_to_array(shell->head);
	if (!env_array)
	{
		free(full_path);
		return (-1);
	}
	pid = fork();
	if (pid == 0)
	{
		execve(full_path, token->args, env_array);
		perror("execve");
		free(full_path);
		free(env_array);
		exit(EXIT_FAILURE);
	}
	else if (pid < 0)
	{
		perror("fork");
		free(full_path);
		free(env_array);
		return (-1);
	}
	else
	{
		waitpid(pid, &sts, 0);
		shell->exit_code = WEXITSTATUS(sts);
		free(full_path);
		free(env_array);
		return (0);
	}
}

char	*get_cmd_path(char *cmd, t_env *env)
{
	char		*path;
	char		*dir;
	char		*full_path;
	struct stat	st;

	if (!cmd || !*cmd)
		return (NULL);
	if (ft_strchr(cmd, '/'))
	{
		
	}
}