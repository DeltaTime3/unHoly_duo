/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils4.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppaula-d <ppaula-d@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 13:59:12 by ppaula-d          #+#    #+#             */
/*   Updated: 2025/07/30 17:16:00 by ppaula-d         ###   ########.fr       */
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

static void	update_quote_state(char current_char, char *quote_state)
{
	if (current_char == '\'' || current_char == '"')
	{
		if (*quote_state == '\0')
			*quote_state = current_char;
		else if (*quote_state == current_char)
			*quote_state = '\0';
	}
}

static int	check_redirect_sequence(const char *input, int i)
{
	char	redirect_char;

	redirect_char = input[i];
	if (input[i + 1] == redirect_char && input[i + 2] == redirect_char)
	{
		ft_printf_fd(2, "minishell: syntax error near unexpected token\n",
			redirect_char);
		return (1);
	}
	return (0);
}

int	check_inv_red(const char *input)
{
	int		i;
	char	quote;

	i = 0;
	quote = '\0';
	while (input[i])
	{
		update_quote_state(input[i], &quote);
		if (quote == '\0' && (input[i] == '>' || input[i] == '<'))
		{
			if (check_redirect_sequence(input, i))
				return (1);
		}
		i++;
	}
	return (0);
}
