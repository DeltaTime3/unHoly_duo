/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_hex.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afilipe- <afilipe-@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/14 13:11:38 by ppaula-d          #+#    #+#             */
/*   Updated: 2025/06/02 14:58:11 by afilipe-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_print_hex(unsigned int nb, const char form)
{
	if (nb == 0)
		return (ft_printc('0'));
	else
		return (ft_put_hex(nb, form));
	return (0);
}

