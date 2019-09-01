/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lglover <lglover@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/28 11:01:15 by lglover           #+#    #+#             */
/*   Updated: 2018/12/21 13:29:22 by lglover          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t size)
{
	size_t count_len;
	size_t src_len;

	count_len = 0;
	src_len = ft_strlen(src);
	while (*dst && size > 0)
	{
		dst++;
		count_len++;
		size--;
	}
	size = (size >= 1 ? size - 1 : size);
	ft_strncat(dst - count_len, src, size);
	return (src_len + count_len);
}
