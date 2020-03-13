/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lglover <lglover@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/11 13:03:36 by lglover           #+#    #+#             */
/*   Updated: 2020/03/11 17:07:44 by lglover          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"

void	map_type_malloc(t_app *a, int fd, char type, uint64_t size)
{
	if (type == 'S')
	{
		a->sectors_count = (int)(size / sizeof(t_sector));
		a->sectors = malloc(sizeof(t_sector) * MAX_SECTOR);
		read(fd, a->sectors, size);
	}
	else if (type == 'D')
		read(fd, &a->md, size);
}

void	gamedata_type_malloc(t_app *a, int fd, char type, uint64_t size)
{
	if (type == 'T')
	{
		a->sprites = malloc(size);
		read(fd, a->sprites, size);
	}
	else if (type == 'A')
	{
		a->audio = malloc(sizeof(t_raw_sfx) * MAX_SFX);
		read(fd, a->audio, size);
	}
	else if (type == 'B')
	{
		a->music = malloc(sizeof(t_raw_bg) * MAX_BG);
		read(fd, a->music, size);
	}
	else if (type == 'F')
		read(fd, &a->fonts, size);
}

void	parse_data(t_app *a, int fd, char *info)
{
	char		tmp[50];
	uint64_t	size;
	char		type;
	int			i;

	i = 0;
	type = *info++;
	info++;
	while (*info != ':')
		tmp[i++] = *info++;
	tmp[i] = '\0';
	size = ft_atoi(&tmp[0]);
	gamedata_type_malloc(a, fd, type, size);
	map_type_malloc(a, fd, type, size);
	read(fd, NULL, 1);
}

int		gamedata_load(t_app *a)
{
	int		data;
	char	info[50];
	char 	buff[2];
	int 	sep;

	if ((data = open("GAME_DATA", O_RDONLY)) == -1)
		return (0);
	info[0] = '\0';
	buff[1] = '\0';
	sep = 0;
	while (read(data, &buff[0], 1))
	{
		if (buff[0] == ':')
			sep++;
		ft_strcat(&info[0], &buff[0]);
		if (sep == 2)
		{
			sep = 0;
			parse_data(a, data, &info[0]);
			info[0] = '\0';
		}
	}
	close(data);
	return (1);
}

int		map_load(t_app *a, char *name)
{
	int		data;
	char	info[50];
	char 	buff[2];
	int 	sep;

	if ((data = open(name, O_RDONLY)) == -1)
		return (0);
	info[0] = '\0';
	buff[1] = '\0';
	sep = 0;
	while (read(data, &buff[0], 1))
	{
		if (buff[0] == ':')
			sep++;
		ft_strcat(&info[0], &buff[0]);
		if (sep == 2)
		{
			sep = 0;
			parse_data(a, data, &info[0]);
			info[0] = '\0';
		}
	}
	close(data);
	return (1);
}
