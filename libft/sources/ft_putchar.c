/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putchar.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lglover <lglover@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/21 16:59:33 by lglover           #+#    #+#             */
/*   Updated: 2018/12/21 13:29:22 by lglover          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_putchar(char c)
{
	int tmp1;
	int tmp2;

	if ((unsigned char)c < 128)
	{
		write(1, &c, 1);
	}
	else
	{
		tmp1 = 192 + (unsigned char)c / 64;
		tmp2 = 128 + (unsigned char)c % 64;
		write(1, &tmp1, 1);
		write(1, &tmp2, 1);
	}
}
