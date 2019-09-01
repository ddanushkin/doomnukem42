/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strsplit.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lglover <lglover@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/29 15:25:27 by lglover           #+#    #+#             */
/*   Updated: 2019/08/06 11:44:18 by ndremora         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static size_t	count_words(char const *s, char c)
{
	size_t	cnt;

	cnt = 0;
	while (*s)
	{
		if (*s != c && (*(s + 1) == c || *(s + 1) == '\0'))
		{
			cnt++;
		}
		s++;
	}
	return (cnt);
}

static char		**del_array(char **array, size_t i)
{
	while (i > 0)
	{
		ft_strdel(&array[i]);
		i--;
	}
	free(array);
	return (NULL);
}

static char		**create_array(char *str, char c, char **array)
{
	size_t	len;
	size_t	i;

	i = 0;
	while (*str)
	{
		while (*str == c)
			str++;
		len = 0;
		while (*str != '\0' && *str != c)
		{
			len++;
			str++;
		}
		array[i] = ft_strnew(len);
		if (array[i] == NULL)
			return (del_array(array, i));
		i++;
		if (*str == '\0')
			break ;
		str++;
	}
	return (array);
}

static char		**input_data(char **array, char *str, char c)
{
	size_t	i;
	size_t	j;

	i = 0;
	while (*str)
	{
		if (*str != c)
		{
			j = 0;
			while (*str != c && *str != '\0')
			{
				array[i][j] = *str;
				str++;
				j++;
			}
			i++;
		}
		else
			str++;
	}
	array[i] = NULL;
	return (array);
}

char			**ft_strsplit(char const *s, char c)
{
	size_t	words;
	char	**res;
	char	*str;

	if (!s)
		return (NULL);
	words = count_words(s, c);
	str = (char*)s;
	res = (char**)malloc(sizeof(char*) * words + 1);
	if (!res)
		return (NULL);
	res = create_array(str, c, res);
	return (input_data(res, str, c));
}
