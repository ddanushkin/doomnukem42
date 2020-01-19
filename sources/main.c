#include "doom_nukem.h"

void	reset_screen(t_app *app)
{
	int		i;
	int		len;
	Uint32	*screen;
	double	*depth;

	i = 0;
	len = SCREEN_H * SCREEN_W;
	depth = app->depth_buffer;
	screen = app->sdl->surface->pixels;
	while (i < len)
	{
		depth[i] = 999999.0;
		screen[i++] = 0;
	}
}

int 	vertex_inside(t_v3d *v)
{
	return (fabs(v->x) <= fabs(v->w) &&
			fabs(v->y) <= fabs(v->w) &&
			fabs(v->z) <= fabs(v->w));
}

void	start_the_game(t_app *app)
{
	SDL_SetRelativeMouseMode(SDL_TRUE);
	app->timer->prev = SDL_GetPerformanceCounter();
	app->prev_pos = app->camera->pos;
	while (1)
	{
		if (!event_handling(app))
			break;
		get_delta_time(app->timer);
		if (app->inputs->keyboard[SDL_SCANCODE_ESCAPE])
			break ;
		if (app->hit_wall)
		{
			if (app->inputs->keyboard[SDL_SCANCODE_LSHIFT])
				texture_change(app);
			if (app->inputs->keyboard[SDL_SCANCODE_LALT])
				texture_scale_y_change(app);
			if (app->inputs->keyboard[SDL_SCANCODE_LCTRL])
				texture_scale_x_change(app);
		}
		reset_screen(app);
		process_inputs(app, app->timer->delta);
		render_map(app);
		app->prev_pos.y = app->camera->pos.y;
		if (!app->collide_x)
			app->prev_pos.x = app->camera->pos.x;
		if (!app->collide_z)
			app->prev_pos.z = app->camera->pos.z;
		if (app->collide_x)
		{
			app->camera->pos.x = app->prev_pos.x;
			app->collide_x = 0;
		}
		if (app->collide_z)
		{
			app->camera->pos.z = app->prev_pos.z;
			app->collide_z = 0;
		}
		update_camera(app->camera);
		if (app->hit_wall && !app->edge_selected)
			show_edge(app);
		if (app->edge_selected)
		{
			if (app->input_g)
			{
				app->grid_size = app->grid_size == 2.0 ? 0.5 : 2.0;
				app->input_g = 0;
			}
			draw_new_wall(app);
			if (app->inputs->mouse.right)
			{
				app->edge_selected = 0;
				app->inputs->mouse.right = 0;
			}
			if (app->edge_selected && app->inputs->mouse.left)
				save_new_wall(app);
		}
		if (!app->edge_selected)
			sector_close(app);
		draw_cross(app, (int)app->sdl->half_width, (int)app->sdl->half_height, 8, 0xffffff);
		update_fps_text(app);
		update_walls_data(app);
		SDL_UpdateWindowSurface(app->sdl->window);
//		if (app->timer->frame == 1000)
//			break;
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

int		main(int argv, char**argc)
{
	t_app	*app;

	//getchar();
	//if (!check_resources())
	//	exit_with_status(STATUS_BAD_RESOURCES, NULL);
	app = (t_app *)malloc(sizeof(t_app));
	app->sdl = (t_sdl *)malloc(sizeof(t_sdl));
	app->inputs = (t_inputs *)malloc(sizeof(t_inputs));
	app->timer = (t_timer *)malloc(sizeof(t_timer));
	app->camera = (t_camera *)malloc(sizeof(t_camera));
	app->depth_buffer = (double *)malloc(sizeof(double) * SCREEN_W * SCREEN_H);
	init_app(app);

	/* TODO: Set number of meshes */
	int number_of_meshes = 2;
	app->meshes = (t_mesh *)malloc(sizeof(t_mesh) * number_of_meshes);
	obj_load("resources/plane.obj", &app->meshes[0]);
	obj_load("resources/cube.obj", &app->meshes[1]);

	/* TODO: Set number of meshes */
	app->sprites_count = 0;
	app->sprites = (t_sprite *)malloc(sizeof(t_sprite) * 493);
	for (int i = 0; i < 493; i++)
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

	app->sectors_count = 1;
	app->sectors = (t_sector *)malloc(sizeof(t_sector) * 1000);
	app->sectors[0].walls_count = 0;
	app->sectors[0].walls = (t_wall *)malloc(sizeof(t_wall) * 1000);

	app->sectors[0].walls[0] = wall_new();
	app->sectors[0].walls[0].v[0] = new_vector(0.0, 0.0, 0.0);
	app->sectors[0].walls[0].v[1] = new_vector(2.0, 2.0, 0.0);
	app->sectors[0].walls[0].v[2] = new_vector(2.0, 0.0, 0.0);
	app->sectors[0].walls[0].v[3] = new_vector(0.0, 2.0, 0.0);
	wall_reset_tex(&app->sectors[0].walls[0]);
	app->sectors[0].walls[0].billboard = 0;
	app->sectors[0].walls_count++;

	app->sectors[0].walls[1] = wall_new();
	app->sectors[0].walls[1].v[0] = new_vector(2.0, 0.0, 0.0);
	app->sectors[0].walls[1].v[1] = new_vector(4.0, 2.0, 0.0);
	app->sectors[0].walls[1].v[2] = new_vector(4.0, 0.0, 0.0);
	app->sectors[0].walls[1].v[3] = new_vector(2.0, 2.0, 0.0);
	wall_reset_tex(&app->sectors[0].walls[1]);
//	app->sectors[0].walls_count++;

	app->sectors[0].ready = 0;

	clock_t begin = clock();
	start_the_game(app);
	clock_t end = clock();
	double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
	printf("%f\n", time_spent);
	quit_properly();
	return (0);
}

