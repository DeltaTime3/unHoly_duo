/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils4.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppaula-d <ppaula-d@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 13:59:12 by ppaula-d          #+#    #+#             */
/*   Updated: 2025/07/27 14:08:33 by ppaula-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_invalid_sequence(const char *input, int i)
{
	if (!input[i] || !input[i + 1] || !input[i + 2])
		return (0);
	if (input[i + 1] && input[i + 2]
		&& ((input[i] == '|' && input[i + 1] == '|')
			|| (input[i] == '|' && input[i + 2] == '|')
			|| (input[i] == '<' && input[i + 1] == '<')
			|| (input[i] == '<' && input[i + 2] == '<')
			|| (input[i] == '>' && input[i + 1] == '>')
			|| (input[i] == '>' && input[i + 2] == '>')))
		return (1);
	return (0);
}

int	check_token_sequence(const char *input)
{
	int	i;

	i = 0;
	while (input[i])
	{
		while (input[i] && ft_isspace(input[i]))
			i++;
		if (input[i])
			break ;
		if (is_invalid_sequence(input, i))
		{
			ft_printf_fd(2, UNEXPECTED_TOKEN);
			return (1);
		}
		if ((input[i] == '|' || input[i] == '<' || input[i] == '>')
			&& (i == 0 || input[i + 1] == '\0'))
		{
			ft_printf_fd(2, UNEXPECTED_TOKEN);
			return (1);
		}
		i++;
	}
	return (0);
}
