/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strstr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lglover <lglover@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/26 10:20:17 by lglover           #+#    #+#             */
/*   Updated: 2018/12/21 13:29:22 by lglover          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	check_needle(const char *haystack, const char *needle)
{
	unsigned char	*h_tmp;
	unsigned char	*n_tmp;

	h_tmp = (unsigned char *)haystack;
	n_tmp = (unsigned char *)needle;
	while (*n_tmp && *h_tmp && *n_tmp == *h_tmp)
	{
		h_tmp++;
		n_tmp++;
	}
	if (*n_tmp)
		return (0);
	return (1);
}

char		*ft_strstr(const char *haystack, const char *needle)
{
	if (!*needle)
		return ((char *)haystack);
	while (*haystack && *needle)
	{
		if (*haystack == *needle && check_needle(haystack, needle))
			return ((char *)haystack);
		haystack++;
	}
	return (NULL);
}
