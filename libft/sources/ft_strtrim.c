/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lglover <lglover@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/29 14:04:25 by lglover           #+#    #+#             */
/*   Updated: 2019/03/20 13:11:10 by lglover          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	is_space(char c)
{
	if (c == '\n' || c == '\t' || c == ' ')
		return (1);
	return (0);
}

static int	trim_start(char *c)
{
	int		i;

	i = 0;
	while (is_space(c[i]))
		i++;
	return (i);
}

static int	trim_end(char *c)
{
	int		i;

	i = ft_strlen(c) - 1;
	while (is_space(c[i]) && i >= trim_start(c))
		i--;
	return (i);
}

char		*ft_strtrim(char const *s)
{
	char	*res;
	int		len;
	int		start_idx;
	int		end_idx;
	int		i;

	if (!s)
		return (NULL);
	i = 0;
	start_idx = trim_start((char *)s);
	end_idx = trim_end((char *)s);
	len = end_idx - start_idx + 1;
	res = ft_strnew(len);
	if (res == NULL)
		return (NULL);
	while (i < len)
	{
		res[i] = s[start_idx + i];
		i++;
	}
	return (res);
}
