/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lglover <lglover@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/27 19:29:28 by lglover           #+#    #+#             */
/*   Updated: 2018/12/21 13:29:22 by lglover          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strncat(char *s1, const char *s2, size_t n)
{
	size_t	last_pos1;
	size_t	last_pos2;

	last_pos1 = ft_strlen(s1);
	last_pos2 = ft_strlen(s2);
	if (last_pos2 < n)
	{
		ft_strcpy(&s1[last_pos1], s2);
	}
	else
	{
		ft_strncpy(&s1[last_pos1], s2, n);
		s1[last_pos1 + n] = '\0';
	}
	return (s1);
}
