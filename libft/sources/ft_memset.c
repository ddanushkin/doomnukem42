/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lglover <lglover@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/23 12:41:00 by lglover           #+#    #+#             */
/*   Updated: 2019/09/05 11:17:27 by lglover          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memset(void *b, int c, size_t len)
{
	unsigned char		*dst_small;
	unsigned long int	*dst_big;

	dst_big = b;
	while (len >= 8)
	{
		*dst_big = (unsigned long int)c;
		dst_big++;
		len -= 8;
	}

	dst_small = (void *)dst_big;
	while (len > 0)
	{
		*dst_small = (unsigned char)c;
		dst_small++;
		len--;
	}
	return (b);
}
