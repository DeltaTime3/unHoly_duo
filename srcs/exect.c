
#include "../include/minishell.h"

void	choose_b_in(t_token *token, t_shell *shell)
{
    if (!token || !token->value)
	{
        return;
	}
	if (token->value[0] == '\0')
	{
        return;
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

// In exect.c
int	ft_execute(t_shell *shell, t_token *value)
{
    int saved_stdout;
    int saved_stdin;
    t_token *cmd;

    saved_stdout = dup(STDOUT_FILENO);
    saved_stdin = dup(STDIN_FILENO);
    if (saved_stdout == -1 || saved_stdin == -1)
    {
        shell->exit_code = 1;
        return (1);
    }
    expand_tokens(value, shell);
    prep_cmd_args(value); // This call will now correctly filter arguments
	if (apply_lonely_redirs(value, shell) == -1)
	{
		dup2(saved_stdout, STDOUT_FILENO);
		dup2(saved_stdin, STDIN_FILENO);
		close(saved_stdout);
		close(saved_stdin);
		return (1);
	}
    // Find the first command token that is NOT an empty-expanded token
    cmd = value;
    while (cmd && (cmd->type != COMMAND || (cmd->value && cmd->value[0] == '\0' && cmd->was_expanded))) // MODIFIED CONDITION
        cmd = cmd->next;
    
    // If we have a command token, check if it's empty
    if (cmd)
    {
        // The prep_cmd_args should have already filtered out empty-expanded commands.
        // So, if cmd->value is empty here, it means it was originally ""
        if (!cmd->value || cmd->value[0] == '\0') // This now only catches original "" commands
        {
            // For "", try to execute and fail
            ft_printf_fd(2, "minishell: : command not found\n");
            shell->exit_code = 127;
            dup2(saved_stdout, STDOUT_FILENO);
            dup2(saved_stdin, STDIN_FILENO);
            close(saved_stdout);
            close(saved_stdin);
            return (127);
        }
    }
    
    // If no valid command found at all (i.e., all commands were empty-expanded or no commands)
    if (!cmd || !cmd->value || cmd->value[0] == '\0') // This condition should now primarily catch cases where all commands were empty-expanded
    {
        dup2(saved_stdout, STDOUT_FILENO);
        dup2(saved_stdin, STDIN_FILENO);
        close(saved_stdout);
        close(saved_stdin);
        shell->exit_code = 0; // Bash returns 0 for "$UNSET_VAR"
        return (0);
    }

    // ... rest of ft_execute remains the same ...
    // Special cases for . and ..
    if (cmd && cmd->value && ft_strcmp(cmd->value, ".") == 0) 
    {
        ft_printf_fd(2, "minishell: .: filename argument required\n");
        ft_printf_fd(2, ".: usage: . filename [arguments]\n");
        shell->exit_code = 2;
        dup2(saved_stdout, STDOUT_FILENO);
        dup2(saved_stdin, STDIN_FILENO);
        close(saved_stdout);
        close(saved_stdin);
        return (2);
    }
    if (cmd && cmd->value && ft_strcmp(cmd->value, "..") == 0) 
    {
        ft_printf_fd(2, "minishell: ..: command not found\n");
        shell->exit_code = 127;
        dup2(saved_stdout, STDOUT_FILENO);
        dup2(saved_stdin, STDIN_FILENO);
        close(saved_stdout);
        close(saved_stdin);
        return (127);
    }

    // Handle pipelines
    if (count_pipes(value) > 0)
    {
        handle_pipes(value, shell);
        dup2(saved_stdout, STDOUT_FILENO);
        dup2(saved_stdin, STDIN_FILENO);
        close(saved_stdout);
        close(saved_stdin);
        return (shell->exit_code);
    }

    // Handle redirections
    if (redirect_handling(value, shell) == -1)
    {
        dup2(saved_stdout, STDOUT_FILENO);
        dup2(saved_stdin, STDIN_FILENO);
        close(saved_stdout);
        close(saved_stdin);
        return (shell->exit_code);
    }

    // Builtin or external command
    if (is_builtin(cmd))
        choose_b_in(cmd, shell);
    else
        execute2(shell, cmd);

    dup2(saved_stdout, STDOUT_FILENO);
    dup2(saved_stdin, STDIN_FILENO);
    close(saved_stdout);
    close(saved_stdin);
    return (shell->exit_code);
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

	sts = 0;
    if (!token || !token->value)
    {
        return 0;
    }
	if (token->value[0] == '\0')
    {
        ft_printf_fd(2, "minishell: %s: command not found\n", token->value);
        shell->exit_code = 127;
        return (127);
    }
    full_path = get_cmd_path(token->value, shell->head);
    if (!full_path)
    {
        if (ft_strchr(token->value, '/'))
        {
            ft_printf_fd(2, "minishell: %s: No such file or directory\n", token->value);
            shell->exit_code = 127;
        }
        else
        {
            ft_printf_fd(2, "minishell: %s: command not found\n", token->value);
            shell->exit_code = 127;
        }
        return (127);
    }
	if (full_path && ft_strcmp(full_path, ":permission_denied:") == 0)
    {
        ft_printf_fd(2, "minishell: %s: Permission denied\n", token->value);
        shell->exit_code = 126;
        free(full_path);
        return (126);
    }
	env_array = env_list_to_array(shell->head);
	if (!env_array)
	{
		free(full_path);
		return (-1);
	}
	pid = fork();
	if (pid == 0)
		return(pid_zero(full_path, env_array, token), 0);
	else if (pid < 0)
		return (pid_neg(full_path, env_array), -1);
	else
		return (pid_else(full_path, env_array, shell, pid, sts));
}

void	pid_zero(char *full_path, char **env_array, t_token *token)
{
    signal(SIGINT, SIG_DFL);
    signal(SIGQUIT, SIG_DFL);
    signal(SIGPIPE, handle_sig_pipe);
    execve(full_path, token->args, env_array);
    int err = errno;
    free_env_array(env_array);
    if (err == EACCES || err == EISDIR)
    {
        ft_printf_fd(2, " Is a directory\n");
        exit (126);
    }
    else if (err == ENOENT)
    {
        if (ft_strchr(token->value, '/'))
            ft_printf_fd(2, "minishell: No such file or directory\n");
        else
            ft_printf_fd(2, "minishell: command not found\n");
        exit (127);
    }
    else
    {
        perror(token->value);
        exit (1);
    }
}

int	pid_neg(char *full_path, char **env_array)
{
	perror("fork");
	free(full_path);
	free_env_array(env_array);
	return (-1);
}

int	pid_else(char *full_path, char **env_array, t_shell *shell, pid_t pid, int sts)
{
	wait(&sts);
	waitpid(pid, &sts, 0);
	shell->exit_code = WEXITSTATUS(sts);
	free(full_path);
	free_env_array(env_array);
	return (0);
}

char	*get_cmd_path(char *cmd, t_env *env)
{
	char	*path;
	char	*pth_cpy;
	char	*res;

	if (!cmd || !*cmd)
		return (NULL);
	if (ft_strchr(cmd, '/'))
	{
		if (access(cmd, F_OK) == 0)
		{
			if (access(cmd, X_OK) == 0)
				return ft_strdup(cmd);
			else
				return ft_strdup(":permission_denied:");
		}
		return (NULL);
	}
	path = get_env_value(env, "PATH");
	if (!path || path[0] == '\0')
	{
		char *fallback_path = fallback(cmd);
		if (fallback_path)
			return fallback_path;
		if (access(cmd, X_OK) == 0)
			return ft_strdup(cmd);
		
		return NULL;
	}
	pth_cpy = ft_strdup(path);
	if (!pth_cpy)
		return (NULL);
	res = check_path_dir(pth_cpy, cmd);
	free(pth_cpy);
	return res;
}


char	*check_path_dir(char *pth_cpy, char *cmd)
{
	char	*dir;
	char	*full_path;
	char	*res;
	
	res = NULL;
	while ((dir = get_next_path(&pth_cpy)))
	{
		full_path = build_cmb_path(dir, cmd);
		free(dir);
		if (full_path && access(full_path, X_OK) == 0)
		{
			res = full_path;
			break;
		}
		free(full_path);
	}
	return (res);
}

char	*get_next_path(char **path_temp)
{
	char	*start;
	char	*end;
	char	*dir;
	int		len;

	start = *path_temp;
	if(!start || !*start)
		return (NULL);
	end = ft_strchr(start, ':');
	if (end != NULL)
		len = end - start;
	else
		len = ft_strlen(start);
	dir = malloc(len + 1);
	if (!dir)
		return (NULL);
	ft_strlcpy(dir, start, len + 1);
	dir[len] = '\0';
	if (end)
		*path_temp = end + 1;
	else
		*path_temp = start + len;
	return (dir);
}

char	*build_cmb_path(char *dir, char *cmd)
{
	char	*full_path;
	size_t	dir_len;
	size_t	cmd_len;
	size_t	total_len;

	dir_len = ft_strlen(dir);
	cmd_len = ft_strlen(cmd);
	if (!dir[0])
		total_len = cmd_len + 3;
	else
		total_len = dir_len + cmd_len + 2;
	full_path = ft_calloc(1, total_len);
	if(!full_path)
		return (NULL);
	if(!dir[0])
		ft_strlcpy(full_path, "./", 3);
	else
	{
		ft_strlcpy(full_path, dir, total_len);
		ft_strlcat(full_path, "/", total_len);
	}
	ft_strlcat(full_path, cmd, total_len);
	return(full_path);	
}
