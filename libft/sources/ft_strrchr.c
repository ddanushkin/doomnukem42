/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lglover <lglover@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/22 18:04:10 by lglover           #+#    #+#             */
/*   Updated: 2018/12/21 13:29:22 by lglover          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *s, int c)
{
	unsigned char	*ptr;
	int				len;
	unsigned char	letter;

	len = ft_strlen(s);
	while (len >= 0)
	{
		letter = *(s + len);
		if ((unsigned char)letter == (unsigned char)c)
		{
			ptr = (unsigned char *)(s + len);
			return ((char *)ptr);
		}
		len--;
	}
	return (NULL);
}
