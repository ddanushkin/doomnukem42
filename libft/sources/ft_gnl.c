/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lglover <lglover@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/07 13:22:45 by lglover           #+#    #+#             */
/*   Updated: 2019/03/20 13:11:10 by lglover          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	check_line(char **line, char **rem)
{
	if (!(*line = ft_strdup(*rem)))
	{
		ft_strdel(rem);
		return (0);
	}
	return (1);
}

static int	fill_line(char **rem, char **line)
{
	char	*nl_ptr;

	if ((nl_ptr = ft_strchr(*rem, '\n')))
	{
		*nl_ptr = '\0';
		if (!check_line(line, rem))
			return (-1);
		*rem = ft_strcpy(*rem, nl_ptr + 1);
	}
	else
	{
		if (!check_line(line, rem))
			return (-1);
		ft_strdel(rem);
	}
	return (1);
}

static int	reconcat(char **file_data, char *buff)
{
	char *tmp;

	if (*file_data)
	{
		tmp = *file_data;
		if (!(*file_data = ft_strjoin(*file_data, buff)))
		{
			ft_strdel(&tmp);
			return (0);
		}
		ft_strdel(&tmp);
		return (1);
	}
	else
	{
		if (!(*file_data = ft_strdup(buff)))
			return (0);
		return (1);
	}
}

int			ft_gnl(const int fd, char **line)
{
	char			buff[BUFF_SIZE + 1];
	static char		*data;
	long			read_result;

	if (fd < 0 || !line || (read(fd, buff, 0) < 0) || BUFF_SIZE <= 0)
		return (-1);
	while ((read_result = read(fd, buff, BUFF_SIZE)))
	{
		buff[read_result] = '\0';
		if (!reconcat(&data, buff))
			return (-1);
		if (ft_strchr(buff, '\n'))
			break ;
	}
	if (!data || !data[0])
		return (0);
	return (fill_line(&data, line));
}
