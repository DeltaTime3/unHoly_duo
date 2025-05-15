/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afilipe- <afilipe-@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 11:51:52 by afilipe-          #+#    #+#             */
/*   Updated: 2025/05/15 10:22:54 by afilipe-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	ft_exit(t_token **token, t_shell *type)
{
	int			nbr_args;
	char		*arg;
	long long	code;

	arg = NULL;
	nbr_args = token_counter(token);
	ft_putstr_fd("exit\n", STDOUT_FILENO);
	if (nbr_args > 1)
		arg = (*token)->next->value;	
	if(nbr_args > 2)
	{
		if (!ft_is_nbr(arg))
		{
			print_error(E_NOTNBR);
			ft_kill(type, 2);
		}
		else
		{
			print_error(E_ARGS);
			return (1);
		}
	}
	ft_exit2(type, nbr_args, arg);
	return (0);
}

int	ft_exit2(t_shell *type, int nbr_args, char *arg)
{
	long long	code;
	int			atol_error;

	atol_error = 0;
	if (nbr_args == 1)
		ft_kill(type, 0);
	if (nbr_args == 2)
	{
		code = ft_atoll(arg, &atol_error);
		if (atol_error)
		{
			print_error(E_NOTNBR);
			ft_kill(type, 2);
		}
		code = ((code % 256) + 256) % 256;
		ft_kill(type, (int)code);
	}
	return(0);
}

int ft_is_nbr(char *str)
{
	int i;

	i = 0;
	if ((!str) || (str == '\0'))
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

 void	ft_kill(t_shell *type, int e_code)
 {
	ft_free_shell(type);
	exit(e_code);
 }
 