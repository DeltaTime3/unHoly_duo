/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppaula-d <ppaula-d@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 12:37:38 by ppaula-d          #+#    #+#             */
/*   Updated: 2025/05/22 09:59:57 by ppaula-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void ft_printf_fd(int fd, const char *format, ...)
{
    va_list args;
    int     i;

    va_start(args, format);
    i = 0;
    while (format[i])
    {
        if (format[i] == '%' && format[i + 1] != '\0')
        {
            i++;
            if (format[i] == 's')
                ft_putstr_fd(va_arg(args, char *), fd);
            else if (format[i] == 'd')
                ft_putnbr_fd(va_arg(args, int), fd);
            else if (format[i] == 'c')
                ft_putchar_fd(va_arg(args, int), fd);
            else if (format[i] == '%')
                ft_putchar_fd('%', fd);          
        }
        else
            ft_putchar_fd(format[i], fd);
        i++;
    }
    va_end(args);
}