
#include "../include/minishell.h"

void	choose_b_in(t_token *token, t_shell *shell)
{
	if (!token || !token->value)
        return;
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
		return (shell->exit_code);
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
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
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
	char		*pth_cpy;
	char		*res;

	if (!cmd || !*cmd)
		return (NULL);
	if (ft_strchr(cmd, '/'))
	{
		if (access(cmd, X_OK) == 0)
			return ft_strdup(cmd);
		return (NULL);
	}
	path = get_env_value(env, "PATH");
	pth_cpy = path;
	if (!path || path[0] == '\0')
	{
		if (pth_cpy)
			free(pth_cpy);
		return (NULL);
	}		
	pth_cpy = ft_strdup(path);
	if (!pth_cpy)
		return (NULL);
	res = check_path_dir(pth_cpy, cmd);
	return(free(pth_cpy), res);	
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
	full_path = malloc(total_len);
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
