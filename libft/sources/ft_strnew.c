/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnew.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lglover <lglover@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/29 09:36:48 by lglover           #+#    #+#             */
/*   Updated: 2018/12/27 16:41:56 by lglover          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnew(size_t size)
{
	char	*s;

	if (size + 1 == 0)
		return (NULL);
	s = (char *)ft_memalloc(size + 1);
	if (s == NULL)
		return (NULL);
	return (s);
}
