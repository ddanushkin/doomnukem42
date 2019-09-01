/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putchar_fd.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lglover <lglover@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/03 17:07:09 by lglover           #+#    #+#             */
/*   Updated: 2018/12/21 13:29:22 by lglover          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_putchar_fd(char c, int fd)
{
	int tmp1;
	int tmp2;

	if ((unsigned char)c < 128)
	{
		write(fd, &c, 1);
	}
	else
	{
		tmp1 = 192 + (unsigned char)c / 64;
		tmp2 = 128 + (unsigned char)c % 64;
		write(fd, &tmp1, 1);
		write(fd, &tmp2, 1);
	}
}
