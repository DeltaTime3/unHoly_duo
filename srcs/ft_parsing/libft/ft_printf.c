/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppaula-d <ppaula-d@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/14 13:01:50 by ppaula-d          #+#    #+#             */
/*   Updated: 2025/02/14 13:04:02 by ppaula-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_form(va_list args, const char form)
{
	int	length;

	length = 0;
	if (form == 'c')
		length += ft_printc(va_arg(args, int));
	else if (form == 's')
		length += ft_printstr(va_arg(args, char *));
	else if (form == 'p')
		length += ft_print_pointer(va_arg(args, unsigned long));
	else if (form == 'd' || form == 'i')
		length += ft_printnbr(va_arg(args, int));
	else if (form == 'u')
		length += ft_print_unsigned(va_arg(args, unsigned int));
	else if (form == 'x' || form == 'X')
		length += ft_print_hex(va_arg(args, unsigned int), form);
	else if (form == '%')
		length += ft_printc('%');
	return (length);
}

int	ft_printf(const char *str, ...)
{
	int		i;
	va_list	args;
	int		length;

	i = 0;
	length = 0;
	va_start(args, str);
	while (str[i] != '\0')
	{
		if (str[i] == '%')
		{
			length += ft_form(args, str[i + 1]);
			i++;
		}
		else
			length += ft_printc(str[i]);
		i++;
	}
	va_end(args);
	return (length);
}
