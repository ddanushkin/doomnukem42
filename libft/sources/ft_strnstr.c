/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lglover <lglover@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/26 11:49:46 by lglover           #+#    #+#             */
/*   Updated: 2018/12/28 12:16:36 by lglover          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	check_needle(const char *haystack, const char *needle, size_t len)
{
	unsigned char	*h_tmp;
	unsigned char	*n_tmp;

	h_tmp = (unsigned char *)haystack;
	n_tmp = (unsigned char *)needle;
	while (len > 0 && *n_tmp && *h_tmp && *n_tmp == *h_tmp)
	{
		h_tmp++;
		n_tmp++;
		if (len != 0)
			len--;
	}
	if (*n_tmp)
		return (0);
	return (1);
}

char		*ft_strnstr(const char *haystack, const char *needle, size_t len)
{
	if (!*needle)
		return ((char *)haystack);
	while (*haystack && *needle && len > 0)
	{
		if (*haystack == *needle && check_needle(haystack, needle, len))
			return ((char *)haystack);
		haystack++;
		if (len > 0)
			len--;
	}
	return (NULL);
}
