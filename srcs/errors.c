/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afilipe- <afilipe-@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 09:28:52 by afilipe-          #+#    #+#             */
/*   Updated: 2025/05/13 10:38:44 by afilipe-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	print_error(char *msg)
{
	ft_putstr_fd(msg, 2);
	return (1);
}

void	ft_putstr_fd(char *str, int fd)
{
	write (fd, str, ft_strlen(str));
}
