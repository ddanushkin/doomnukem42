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

void 	decore_move(t_wall *decor, t_v3d forward)
{
	decor->v[0].y -= DECOR_LEN_HALF;
	decor->v[1].y += DECOR_LEN_HALF;
	decor->v[2].y -= DECOR_LEN_HALF;
	decor->v[3].y += DECOR_LEN_HALF;
	move(&decor->v[0], forward, -0.01);
	move(&decor->v[1], forward, -0.01);
	move(&decor->v[2], forward, -0.01);
	move(&decor->v[3], forward, -0.01);
}

void 	decore_new(t_v3d hit_p, t_sector *cs, t_wall *hit_w, t_camera *cam)
{
	t_wall	*decor;
	double	dz;
	double	dx;
	double	nz;
	double	nx;

	decor = &cs->decor[cs->decor_count];
	*decor = wall_new();
	dz = hit_w->v[1].z - hit_w->v[0].z;
	dx = hit_w->v[1].x - hit_w->v[0].x;
	nz = ((hit_p.z / DECOR_LEN) + dz / sqrt(dx*dx+dz*dz)) * DECOR_LEN;
	nx = ((hit_p.x / DECOR_LEN) + dx / sqrt(dx*dx+dz*dz)) * DECOR_LEN;
	decor->v[0] = hit_p;
	decor->v[1] = new_vector(nx, hit_p.y, nz);
	decor->v[2] = decor->v[1];
	decor->v[3] = decor->v[0];
	decore_move(decor, cam->forward);
	wall_reset_tex(decor);
	decor->sprite = 323;
	cs->decor_count++;
	sector_update_shade(cs);
}

int is_colliding(t_v3d c0, double radius, t_v3d v0, t_v3d v1)
{
	double	dist;
	double	u;
	t_v3d	v1v0;
	t_v3d	c0v0;
	t_v3d	tmp;

	v1v0 = vector_sub(v1, v0);
	c0v0 = vector_sub(c0, v0);
	u = (c0v0.x * v1v0.x + c0v0.z * v1v0.z) /
		(v1v0.z * v1v0.z + v1v0.x * v1v0.x);
	if (u >= 0.0 && u <= 1.0)
	{
		dist = (v0.x + v1v0.x * u - c0.x) * (v0.x + v1v0.x * u - c0.x) +
			   (v0.z + v1v0.z * u - c0.z) * (v0.z + v1v0.z * u - c0.z);
	} else
	{
		tmp = u < 0.0 ? vector_sub(v0, c0) : vector_sub(v1, c0);
		dist = (tmp.x * tmp.x) + (tmp.z * tmp.z);
	}
	return (dist < (radius * radius));
}

void 	wall_check_collision(t_app *app, t_wall *w)
{
	t_v3d	*new;
	t_v3d	*old;

	new = &app->camera->pos;
	old = &app->camera->pos_old;
	app->collide_x = is_colliding(new_vector(new->x, 0.0, old->z), 0.5, w->v[2], w->v[0]);
	if (app->collide_x)
		new->x = old->x;
	app->collide_z = is_colliding(new_vector(new->x, 0.0, new->z), 0.5, w->v[2], w->v[0]);
	if (app->collide_z)
		new->z = old->z;
}

void 	check_collision(t_app *app)
{
	int	i;
	int j;

	i = 0;
	while (i < app->sectors_count)
	{
		j = 0;
		while (j < app->sectors[i].walls_count)
			wall_check_collision(app, &app->sectors[i].walls[j++]);
		i++;
	}
}

