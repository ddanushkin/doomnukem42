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

int		check_resources(void)
{
	int		fd;
	char	hash[33];

	system("md5 -q GAME_DATA > .md5_hash");
	fd = open(".md5_hash", O_RDONLY);
	if (fd < 3)
		exit_with_status(STATUS_FILE_NOT_FOUND, ".md5_hash");
	hash[32] = '\0';
	read(fd, hash, 32);
	close(fd);
	return (ft_strequ(hash, RESOURCES_MD5));
}

void	gamedata_write(int fd, void *mem, size_t size, char *type)
{
	char	info[100];

	info[0] = '\0';
	ft_strcat(&info[0], type);
	ft_itoa2(size, &info[2]);
	ft_strcat(&info[0], ":");
	printf("WRITE_INFO: %s\n", info);
	write(fd, &info[0], ft_strlen(info));
	write(fd, mem, size);
}

void	map_save(t_app *a, char *name)
{
	int		data;

	data = open(name, O_RDWR | O_CREAT | O_TRUNC, 0777);
	if (data == -1)
		return ;
	gamedata_write(data, &a->sectors[0], sizeof(t_sector) * a->sectors_count, "S:\0");
	gamedata_write(data, &a->md, sizeof(t_map_data), "D:\0");
	close(data);
	usleep(10);
}
