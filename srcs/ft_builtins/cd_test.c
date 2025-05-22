/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_test.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afilipe- <afilipe-@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 15:47:58 by afilipe-          #+#    #+#             */
/*   Updated: 2025/05/22 15:52:42 by afilipe-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../include/minishell.h"

// Mock implementations for required functions
// You need to provide real ones or adapt as needed

// Helper to create a t_token linked list for arguments
t_token *make_token_list(const char *cmd, const char *arg) {
    t_token *head = malloc(sizeof(t_token));
    head->value = strdup(cmd);
    head->next = NULL;
    if (arg) {
        head->next = malloc(sizeof(t_token));
        head->next->value = strdup(arg);
        head->next->next = NULL;
    }
    return head;
}

void free_token_list(t_token *token) {
    t_token *tmp;
    while (token) {
        tmp = token;
        token = token->next;
        free(tmp->value);
        free(tmp);
    }
}

// Minimal t_shell mock
t_shell *make_shell(void) {
    t_shell *sh = malloc(sizeof(t_shell));
    sh->curr_dir = getcwd(NULL, 0);
    sh->prev_dir = strdup(sh->curr_dir);
    sh->pwd = strdup(sh->curr_dir);
    // Set up env_var as needed for your code
    // ...
    sh->r_code = 0;
    return sh;
}

void free_shell(t_shell *sh) {
    free(sh->curr_dir);
    free(sh->prev_dir);
    free(sh->pwd);
    // free env_var if needed
    free(sh);
}

int main(void) {
    t_shell *sh = make_shell();
    t_token *token;

    // Test 1: cd with no arguments (should go to HOME)
    token = make_token_list("cd", NULL);
    ft_cd(token, sh);
    printf("After 'cd': curr_dir=%s, r_code=%d\n", sh->curr_dir, sh->r_code);
    free_token_list(token);

    // Test 2: cd to a valid directory
    token = make_token_list("cd", "/tmp");
    ft_cd(token, sh);
    printf("After 'cd /tmp': curr_dir=%s, r_code=%d\n", sh->curr_dir, sh->r_code);
    free_token_list(token);

    // Test 3: cd to invalid directory
    token = make_token_list("cd", "/not_a_real_dir");
    ft_cd(token, sh);
    printf("After 'cd /not_a_real_dir': curr_dir=%s, r_code=%d\n", sh->curr_dir, sh->r_code);
    free_token_list(token);

    // Test 4: cd -
    token = make_token_list("cd", "-");
    ft_cd(token, sh);
    printf("After 'cd -': curr_dir=%s, r_code=%d\n", sh->curr_dir, sh->r_code);
    free_token_list(token);

    free_shell(sh);
    return 0;
}
