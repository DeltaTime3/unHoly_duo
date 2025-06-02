/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_put_hex.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afilipe- <afilipe-@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 14:59:05 by afilipe-          #+#    #+#             */
/*   Updated: 2025/06/02 15:00:15 by afilipe-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_put_hex(unsigned int nb, const char form)
{
	int		length;
	char	*hex_digits;

	length = 0;
	if (form == 'x')
		hex_digits = "0123456789abcdef";
	else if (form == 'X')
		hex_digits = "0123456789ABCDEF";
	else
		return 0;
	if (nb >= 16)
		length += ft_put_hex(nb / 16, form);
	length += ft_printc(hex_digits[nb % 16]);
	return length;
}
