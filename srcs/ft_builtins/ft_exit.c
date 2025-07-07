/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afilipe- <afilipe-@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 11:51:52 by afilipe-          #+#    #+#             */
/*   Updated: 2025/07/07 11:46:57 by afilipe-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	ft_exit(t_token **token, t_shell *type)
{
    int			nbr_args;
    char		*arg;

    arg = NULL;
    nbr_args = token_counter(*token);
    ft_putstr_fd("exit\n", STDOUT_FILENO);
    if (nbr_args > 1)
        arg = (*token)->next->value;
    if (nbr_args > 2)
    {
        if (!ft_is_nbr(arg))
        {
            ft_printf_fd(2, "exit: %s: numeric argument required\n", arg);
            ft_kill(type, *token, 2);
        }
        else
        {
            print_error(E_ARGS);
            return (1);
        }
    }
    ft_exit2(type, *token, nbr_args, arg); // Pass *token to ft_exit2
    return (0);
}

int	ft_exit2(t_shell *type, t_token *tokens, int nbr_args, char *arg)
{
    long long	code;
    int			atol_error;

    atol_error = 0;
    if (nbr_args == 1)
        ft_kill(type, tokens, 0); // Pass tokens
    if (nbr_args == 2)
    {
        code = ft_atoll(arg, &atol_error);
        if (atol_error)
        {
            print_error(E_NOTNBR);
            ft_kill(type, tokens, 2); // Pass tokens
        }
        code = ((code % 256) + 256) % 256;
        ft_kill(type, tokens, (int)code); // Pass tokens
    }
    return (0);
}

int	ft_is_nbr(char *str)
{
	int	i;

	i = 0;
	if ((!str) || (*str == '\0'))
		return (0);
	if (str[0] == '+' || str[0] == '-')
		++i;
	if (!str[i])
		return (0);
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	return (1);
}

void	ft_kill(t_shell *type, t_token *tokens, int e_code)
{
    free_tokens(tokens); // Free the token list
    clean_all_resources(type);
    rl_clear_history();
    rl_cleanup_after_signal();
    exit(e_code);
}

int	token_counter(t_token *token)
{
	int	count;
	t_token	*curr;

	count = 0;
	curr = token;
	while (curr)
	{
		count++;
		curr = curr->next;
	}
	return (count);
}

// void	ft_free_shell(t_shell *shell)
// {
// 	if (!shell)
//         return;
//     if (shell->head)
//         free_env(shell->head);
//     if (shell->token)
//     {
//         if (shell->token->value)
//             free(shell->token->value);
//         if (shell->token->args)
//             free_args(shell->token->args);
//         free(shell->token);
//         shell->token = NULL;
//     }
//     if (shell->prev_dir)
//         free(shell->prev_dir);
//     if (shell->curr_dir)
//         free(shell->curr_dir);
//     if (shell->pwd)
//         free(shell->pwd);
//     shell->head = NULL;
//     shell->token = NULL;
//     shell->prev_dir = NULL;
//     shell->curr_dir = NULL;
//     shell->pwd = NULL;
// }
