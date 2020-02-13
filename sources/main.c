#include "doom_nukem.h"

void	prepare_chunks(t_app *app)
{
	int		i;
	double	*depth_chunk;
	Uint32	*screen_chunk;

	i = 0;
	depth_chunk = (double *)&app->depth_chunk;
	screen_chunk = (Uint32 *)&app->screen_chunk;
	while (i++ < SCREEN_W)
	{
		*depth_chunk++ = 100000.0;
		*screen_chunk++ = 0;
	}
	app->depth_chunk_array = (t_depth_chunk *)app->depth_buffer;
	app->screen_chunk_array = (t_screen_chunk *)app->sdl->surface->pixels;
}

void	reset_screen(t_app *app)
{
	int				i;
	t_depth_chunk	*depth_buffer;
	t_screen_chunk	*screen_buffer;

	depth_buffer = app->depth_chunk_array;
	screen_buffer = app->screen_chunk_array;
	i = 0;
	while (i++ < SCREEN_H)
	{
		*depth_buffer++ = app->depth_chunk;
		*screen_buffer++ = app->screen_chunk;
	}
}

Uint8 	vertex_inside(t_v3d *v)
{
	double	w;

	w = fabs(v->w);
	return (fabs(v->x) <= w &&
			fabs(v->y) <= w &&
			fabs(v->z) <= w);
}

void	start_the_game(t_app *app)
{
	app->hit_wall = NULL;
	app->hit_sector = NULL;
	app->speed = 4.54321;
	app->acc = 0.0;
	app->jump = 0.0;
	app->fall = 0.0;
	app->height = 1.0;
	app->cursor_x = SCREEN_W * 0.5;
	app->cursor_y = SCREEN_H * 0.5;
	app->camera->pos.x = 0.0;
	app->camera->pos.z = 0.0;
	app->points_count = 0;
	app->point_mode = 0;
	prepare_chunks(app);
	switch_mode(app);
	SDL_SetRelativeMouseMode(SDL_TRUE);
	while (1)
	{
		get_delta_time(app->timer);
		ft_bzero(&app->keys, sizeof(uint8_t) * 512);
		ft_bzero(&app->mouse, sizeof(uint8_t) * 6);
		if (!event_handling(app))
			break;
		if (app->keys[SDL_SCANCODE_P] && switch_mode(app))
			continue ;
		if (app->inputs->keyboard[SDL_SCANCODE_ESCAPE])
			break ;
		if (app->point_mode)
		{
			point_mode_inputs(app);
			update_points_camera(app->camera);
			process_points_inputs(app, app->timer->delta);
			draw_cross(app, (int)app->cursor_x, (int)app->cursor_y, 8, 0xffffff);
			draw_point_mode(app);
			draw_sectors(app);
			if (app->points_count > 0)
				draw_points(app, &app->points[0], app->points_count);
		}
		else
		{
			live_mode_inputs(app);
			update_camera(app, app->camera);
			process_inputs(app, app->timer->delta);
			render_map(app);
			draw_cross(app, SCREEN_W / 2, SCREEN_H / 2, 8, 0xffffff);
		}
		update_fps_text(app);
		update_walls_data(app);
		SDL_UpdateWindowSurface(app->sdl->window);
		reset_screen(app);
	}
	TTF_CloseFont(app->font);
	TTF_Quit();
	SDL_Quit();
	SDL_DestroyWindow(app->sdl->window);
}

int	check_resources(void)
{
	int		fd;
	char 	hash[33];

	system("tar c ./resources/ | md5 > resources_hash");
	fd = open("resources_hash", O_RDONLY);
	if (fd < 3)
		exit_with_status(STATUS_FILE_NOT_FOUND, "resources_hash");
	hash[32] = '\0';
	read(fd, hash, 32);
	close(fd);
	printf("%s\n", hash);
	return (ft_strequ(hash, RESOURCES_MD5));
}

void 	gamedata_write(int fd, void *mem, size_t type_size, size_t count)
{
	char	info[50];
	size_t	size;

	size = type_size * count;
	info[0] = '\0';
	ft_strcat(&info[0], "T:");
	ft_itoa2(size, &info[2]);
	ft_strcat(&info[0], ":");
	printf("WRITE_INFO: %s\n", info);
	write(fd, &info[0], ft_strlen(info));
	write(fd, mem, size);
}

