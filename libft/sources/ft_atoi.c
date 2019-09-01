/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lglover <lglover@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/22 13:14:05 by lglover           #+#    #+#             */
/*   Updated: 2019/03/20 18:44:28 by lglover          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	isoverlong(unsigned long long num, int sign)
{
	if (num > 9223372036854775807)
	{
		if (sign > 0)
			return (-1);
		if (sign < 0)
			return (0);
	}
	return (sign * num);
}

int			ft_atoi(const char *str)
{
	unsigned long long	num;
	int					sign;

	num = 0;
	sign = 1;
	while ((*str >= 0 && *str <= 31) || *str == '\e' || *str == ' ')
	{
		if (*str == '\e')
			return (num);
		str++;
	}
	if (*str == '+' || *str == '-')
	{
		if (*str == '-')
			sign = -1;
		str++;
	}
	while (*str && ft_isdigit(*str))
	{
		num = num * 10 + (*str - '0');
		str++;
	}
	return (isoverlong(num, sign));
}
