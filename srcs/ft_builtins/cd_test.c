#include "../../include/minishell.h"
#include <stdio.h>
#include <stdlib.h>

// Mock function to simulate print_error
int print_error(char *msg)
{
    fprintf(stderr, "%s", msg);
    return 1;
}

// Mock function to simulate expander (if not implemented yet)
void expander(t_token **token, t_shell *type)
{
    (void)token; // Mark token as unused
    (void)type;  // Mark type as unused
}

// Main test function
int main(void)
{
    t_shell shell;
    t_token token;
    t_token next_token;

    // Initialize shell state
    shell.env_var = malloc(sizeof(char *) * 3);
    shell.env_var[0] = strdup("HOME=/home/testuser");
    shell.env_var[1] = strdup("OLDPWD=/home/testuser/old");
    shell.env_var[2] = NULL;
    shell.curr_dir = strdup("/home/testuser");
    shell.prev_dir = strdup("/home/testuser/old");
    shell.pwd = strdup("/home/testuser");
    shell.r_code = 0;

    // Initialize token
    token.value = NULL; // No arguments for the first test case
    token.next = NULL;

    // Test case 1: No arguments (should go to HOME)
    ft_cd(&token, &shell);
    printf("Current Directory: %s\n", shell.curr_dir);

    // Test case 2: Change to a valid directory
    next_token.value = strdup("/tmp"); // Properly initialize the value
    next_token.next = NULL;            // Ensure the next pointer is NULL
    token.next = &next_token;
    ft_cd(&token, &shell);
    printf("Current Directory: %s\n", shell.curr_dir);
    free(next_token.value); // Free memory after use

    // Test case 3: Change to previous directory ("-")
    next_token.value = strdup("-"); // Properly initialize the value
    next_token.next = NULL;         // Ensure the next pointer is NULL
    token.next = &next_token;
    ft_cd(&token, &shell);
    printf("Current Directory: %s\n", shell.curr_dir);
    free(next_token.value); // Free memory after use

    // Test case 4: Invalid directory
    next_token.value = strdup("/invalid/path"); // Properly initialize the value
    next_token.next = NULL;                     // Ensure the next pointer is NULL
    token.next = &next_token;
    ft_cd(&token, &shell);
    printf("Return Code: %d\n", shell.r_code);
    free(next_token.value); // Free memory after use

    // Cleanup
    free(shell.env_var[0]);
    free(shell.env_var[1]);
    free(shell.env_var);
    free(shell.curr_dir);
    free(shell.prev_dir);
    free(shell.pwd);

    return 0;
}