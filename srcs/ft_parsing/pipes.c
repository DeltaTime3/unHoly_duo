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
    current = tokens;
    while (current && (!current->value || current->value[0] == '\0'))
        current = find_next_cmd_after_pipe(current);
    if (!current) 
    {
        shell->exit_code = 0;
        return 0;
    }
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
        // Redirection: if it fails, print error and exit(1), but don't set shell->exit_code
        if (redirect_handling(current, shell) == -1)
        {
            perror("minishell");
            exit(1);
        }
        if (is_builtin(current))
            choose_b_in(current, shell);
        else
            execute2(shell, current);
        exit(shell->exit_code);
    }
    if (pipe_count > 0 && pipes[0][1] >= 0)
        close(pipes[0][1]);
    // Handle middle commands (if any)
    for (i = 1; i < pipe_count; i++)
    {
        current = find_next_cmd_after_pipe(current);
        while (current && (!current->value || current->value[0] == '\0'))
            current = find_next_cmd_after_pipe(current);
        if (!current) break;
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
            dup2(pipes[(i-1) % 2][0], STDIN_FILENO);
            dup2(pipes[i % 2][1], STDOUT_FILENO);
            if (pipes[0][0] >= 0) close(pipes[0][0]);
            if (pipes[0][1] >= 0) close(pipes[0][1]);
            if (pipes[1][0] >= 0) close(pipes[1][0]);
            if (pipes[1][1] >= 0) close(pipes[1][1]);
            if (redirect_handling(current, shell) == -1)
            {
                perror("minishell");
                exit(1);
            }
            if (is_builtin(current))
                choose_b_in(current, shell);
            else
                execute2(shell, current);
            exit(shell->exit_code);
        }
        if (pipes[(i-1) % 2][0] >= 0)
            close(pipes[(i-1) % 2][0]);
        if (pipes[i % 2][1] >= 0)
            close(pipes[i % 2][1]);
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
                if (pipes[0][0] >= 0) close(pipes[0][0]);
                if (pipes[0][1] >= 0) close(pipes[0][1]);
                if (pipes[1][0] >= 0) close(pipes[1][0]);
                if (pipes[1][1] >= 0) close(pipes[1][1]);
                if (redirect_handling(current, shell) == -1)
                {
                    perror("minishell");
                    exit(1);
                }
                if (is_builtin(current))
                    choose_b_in(current, shell);
                else
                    execute2(shell, current);
                exit(shell->exit_code);
            }
            last_pid = pid;
            if (pipes[(pipe_count-1) % 2][0] >= 0)
                close(pipes[(pipe_count-1) % 2][0]);
        }
    }
    // Wait for all child processes, but set exit_code to last command's exit status
    int last_status = 0;
    i = 0;
    while (i <= pipe_count)
    {
        pid_t wpid = wait(&status);
        if (wpid == last_pid && WIFEXITED(status))
            last_status = WEXITSTATUS(status);
        i++;
    }
    shell->exit_code = last_status;
    dup2(saved_stdin, STDIN_FILENO);
    dup2(saved_stdout, STDOUT_FILENO);
    close(saved_stdin);
    close(saved_stdout);
    return 0;
}