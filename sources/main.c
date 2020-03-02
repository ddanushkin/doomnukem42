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
	app->speed = PLAYER_SPEED;
	app->acc = 0.0;
	app->y_vel = 0.0;
	app->y_acc = 0.0;
	app->ground = 1;
	app->height = PLAYER_HEIGHT;
	app->falling = 0.0;
	app->jumped = 0;
	app->cursor_x = SCREEN_W * 0.5;
	app->cursor_y = SCREEN_H * 0.5;
	app->camera->pos.x = 0.0;
	app->camera->pos.z = 0.0;
	app->points_count = 0;
	app->point_mode = 0;
	app->bad_close = 0.0;
	app->bflag = 0;
	app->bclr[0] = 0xff0000;
	app->bclr[1] = 0x00ff00;
	app->si = 0;
	app->prev_dy = app->height;

	prepare_chunks(app);
	switch_mode(app);

	t_animation a;

	a.speed = 1.5;
	a.frame_nbr = 5;
	a.loop = 0;
	a.delayed = 0;
	a.play = 0;

	SDL_SetRelativeMouseMode(SDL_TRUE);
	//Mix_PlayMusic(app->bg[5], -1);
	app->a = &a;
	while (1)
	{
		get_delta_time(app->timer);
		animation_next_frame(&a, app->timer->delta);
		ft_bzero(&app->keys, sizeof(uint8_t) * 512);
		ft_bzero(&app->mouse, sizeof(uint8_t) * 6);
		if (!event_handling(app))
			break;
		if (app->keys[SDL_SCANCODE_P] && switch_mode(app))
			continue ;
		if (app->inputs->keyboard[SDL_SCANCODE_ESCAPE])
			break ;

		if (app->keys[SDL_SCANCODE_1])
		{
			Mix_PlayChannel(-1, app->sfx[0], 0);
			printf("%s\n", SDL_GetError());
		}
		if (app->keys[SDL_SCANCODE_2])
		{
			Mix_PlayChannel(-1, app->sfx[1], 0);
			printf("%s\n", SDL_GetError());
		}
		if (app->keys[SDL_SCANCODE_3])
			Mix_PlayMusic(app->bg[0], 0);
		if (app->keys[SDL_SCANCODE_4])
			Mix_PlayMusic(app->bg[1], 0);

		if (app->keys[SDL_SCANCODE_F5])
			app->si--;
		if (app->keys[SDL_SCANCODE_F6])
			app->si++;
		if (app->si < 0)
			app->si = MAX_SFX - 1;
		if (app->si >= MAX_SFX)
			app->si = 0;

		if (app->point_mode)
		{
			point_mode_inputs(app);
			update_points_camera(app->camera);
			process_points_inputs(app, app->timer->delta);
			draw_cross(app, (int)app->cursor_x, (int)app->cursor_y, 8, 0xffffff);
			draw_sectors(app);
			if (app->points_count > 3)
				points_add_check(&app->points[0], &app->points_count);
			if (app->points_count > 0)
				draw_points(app, &app->points[0], app->points_count);
			draw_point_mode(app);
			draw_exit(app);
			draw_start(app);
			SDL_UpdateWindowSurface(app->sdl->window);
			reset_screen(app);
		}
		else
		{
			if (!a.play)
				animation_play(&a);
			live_mode_inputs(app);
			update_camera(app, app->camera);
			process_inputs(app, app->timer->delta);
			render_map(app);
			draw_cross(app, SCREEN_W / 2, SCREEN_H / 2, 8, 0xffffff);
			SDL_UpdateWindowSurface(app->sdl->window);
			reset_screen(app);
		}
	}
	TTF_CloseFont(app->font);
	TTF_Quit();
	SDL_Quit();
	SDL_DestroyWindow(app->sdl->window);
}

/* TODO: rewrite for GAME_DATA*/
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

void 	gamedata_write(int fd, void *mem, size_t size, char *type)
{
	char	info[50];

	info[0] = '\0';
	ft_strcat(&info[0], type);
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
	data = open("GAME_DATA", O_RDWR | O_CREAT | O_TRUNC, 777);
#else
	data = open("GAME_DATA", O_BINARY | O_WRONLY | O_CREAT | O_TRUNC, 655);
#endif
	if (data == -1)
		return ;
	gamedata_write(data, &a->sprites[0], sizeof(t_sprite) * MAX_SPRITE, "T:\0");
	gamedata_write(data, &a->audio[0], sizeof(t_raw_sfx) * MAX_SFX, "A:\0");
	gamedata_write(data, &a->music[0], sizeof(t_raw_bg) * MAX_BG, "B:\0");
	gamedata_write(data, &a->fonts, sizeof(t_raw_font), "F:\0");
	close(data);
}

