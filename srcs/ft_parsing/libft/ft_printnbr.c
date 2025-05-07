/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printnbr.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppaula-d <ppaula-d@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/14 13:10:12 by ppaula-d          #+#    #+#             */
/*   Updated: 2025/02/20 17:26:31 by ppaula-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_printnbr(int nb)
{
	char	*nbtostr;
	int		lenght;

	lenght = 0;
	nbtostr = ft_itoa(nb);
	lenght = ft_printstr(nbtostr);
	free(nbtostr);
	return (lenght);
}