void	gamedata_save(t_app *a)
{
	int		data;

#ifdef __APPLE__
	data = open("GAME_DATA.BIN", O_RDWR | O_CREAT | O_TRUNC, 777);
#else
	data = open("GAME_DATA.BIN", O_BINARY | O_WRONLY | O_CREAT | O_TRUNC, 655);
#endif
	if (data == -1)
		return ;
	gamedata_write(data, &a->sprites[0], sizeof(t_sprite), a->sprites_count);
	close(data);
	usleep(10);
}

void 	gamedata_type_malloc(t_app *a, int fd, char type, uint64_t size)
{
	if (type == 'T')
	{
		a->sprites = malloc(size);
		read(fd, a->sprites, size);
	}
	else if (type == 'S')
	{
		a->sectors = malloc(size);
		read(fd, a->sectors, size);
	}
}

void 	gamedata_parse_info(t_app *a, int fd, char *info)
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
	read(fd, NULL, 1);
	*info = '\0';
}

void	gamedata_load(t_app *a)
{
	int		data;
	char	info[50];
	char 	buff[2];
	int 	sep;

#ifdef __APPLE__
	data = open("GAME_DATA.BIN", O_RDONLY);
#else
	data = open("GAME_DATA.BIN", O_BINARY | O_RDONLY);
#endif
	if (data == -1)
		return ;
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
			gamedata_parse_info(a, data, &info[0]);
			break ;
		}
	}
	close(data);
}

int		main(int argv, char**argc)
{
	t_app	*app;

	printf("%d, %s\n", argv, argc[0]);
	//getchar();
	//if (!check_resources())
	//	exit_with_status(STATUS_BAD_RESOURCES, NULL);
	app = (t_app *)malloc(sizeof(t_app));
	app->sdl = (t_sdl *)malloc(sizeof(t_sdl));
	app->inputs = (t_inputs *)malloc(sizeof(t_inputs));
	app->timer = (t_timer *)malloc(sizeof(t_timer));
	app->camera = (t_camera *)malloc(sizeof(t_camera));
	app->camera->up = new_vector(0.0, 1.0, 0.0);
	app->depth_buffer = (double *)malloc(sizeof(double) * SCREEN_W * SCREEN_H);
	init_app(app);

	printf("SECTORS SIZE: %d MB.\n", (int)(sizeof(t_sector)*MAX_SECTOR/1000000));
	printf("MEMORY: %d MB.\n", (int)(sizeof(*app)/1000000));

	app->sprites_count = 0;
	app->sprites = (t_sprite *)malloc(sizeof(t_sprite) * 504);
	for (int i = 0; i < 504; i++)
	{
		char	*file_name;
		char	file_path[100];
		file_path[0] = '\0';
		file_name = ft_itoa(i);
		ft_strcat(file_path, "resources/sprites/");
		ft_strcat(file_path, file_name);
		ft_strdel(&file_name);
		ft_strcat(file_path, ".bmp");
		bmp_load(app, file_path);
	}

	gamedata_save(app);
	free(app->sprites);
	gamedata_load(app);

	app->sectors_count = 0;
	app->sectors = (t_sector *)malloc(sizeof(t_sector) * MAX_SECTOR);

	double size = 100.0;
	app->skybox.v[0] = new_vector(-size, -size, size);
	app->skybox.v[1] = new_vector(size, size, size);
	app->skybox.v[2] = new_vector(size, -size, size);
	app->skybox.v[3] = new_vector(-size, size, size);
	app->skybox.v[4] = new_vector(-size, -size, -size);
	app->skybox.v[5] = new_vector(size, size, -size);
	app->skybox.v[6] = new_vector(size, -size, -size);
	app->skybox.v[7] = new_vector(-size, size, -size);

//	clock_t begin = clock();
	start_the_game(app);
//	clock_t end = clock();
//	printf("UPDATE LOOP TIME -> %f\n", (double)(end - begin) / CLOCKS_PER_SEC);
	quit_properly();
	return (0);
}

