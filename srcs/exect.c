/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exect.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppaula-d <ppaula-d@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 13:59:12 by ppaula-d          #+#    #+#             */
/*   Updated: 2025/07/31 18:06:26 by ppaula-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	choose_b_in(t_token *token, t_shell *shell)
{
	if (!token || !token->value)
	{
		return ;
	}
	if (token->value[0] == '\0')
	{
		return ;
	}
	if (ft_strcmp(token->value, "cd") == 0)
		ft_cd(token, shell);
	else if (ft_strcmp(token->value, "exit") == 0)
		ft_exit(&token, shell);
	else if (ft_strcmp(token->value, "export") == 0)
		ft_export(shell, token->args);
	else if (ft_strcmp(token->value, "env") == 0)
		ft_env(shell, token);
	else if (ft_strcmp(token->value, "echo") == 0)
		ft_echo(token);
	else if (ft_strcmp(token->value, "pwd") == 0)
		ft_pwd(shell);
	else if (ft_strcmp(token->value, "unset") == 0)
		ft_unset(shell, token);
}

int	is_builtin(t_token *token)
{
	char	*builtin[8];
	int		i;

	builtin[0] = "cd";
	builtin[1] = "exit";
	builtin[2] = "export";
	builtin[3] = "env";
	builtin[4] = "echo";
	builtin[5] = "pwd";
	builtin[6] = "unset";
	builtin[7] = NULL;
	i = 0;
	while (builtin[i])
	{
		if (token->value && ft_strcmp(token->value, builtin[i]) == 0)
			return (1);
		i++;
	}
	return (0);
}

int	ft_execute(t_shell *shell, t_token *value)
{
	int		out;
	int		in;
	int		special;
	t_token	*cmd;

	shell->head_tokens = value;
	out = dup(STDOUT_FILENO);
	in = dup(STDIN_FILENO);
	if (out == -1 || in == -1)
		return (shell->exit_code = 1, 1);
	expand_tokens(value, shell);
	prep_cmd_args(value);
	if (count_pipes(value) > 0)
		return (handle_pipes_n_restore(value, shell, out, in));
	cmd = find_valid_cmd(value);
	if (cmd && cmd->value && cmd->value[0] == '\0' && !has_pipe_after(cmd))
	{
		restore_fds(out, in);
		return (print_cmd_not_found(shell, cmd));
	}
	while (cmd && cmd->value && cmd->value[0] == '\0')
		cmd = find_valid_cmd(cmd->next);
	special = handle_special_cmds(shell, cmd, out, in);
	if (special != -1)
		return (special);
	if (handle_red_n_restore(value, shell, out, in))
		return (shell->exit_code);
	exec_cmd_or_b_in(shell, cmd);
	restore_fds(out, in);
	return (shell->exit_code);
}

int	execute2(t_shell *shell, t_token *token)
{
	char	*full_path;
	int		err;
	char	**env_array;
	pid_t	pid;
	int		ret;

	full_path = get_cmd_path(token->value, shell->head);
	err = handle_exe2_err(shell, token, full_path);
	if (err != -1)
		return (err);
	env_array = env_list_to_array(shell->head);
	if (!env_array)
		return (free(full_path), -1);
	pid = fork();
	if (pid == 0)
		return (pid_zero(full_path, env_array, token, shell), 0);
	if (pid < 0)
		return (pid_neg(full_path, env_array), -1);
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	ret = pid_else(full_path, env_array, shell, pid);
	ft_signals();
	return (ret);
}

char	*get_cmd_path(char *cmd, t_env *env)
{
	char	*path;
	char	*pth_cpy;
	char	*res;

	if (!cmd || !*cmd)
		return (NULL);
	if (ft_strchr(cmd, '/'))
		return (handle_cmd_slash(cmd));
	path = get_env_value(env, "PATH");
	if (!path || path[0] == '\0')
		return (handle_no_path(cmd));
	pth_cpy = ft_strdup(path);
	if (!pth_cpy)
		return (NULL);
	res = check_path_dir(pth_cpy, cmd);
	free(pth_cpy);
	return (res);
}
