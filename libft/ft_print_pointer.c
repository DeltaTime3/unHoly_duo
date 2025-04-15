/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_pointer.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppaula-d <ppaula-d@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/14 13:09:22 by ppaula-d          #+#    #+#             */
/*   Updated: 2025/02/14 13:09:35 by ppaula-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_print_pointer(unsigned long pointer)
{
	int	lenght;

	lenght = 0;
	if (!pointer)
	{
		ft_putstr_fd("(nil)", 1);
		return (5);
	}
	else
	{
		lenght += ft_printstr("0x");
		lenght += ft_put_hex(pointer, 'x');
	}
	return (lenght);
}
