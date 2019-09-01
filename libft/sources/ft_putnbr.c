/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lglover <lglover@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/03 15:37:46 by lglover           #+#    #+#             */
/*   Updated: 2018/12/21 13:29:22 by lglover          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void	print_num(long long nbr)
{
	if (nbr >= 10)
	{
		print_num(nbr / 10);
		ft_putchar((nbr % 10) + '0');
	}
	else
	{
		ft_putchar(nbr + '0');
	}
}

void		ft_putnbr(int n)
{
	long long	lng_n;
	int			sign;

	sign = 1;
	lng_n = (long long)n;
	if (lng_n < 0)
	{
		ft_putchar('-');
		sign = -1;
	}
	print_num(sign * lng_n);
}
