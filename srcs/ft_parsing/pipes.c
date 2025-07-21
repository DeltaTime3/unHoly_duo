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

int handle_pipes(t_token *tokens, t_shell *shell)
{
    int pipe_count;
    int pipes[2][2];
    pid_t pid;
    t_token *current;
    int i;
    int status;
    int saved_stdin;
    int saved_stdout;
    pid_t last_pid = -1;

    pipes[0][0] = -1; 
    pipes[0][1] = -1;
    pipes[1][0] = -1; 
    pipes[1][1] = -1;
    pipe_count = count_pipes(tokens);
    if (pipe_count == 0)
        return (0);
    saved_stdin = dup(STDIN_FILENO);
    saved_stdout = dup(STDOUT_FILENO);
    if (saved_stdin == -1 || saved_stdout == -1)
        return (-1);
    current = tokens;
    
    // First command in pipeline
    if (pipe(pipes[0]) < 0)
        return (print_error("Pipe creation failed\n"), -1);
    
    pid = fork();
    if (pid < 0)
    {
        close(pipes[0][0]);
        close(pipes[0][1]);
        return (print_error("Fork failed\n"), -1);
    }
    else if (pid == 0) // Child for first command
    {
        dup2(pipes[0][1], STDOUT_FILENO);
        close(pipes[0][0]);
        close(pipes[0][1]);
        
        // Redirection: if it fails, exit with 0 so pipeline continues
        if (redirect_handling(current, shell) == -1)
        {
            exit(1);
        }
        
        if (is_builtin(current))
            choose_b_in(current, shell);
        else
            execute2(shell, current);
        exit(shell->exit_code);
    }
    
    // Parent: close write end of first pipe
    close(pipes[0][1]);
    
    // Middle commands
    for (i = 0; i < pipe_count - 1; i++)
    {
        current = find_next_cmd_after_pipe(current);
        while (current && (!current->value || current->value[0] == '\0'))
            current = find_next_cmd_after_pipe(current);
        if (!current) break;
        if (pipe(pipes[(i + 1) % 2]) < 0)
        {
            if (pipes[i % 2][0] >= 0)
                close(pipes[i % 2][0]);
            return (print_error("Pipe creation failed\n"), -1);
        }
        pid = fork();
        if (pid < 0)
        {
            if (pipes[i % 2][0] >= 0)
                close(pipes[i % 2][0]);
            if (pipes[(i + 1) % 2][0] >= 0)
                close(pipes[(i + 1) % 2][0]);
            if (pipes[(i + 1) % 2][1] >= 0)
                close(pipes[(i + 1) % 2][1]);
            return (print_error("Fork failed\n"), -1);
        }
        else if (pid == 0) // Child process
        {
            dup2(pipes[i % 2][0], STDIN_FILENO);
            dup2(pipes[(i + 1) % 2][1], STDOUT_FILENO);
            if (pipes[0][0] >= 0) close(pipes[0][0]);
            if (pipes[0][1] >= 0) close(pipes[0][1]);
            if (pipes[1][0] >= 0) close(pipes[1][0]);
            if (pipes[1][1] >= 0) close(pipes[1][1]);
            
            // Redirection: if it fails, exit with 0
            if (redirect_handling(current, shell) == -1)
            {
                exit(1);
            }
            
            if (is_builtin(current))
                choose_b_in(current, shell);
            else
                execute2(shell, current);
            exit(shell->exit_code);
        }
        // Parent: close read end of current pipe
        if (pipes[i % 2][0] >= 0)
            close(pipes[i % 2][0]);
        // Parent: close write end of new pipe
        if (pipes[(i + 1) % 2][1] >= 0)
            close(pipes[(i + 1) % 2][1]);
    }
    
    // Handle last command
    if (pipe_count > 0)
    {
        current = find_next_cmd_after_pipe(current);
        while (current && (!current->value || current->value[0] == '\0'))
            current = find_next_cmd_after_pipe(current);
        if (current)
        {
            pid = fork();
            if (pid < 0)
            {
                if (pipes[(pipe_count-1) % 2][0] >= 0)
                    close(pipes[(pipe_count-1) % 2][0]);
                return (print_error("Fork failed\n"), -1);
            }
            else if (pid == 0) // Child for last command
            {
                dup2(pipes[(pipe_count-1) % 2][0], STDIN_FILENO);
                close(pipes[(pipe_count-1) % 2][0]);
                
                // Redirection: if it fails, exit with 0
                if (redirect_handling(current, shell) == -1)
                {
                    exit(1);
                }
                
                if (is_builtin(current))
                    choose_b_in(current, shell);
                else
                    execute2(shell, current);
                exit(shell->exit_code);
            }
            last_pid = pid;
        }
        // Parent: close read end of last pipe
        if (pipes[(pipe_count-1) % 2][0] >= 0)
            close(pipes[(pipe_count-1) % 2][0]);
    }
    
    // Wait for all children and track the last one's status
    pid_t wpid;
    int last_status = 0;
    while ((wpid = wait(&status)) > 0)
    {
        if (wpid == last_pid)
            last_status = status;
    }
    
    // Set exit status from last command
    if (last_pid > 0)
    {
        if (WIFEXITED(last_status))
            shell->exit_code = WEXITSTATUS(last_status);
        else if (WIFSIGNALED(last_status))
            shell->exit_code = 128 + WTERMSIG(last_status);
    }
    
    // Restore stdin/stdout
    dup2(saved_stdin, STDIN_FILENO);
    dup2(saved_stdout, STDOUT_FILENO);
    close(saved_stdin);
    close(saved_stdout);
    
    return (0);
}