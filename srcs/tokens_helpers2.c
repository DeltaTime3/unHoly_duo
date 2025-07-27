/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens_helpers2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppaula-d <ppaula-d@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 13:59:12 by ppaula-d          #+#    #+#             */
/*   Updated: 2025/07/27 13:21:04 by ppaula-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	input_handling(char *input, t_token *tokens)
{
	(void)tokens;
	input = readline("minishell> ");
	if (!input)
		exit_handling(input);
	else if (ft_strlen(input) == 0)
		free(input);
	else if (ft_strcmp(input, "exit") == 0)
		exit_handling(input);
	else
		process_input(input);
}

void	process_input(char *input)
{
	t_token	*tokens;

	if (*input)
		add_history(input);
	if (validate_input(input))
	{
		free(input);
		return ;
	}
	tokens = tokenize_input(input);
	if (!tokens)
	{
		free(input);
		return ;
	}
	free(input);
	if (tokens)
		free_tokens(tokens);
}

void	exit_handling(char *input)
{
	printf("exit\n");
	free(input);
	rl_clear_history();
	exit(0);
}

void	add_token(t_token **head, t_token *new_token)
{
	t_token	*current;

	if (!head || !new_token)
		return ;
	if (*head == NULL)
	{
		*head = new_token;
		return ;
	}
	current = *head;
	while (current->next)
		current = current->next;
	current->next = new_token;
}

int	word_handling(const char *input, int *i, t_token **tokens,
		int *expect_command)
{
	int		start;
	char	*content;
	t_cat	type;

	start = *i;
	scan_word_with_quotes(input, i);
	content = ft_substr(input, start, *i - start);
	if (!content)
		return (1);
	type = determine_token_type(content, expect_command);
	add_token(tokens, create_token(type, content));
	free(content);
	return (0);
}