void	start_the_game(t_app *app)
{
	SDL_SetRelativeMouseMode(SDL_TRUE);
	app->timer->prev = SDL_GetPerformanceCounter();
	app->camera->pos.z += 9.5;
	app->hit_wall = NULL;
	app->hit_sector = NULL;
	app->cs = &app->sectors[0];
	app->height = 1.0;
	app->camera->pos.y = app->cs->floor_y + app->height;
	app->speed = 4.54321;
	app->acc = 0.0;
	app->jump = 0.0;
	prepare_chunks(app);
	while (1)
	{
		if (!event_handling(app))
			break;
		if (app->inputs->keyboard[SDL_SCANCODE_ESCAPE])
			break ;
		if (app->hit_wall && app->hit_sector->ready)
		{
			if (app->inputs->keyboard[SDL_SCANCODE_LSHIFT])
				texture_change(app);
			if (app->inputs->keyboard[SDL_SCANCODE_LALT])
				texture_scale_y_change(app);
			if (app->inputs->keyboard[SDL_SCANCODE_LCTRL])
				texture_scale_x_change(app);
			if (app->input_t && app->hit_type == wall)
			{
				decore_new(app->hit_point,
						app->hit_sector,
						app->hit_wall,
						app->camera);
				app->input_t = 0;
			}
			if (app->inputs->keyboard[SDL_SCANCODE_L])
			{
				if (app->input_minus)
				{
					app->input_minus = 0;
					app->hit_sector->l.power -= 0.15;
				}
				else if (app->input_plus)
				{
					app->input_plus = 0;
					app->hit_sector->l.power += 0.15;
				}
				sector_update_shade(app->hit_sector);
				app->hit_sector->l.power = CLAMP(app->hit_sector->l.power, 0.0, 1000.0);
			}
			if (app->inputs->keyboard[SDL_SCANCODE_F])
			{
				if (app->input_minus)
				{
					app->input_minus = 0;
					app->hit_sector->floor_y -= 0.5;
//					app->camera->pos.y -= 0.5;
				}
				else if (app->input_plus)
				{
					app->input_plus = 0;
					app->hit_sector->floor_y += 0.5;
//					app->camera->pos.y += 0.5;
				}
				sector_update_height(app->hit_sector);
				sector_update_shade(app->hit_sector);
			}
			if (app->inputs->keyboard[SDL_SCANCODE_C])
			{
				if (app->input_minus)
				{
					app->input_minus = 0;
					app->hit_sector->ceil_y -= 0.5;
				}
				else if (app->input_plus)
				{
					app->input_plus = 0;
					app->hit_sector->ceil_y += 0.5;
				}
				sector_update_height(app->hit_sector);
				sector_update_shade(app->hit_sector);
			}
		}
		reset_screen(app);
		process_inputs(app, app->timer->delta);
		if (app->input_r)
		{
			app->camera->fly = !app->camera->fly;
			app->input_r = 0;
		}
		if (!app->camera->fly && app->cs->ready)
			check_collision(app);
		if (!app->camera->fly && fabs(app->camera->pos.y - app->cs->floor_y) > 1.0)
			app->camera->pos.y -= 9.8 * app->timer->delta;
		if (!app->camera->fly && app->camera->pos.y < app->cs->floor_y)
			app->camera->pos.y = app->cs->floor_y + app->height;
		update_camera(app->camera);
		render_map(app);
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
		draw_cross(app, SCREEN_W / 2, SCREEN_H / 2, 8, 0xffffff);
		update_fps_text(app);
		update_walls_data(app);
		SDL_UpdateWindowSurface(app->sdl->window);
		get_delta_time(app->timer);
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
	app->camera->up = new_vector(0.0, 1.0, 0.0);
	app->depth_buffer = (double *)malloc(sizeof(double) * SCREEN_W * SCREEN_H);
	init_app(app);

	/* TODO: Set number of meshes */
//	int number_of_meshes = 2;
//	app->meshes = (t_mesh *)malloc(sizeof(t_mesh) * number_of_meshes);
//	obj_load("resources/plane.obj", &app->meshes[0]);
//	obj_load("resources/cube.obj", &app->meshes[1]);

	/* TODO: Set number of meshes */
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

	double size = 100.0;
	app->skybox.v[0] = new_vector(-size, -size, size);
	app->skybox.v[1] = new_vector(size, size, size);
	app->skybox.v[2] = new_vector(size, -size, size);
	app->skybox.v[3] = new_vector(-size, size, size);
	app->skybox.v[4] = new_vector(-size, -size, -size);
	app->skybox.v[5] = new_vector(size, size, -size);
	app->skybox.v[6] = new_vector(size, -size, -size);
	app->skybox.v[7] = new_vector(-size, size, -size);

	app->sectors_count = 1;
	app->sectors = (t_sector *)malloc(sizeof(t_sector) * 1000);

	t_sector *cs;

	cs = &app->sectors[0];
	cs->floor_y = 0.0;
	cs->ceil_y = 2.0;
	cs->delta_y = 2.0;

	cs->walls_count = 0;
	cs->walls = (t_wall *)malloc(sizeof(t_wall) * 1000);
	cs->walls[0] = wall_new();

//	cs->walls[0].v[0] = new_vector(-1.878427, 0.795421, -1.137056);
//	cs->walls[0].v[1] = new_vector(-0.628427, 1.295421, -0.387056);
//	cs->walls[0].v[2] = new_vector(-0.628427, 0.795421, -1.137056);
//	cs->walls[0].v[3] = new_vector(-1.878427, 1.295421, -0.387056);

	cs->walls[0].v[0] = new_vector(0.0, 0.0, 0.0);
	cs->walls[0].v[1] = new_vector(2.0, 2.0, 0.0);
	cs->walls[0].v[2] = new_vector(2.0, 0.0, 0.0);
	cs->walls[0].v[3] = new_vector(0.0, 2.0, 0.0);
	wall_reset_tex(&cs->walls[0]);
	wall_update_scale(&cs->walls[0]);
	cs->walls_count++;

	cs->objs_count = 0;
	cs->objs = (t_wall *)malloc(sizeof(t_wall) * 1000);
	cs->objs[0] = wall_new();
	cs->objs[0].size = 1.5;
	cs->objs[0].pos = new_vector(1.0, cs->walls[0].v[0].y, -4.0);
	wall_reset_tex(&cs->objs[0]);
	cs->objs[0].sprite = 499;
	cs->objs_count++;

	cs->decor = (t_wall *)malloc(sizeof(t_wall) * 1000);
	cs->decor_count = 0;

	app->sectors[0].ready = 0;

//	clock_t begin = clock();
	start_the_game(app);
//	clock_t end = clock();
//	double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
//	printf("UPDATE LOOP TIME -> %f\n", time_spent);
	quit_properly();
	return (0);
}

