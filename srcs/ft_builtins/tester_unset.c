/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tester_unset.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afilipe- <afilipe-@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 12:01:02 by afilipe-          #+#    #+#             */
/*   Updated: 2025/06/02 12:42:04 by afilipe-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../include/minishell.h"

// Helper: create a t_token list from an array of strings
t_token *make_token_list(char **names) {
    t_token *head = NULL, *curr = NULL, *node;
    int i = 0;
    while (names[i]) {
        node = malloc(sizeof(t_token));
        node->value = strdup(names[i]);
        node->token = NULL;
        node->args = NULL;
        node->next = NULL;
        if (!head)
            head = node;
        else
            curr->next = node;
        curr = node;
        i++;
    }
    return head;
}

// Helper: free a t_token list
void free_token_list(t_token *head) {
    t_token *tmp;
    while (head) {
        tmp = head;
        head = head->next;
        free(tmp->value);
        free(tmp);
    }
}

int main(void) {
    t_shell shell;
    memset(&shell, 0, sizeof(shell));
    char *envs[][2] = {
        {"USER", "alice"},
        {"HOME", "/home/alice"},
        {"_TEST", "foo"},
        {"VAR123", "bar"},
        {NULL, NULL}
    };
    char *unset_names[] = {
        "HOME",         // valid, should be removed
        "VAR123",       // valid, should be removed
        "INVALID-NAME", // invalid, should print error
        "123BAD",       // invalid, should print error
        "_TEST",        // valid, should be removed
        NULL
    };

    // Build environment list using your API
    for (int i = 0; envs[i][0]; i++) {
        // We don't need to create_env_node ourselves, just add_new_node
        add_new_node(&shell.head, NULL, envs[i][0], envs[i][1]);
    }

    // Print environment before unset
    printf("Before unset:\n");
    print_env(&shell);

    // Build token list for unset
    t_token *unset_args = make_token_list(unset_names);

    // Run unset
    printf("\nRunning unset...\n");
    int ret = ft_unset(&shell, unset_args);

    // Print environment after unset
    printf("\nAfter unset:\n");
    print_env(&shell);

    // Show return code
    printf("\nft_unset return code: %d\n", ret);

    // Cleanup
    free_token_list(unset_args);
    free_env(shell.head);

    return 0;
}
