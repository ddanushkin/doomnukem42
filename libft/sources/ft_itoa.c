/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lglover <lglover@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/03 13:04:06 by lglover           #+#    #+#             */
/*   Updated: 2018/12/21 13:29:22 by lglover          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int		get_sign(int n)
{
	if (n < 0)
		return (-1);
	return (1);
}

static size_t	get_nbr_len(long num)
{
	if (num >= 1000000000)
		return (10);
	if (num >= 100000000)
		return (9);
	if (num >= 10000000)
		return (8);
	if (num >= 1000000)
		return (7);
	if (num >= 100000)
		return (6);
	if (num >= 10000)
		return (5);
	if (num >= 1000)
		return (4);
	if (num >= 100)
		return (3);
	if (num >= 10)
		return (2);
	return (1);
}

char			*ft_itoa(int n)
{
	size_t	nbr_len;
	int		sign;
	char	*res;
	int		i;
	long	lng_n;

	lng_n = (long)n;
	sign = get_sign(lng_n);
	lng_n = (lng_n < 0 ? lng_n * -1 : lng_n);
	nbr_len = (sign < 0 ? get_nbr_len(lng_n) + 1 : get_nbr_len(lng_n));
	i = nbr_len - 1;
	res = ft_strnew(nbr_len);
	if (!res)
		return (NULL);
	while (i >= 0)
	{
		res[i] = (lng_n % 10) + '0';
		lng_n = lng_n / 10;
		i--;
	}
	if (sign < 0)
		res[0] = '-';
	res[nbr_len] = '\0';
	return (res);
}
