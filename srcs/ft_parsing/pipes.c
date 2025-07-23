#include "minishell.h"

t_token *find_next_cmd_after_pipe(t_token *current)
{
    while (current && current->type != PIPE)
        current = current->next;
    if (current && current->type == PIPE)
        return (current->next);
    return (NULL);
}

int count_pipes(t_token *tokens)
{
    int count;
    t_token *current;

    count = 0;
    current = tokens;
    while (current)
    {
        if (current->type == PIPE)
            count++;
        current = current->next;
    }
    return (count);
}

static void	child_process_exit(t_shell *shell, t_token *tokens, int exit_code)
{
	int	code;

	code = exit_code;
	if (shell)
	{
		code = shell->exit_code;
		clean_all_resources(shell);
	}
	if (tokens)
		free_tokens(tokens);
	exit(code);
}

int	handle_pipes(t_token *tokens, t_shell *shell)
{
	int		pipe_count;
	int		pipe_fd[2];
	int		in_fd;
	pid_t	pid;
	t_token	*current;
	int		i;
	int		status;
	int		saved_fds[2];
	pid_t	last_pid;

	pipe_count = count_pipes(tokens);
	saved_fds[0] = dup(STDIN_FILENO);
	saved_fds[1] = dup(STDOUT_FILENO);
	in_fd = saved_fds[0];
	current = tokens;
	i = 0;
	last_pid = -1;
	while (i <= pipe_count)
	{
		if (i < pipe_count)
		{
			if (pipe(pipe_fd) < 0)
				return (print_error("Pipe creation failed\n"), -1);
		}
		pid = fork();
		if (pid < 0)
			return (print_error("Fork failed\n"), -1);
		else if (pid == 0) // Child process
		{
			if (i > 0) // Not the first command, redirect stdin from previous pipe
			{
				dup2(in_fd, STDIN_FILENO);
				close(in_fd);
			}
			if (i < pipe_count) // Not the last command, redirect stdout to current pipe
			{
				close(pipe_fd[0]);
				dup2(pipe_fd[1], STDOUT_FILENO);
				close(pipe_fd[1]);
			}
			if (redirect_handling(current, shell) == -1)
				child_process_exit(shell, tokens, 1);
			if (is_builtin(current))
				choose_b_in(current, shell);
			else
				execute2(shell, current);
			child_process_exit(shell, tokens, shell->exit_code);
		}
		// Parent process
		if (i > 0)
			close(in_fd); // Close read end of the previous pipe
		if (i < pipe_count)
		{
			close(pipe_fd[1]); // Close write end of the current pipe
			in_fd = pipe_fd[0]; // Save read end for the next child
		}
		last_pid = pid;
		if (i < pipe_count)
			current = find_next_cmd_after_pipe(current);
		i++;
	}
	int last_status = 0;
	pid_t wpid;
	while ((wpid = wait(&status)) > 0)
	{
		if (wpid == last_pid)
			last_status = status;
	}
	if (WIFEXITED(last_status))
		shell->exit_code = WEXITSTATUS(last_status);
	else if (WIFSIGNALED(last_status))
		shell->exit_code = 128 + WTERMSIG(last_status);
	dup2(saved_fds[0], STDIN_FILENO);
	dup2(saved_fds[1], STDOUT_FILENO);
	close(saved_fds[0]);
	close(saved_fds[1]);
	return (0);
}
