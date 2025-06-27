
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

    // Initialize all pipe file descriptors to -1
    pipes[0][0] = -1; pipes[0][1] = -1;
    pipes[1][0] = -1; pipes[1][1] = -1;
    pipe_count = count_pipes(tokens);
    current = tokens;
    if (!current)
        return (print_error("No commands to execute\n"), -1);
    saved_stdin = dup(STDIN_FILENO);
    saved_stdout = dup(STDOUT_FILENO);
    if (saved_stdin == -1 || saved_stdout == -1)
        return (print_error("Failed to save file descriptors\n"), -1);
    // Create first pipe if needed
    if (pipe_count > 0)
    {
        if (pipe(pipes[0]) < 0)
            return (print_error("Pipe creation failed\n"), -1);
    }
    // Execute first command
    pid = fork();
    if (pid < 0)
        return (print_error("Fork failed\n"), -1);
    else if (pid == 0) // Child process for first command
    {
        if (pipe_count > 0)
        {
            dup2(pipes[0][1], STDOUT_FILENO);
            if (pipes[0][0] >= 0) close(pipes[0][0]);
            if (pipes[0][1] >= 0) close(pipes[0][1]);
        }
        
        if (redirect_handling(current) == -1)
        {
            free_tokens(tokens);
            exit(1);
        }        
        if (is_builtin(current))
            choose_b_in(current, shell);
        else
            execute2(shell, current);
        free_tokens(tokens);
        exit(0);
    }
    // Parent process continues
    if (pipe_count > 0 && pipes[0][1] >= 0)
        close(pipes[0][1]);
    // Handle middle commands (if any)
    for (i = 1; i < pipe_count; i++)
    {
        // Find next command after pipe
        current = find_next_cmd_after_pipe(current);
        if (!current) break;
        
        // Create next pipe
        if (pipe(pipes[i % 2]) < 0)
        {
            if (pipes[(i-1) % 2][0] >= 0)
                close(pipes[(i-1) % 2][0]);
            return (print_error("Pipe creation failed\n"), -1);
        }
        pid = fork();
        if (pid < 0)
        {
            if (pipes[(i-1) % 2][0] >= 0)
                close(pipes[(i-1) % 2][0]);
            if (pipes[i % 2][0] >= 0)
                close(pipes[i % 2][0]);
            if (pipes[i % 2][1] >= 0)
                close(pipes[i % 2][1]);
            return (print_error("Fork failed\n"), -1);
        }
        else if (pid == 0) // Child process
        {
            // Set up stdin from previous pipe
            dup2(pipes[(i-1) % 2][0], STDIN_FILENO);
            // Set up stdout to next pipe
            dup2(pipes[i % 2][1], STDOUT_FILENO);
            // Close all pipe fds
            if (pipes[0][0] >= 0) close(pipes[0][0]);
            if (pipes[0][1] >= 0) close(pipes[0][1]);
            if (pipes[1][0] >= 0) close(pipes[1][0]);
            if (pipes[1][1] >= 0) close(pipes[1][1]);
            if (redirect_handling(current) == -1)
            {
                free_tokens(tokens);
                exit(1);
            }
            if (is_builtin(current))
                choose_b_in(current, shell);
            else
                execute2(shell, current);
            free_tokens(tokens);
            exit(0);
        }
        // Parent: close previous pipe read end and current pipe write end
        if (pipes[(i-1) % 2][0] >= 0)
            close(pipes[(i-1) % 2][0]);
        if (pipes[i % 2][1] >= 0)
            close(pipes[i % 2][1]);
    }
    // Handle last command
    if (pipe_count > 0)
    {
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
                // Read from last pipe
                dup2(pipes[(pipe_count-1) % 2][0], STDIN_FILENO);
                
                // Close all pipe fds
                if (pipes[0][0] >= 0) close(pipes[0][0]);
                if (pipes[0][1] >= 0) close(pipes[0][1]);
                if (pipes[1][0] >= 0) close(pipes[1][0]);
                if (pipes[1][1] >= 0) close(pipes[1][1]);
                
                if (redirect_handling(current) == -1)
                {
                    free_tokens(tokens);
                    exit(1);
                }                
                if (is_builtin(current))
                    choose_b_in(current, shell);
                else
                    execute2(shell, current);
                free_tokens(tokens);
                exit(0);
            }
            
            // Close the last pipe read end
            if (pipes[(pipe_count-1) % 2][0] >= 0)
                close(pipes[(pipe_count-1) % 2][0]);
        }
    }
    // Wait for all child processes
    for (i = 0; i <= pipe_count; i++)
    {
        wait(&status);
        if (WIFEXITED(status))
            shell->exit_code = WEXITSTATUS(status);
    }
    // Restore original stdin/stdout
    dup2(saved_stdin, STDIN_FILENO);
    dup2(saved_stdout, STDOUT_FILENO);
    close(saved_stdin);
    close(saved_stdout);
    return 0;
}