void	map_save(t_app *a, char *name)
{
	int		data;

#ifdef __APPLE__
	data = open(name, O_RDWR | O_CREAT | O_TRUNC, 777);
#else
	data = open(name, O_BINARY | O_WRONLY | O_CREAT | O_TRUNC, 655);
#endif
	if (data == -1)
		return ;
	gamedata_write(data, &a->sectors[0], sizeof(t_sector) * a->sectors_count, "S:\0");
	gamedata_write(data, &a->md, sizeof(t_map_data), "D:\0");
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
		a->sectors_count = (int)(size / sizeof(t_sector));
		a->sectors = malloc(sizeof(t_sector) * MAX_SECTOR);
		read(fd, a->sectors, size);
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
	else if (type == 'D')
		read(fd, &a->md, size);
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
}

void	gamedata_load(t_app *a)
{
	int		data;
	char	info[50];
	char 	buff[2];
	int 	sep;

#ifdef __APPLE__
	data = open("GAME_DATA", O_RDONLY);
#else
	data = open("GAME_DATA", O_BINARY | O_RDONLY);
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
			info[0] = '\0';
		}
	}
	close(data);
}

void	map_load(t_app *a, char *name)
{
	int		data;
	char	info[50];
	char 	buff[2];
	int 	sep;

#ifdef __APPLE__
	data = open(name, O_RDONLY);
#else
	data = open(name, O_BINARY | O_RDONLY);
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
			info[0] = '\0';
		}
	}
	close(data);
}

void 	init_game_data(t_app *app)
{
	SDL_RWops	*raw = SDL_RWFromFile("resources/calibri.ttf", "rb");
	size_t size = SDL_RWsize(raw);
	SDL_RWread(raw, &app->fonts.mem[0], size, 1);
	app->fonts.size = size;
	SDL_RWclose(raw);

	app->audio = (t_raw_sfx *)malloc(sizeof(t_raw_sfx) * MAX_SFX);
	for (int i = 0; i < MAX_SFX; i++)
	{
		char	*file_name;
		char	file_path[100];
		file_path[0] = '\0';
		file_name = ft_itoa(i);
		ft_strcat(file_path, "resources/audio/");
		ft_strcat(file_path, file_name);
		ft_strdel(&file_name);
		ft_strcat(file_path, ".wav");
		raw = SDL_RWFromFile(file_path, "rb");
		size = SDL_RWsize(raw);
		SDL_RWread(raw, &app->audio[i].mem[0], size, 1);
		app->audio[i].size = size;
		SDL_RWclose(raw);
	}

	app->music = (t_raw_bg *)malloc(sizeof(t_raw_bg) * MAX_BG);
	for (int i = 0; i < MAX_BG; i++)
	{
		char	*file_name;
		char	file_path[100];
		file_path[0] = '\0';
		file_name = ft_itoa(i);
		ft_strcat(file_path, "resources/music/");
		ft_strcat(file_path, file_name);
		ft_strdel(&file_name);
		ft_strcat(file_path, ".ogg");
		raw = SDL_RWFromFile(file_path, "rb");
		size = SDL_RWsize(raw);
		printf("%zu\n", size);
		SDL_RWread(raw, &app->music[i].mem[0], size, 1);
		app->music[i].size = size;
		SDL_RWclose(raw);
	}

	app->sprites_count = 0;
	app->sprites = (t_sprite *)malloc(sizeof(t_sprite) * MAX_SPRITE);
	for (int i = 0; i < MAX_SPRITE; i++)
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
}

void 	init_map(t_app *app)
{
	app->sectors_count = 0;
	app->md.start_set = 0;
	app->sectors = (t_sector *)malloc(sizeof(t_sector) * MAX_SECTOR);
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
	app->game_data_init = 0;
	app->map_init = 0;
	if (!app->game_data_init)
		gamedata_load(app);
	if (!app->map_init)
		map_load(app, "test_map");
	if (app->game_data_init)
		init_game_data(app);
	if (app->map_init)
		init_map(app);

	init_app(app);
	start_the_game(app);

	gamedata_save(app);
	map_save(app, "test_map");

	quit_properly();
	return (0);
}
