/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lglover <lglover@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/03 17:19:42 by lglover           #+#    #+#             */
/*   Updated: 2018/12/21 13:29:22 by lglover          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void	print_num(long long nbr, int fd)
{
	if (nbr >= 10)
	{
		print_num(nbr / 10, fd);
		ft_putchar_fd((nbr % 10) + '0', fd);
	}
	else
	{
		ft_putchar_fd(nbr + '0', fd);
	}
}

void		ft_putnbr_fd(int n, int fd)
{
	long long	lng_n;
	int			sign;

	sign = 1;
	lng_n = (long long)n;
	if (lng_n < 0)
	{
		ft_putchar_fd('-', fd);
		sign = -1;
	}
	print_num(sign * lng_n, fd);
}
