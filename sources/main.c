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

t_v3d 	get_triangle_normal(t_v3d v0, t_v3d v1, t_v3d v2)
{
	t_v3d	a;
	t_v3d	b;

	a = vector_sub(v1, v0);
	b = vector_sub(v2, v0);
	return (vector_normalise(
			new_vector(a.y * b.z - a.z * b.y,
					   a.z * b.x - a.x * b.z,
					   a.x * b.y - a.y * b.x)));
}

void 	wall_check_collision(t_wall *w, t_v3d *pos, t_v3d *f)
{
	int		collide;
	t_v3d	normal;
	t_v3d	inv_normal;
	t_v3d	tmp_pos;

	collide = is_colliding(vector_sum(*pos, *f), 0.5, w->v[2], w->v[0]);
	if (collide)
	{
		normal = get_triangle_normal(w->v[0], w->v[1], w->v[2]);
		inv_normal = vector_mul_by(normal, -vector_length(*f));
		tmp_pos = vector_sum(*pos, vector_sub(*f, inv_normal));
		*f = vector_sub(tmp_pos, *pos);
	}
}

void 	check_collision(t_app *app, t_v3d *pos, t_v3d f)
{
	int	i;
	int j;

	i = 0;
	while (i < app->sectors_count)
	{
		j = 0;
		while (j < app->sectors[i].walls_count)
			wall_check_collision(&app->sectors[i].walls[j++], pos, &f);
		i++;
	}
	*pos = vector_sum(*pos, f);
}

void 	draw_point(t_app *app, t_v3d p, Uint32 c)
{
	p = matrix_transform(app->camera->transform, p);
	if (vertex_inside(&p))
	{
		p = matrix_transform(app->camera->screen_space, p);
		vertex_perspective_divide(&p);
		int offset = (int)p.y * SCREEN_W + (int)p.x;
		if (p.z < app->depth_buffer[offset])
			set_pixel_uint32(app->sdl->surface, offset, c);
	}
}

t_v3d	save_point(t_app *app, double x, double z)
{
	double angle;

	angle = tan(0.5 * app->camera->fov) * app->camera->pos.y;
	x = (2.0 * x / SCREEN_W - 1) * angle / app->camera->asp_ratio;
	z = (2.0 * z / SCREEN_H - 1) * angle;
	x = round((app->camera->pos.x + x) / app->grid_size) * app->grid_size;
	z = round((app->camera->pos.z - z) / app->grid_size) * app->grid_size;
	return (new_vector(x, 0.0, z));
}

void 	draw_line_3d(t_app *app, t_v3d start, t_v3d end, uint32_t c)
{
	t_v3d		tmp1;
	t_v3d		tmp2;

	tmp1 = matrix_transform(app->camera->transform, start);
	tmp2 = matrix_transform(app->camera->transform, end);
	if (vertex_inside(&tmp1) &&
		vertex_inside(&tmp2))
	{
		tmp1 = matrix_transform(app->camera->screen_space, tmp1);
		vertex_perspective_divide(&tmp1);
		tmp2 = matrix_transform(app->camera->screen_space, tmp2);
		vertex_perspective_divide(&tmp2);
		draw_line(app, &tmp1, &tmp2, c);
	}
}

void 	camera_point_mode(t_v3d *pos, t_v3d *rot)
{
	pos->y = 10.0;
	rot->x = 1.57;
	rot->y = 0.0;
	rot->z = 0.0;
}

void 	camera_live_mode(t_v3d *pos, t_v3d *rot)
{
	rot->x = 0.0;
	rot->y = 0.0;
	rot->z = 0.0;
}

void 	draw_points_sector(t_app *app, t_v3d *p, int size)
{
	int		i;

	if (size == 1)
		draw_point(app, p[0], 0xff00ff);
	else
	{
		i = 0;
		while (i < size - 1)
		{
			draw_line_3d(app, p[i], p[i + 1], 0xffffff);
			i++;
		}
		draw_line_3d(app, p[size - 1], p[0], 0xffffff);
	}
}

void 	draw_points(t_app *app, t_v3d *p, int size)
{
	int		i;

	if (size == 1)
		draw_point(app, p[0], 0xff00ff);
	else
	{
		i = 0;
		while (i < size - 1)
		{
			draw_line_3d(app, p[i], p[i + 1], 0xff00ff);
			i++;
		}
		draw_line_3d(app, p[size - 1], p[0], 0xffff00);
	}
}

void	start_the_game(t_app *app)
{
	SDL_SetRelativeMouseMode(SDL_TRUE);
	app->timer->prev = SDL_GetPerformanceCounter();
	app->hit_wall = NULL;
	app->hit_sector = NULL;
	app->speed = 4.54321;
	app->acc = 0.0;
	app->jump = 0.0;
	app->head_acc = 1.0;
	app->head_power = 0.05;
	app->head_speed = 0.0;
	app->fall = 0.0;
	app->height = 1.0;
	prepare_chunks(app);
	app->point_mode = 1;
	app->cursor_x = SCREEN_W * 0.5;
	app->cursor_y = SCREEN_H * 0.5;
	app->camera->pos.x = 0.0;
	app->camera->pos.z = 0.0;
	camera_point_mode(&app->camera->pos, &app->camera->rot);
	app->points_count = 0;
	ft_bzero(&app->keys, sizeof(uint8_t) * 512);
	ft_bzero(&app->mouse, sizeof(uint8_t) * 6);
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
			if (app->keys[SDL_SCANCODE_T] && app->hit_type == wall)
			{
				decore_new(app->hit_point,
						app->hit_sector,
						app->hit_wall,
						app->camera);
			}
			if (app->inputs->keyboard[SDL_SCANCODE_L])
			{
				if (app->keys[SDL_SCANCODE_MINUS])
					app->hit_sector->l.power -= 0.15;
				else if (app->keys[SDL_SCANCODE_EQUALS])
					app->hit_sector->l.power += 0.15;
				sector_update_shade(app->hit_sector);
				app->hit_sector->l.power = CLAMP(app->hit_sector->l.power, 0.0, 1000.0);
			}
			if (app->inputs->keyboard[SDL_SCANCODE_F])
			{
				if (app->keys[SDL_SCANCODE_MINUS])
					app->hit_sector->floor_y -= 0.5;
				else if (app->keys[SDL_SCANCODE_EQUALS])
					app->hit_sector->floor_y += 0.5;
				sector_update_height(app->hit_sector);
				sector_update_shade(app->hit_sector);
			}
			if (app->inputs->keyboard[SDL_SCANCODE_C])
			{
				if (app->keys[SDL_SCANCODE_MINUS])
					app->hit_sector->ceil_y -= 0.5;
				else if (app->keys[SDL_SCANCODE_EQUALS])
					app->hit_sector->ceil_y += 0.5;
				sector_update_height(app->hit_sector);
				sector_update_shade(app->hit_sector);
			}
		}
		reset_screen(app);
		if (app->point_mode)
		{
			process_points_inputs(app, app->timer->delta);
			if (app->keys[SDL_SCANCODE_G])
				app->grid_size = app->grid_size == 2.0 ? 0.5 : 2.0;
			if (app->keys[SDL_SCANCODE_Q] && app->points_count >= 3)
			{
				sector_close(app, &app->sectors[app->sectors_count]);
				app->point_mode = 0;
				camera_live_mode(&app->camera->pos, &app->camera->rot);
				ft_bzero(&app->keys, sizeof(uint8_t) * 512);
				ft_bzero(&app->mouse, sizeof(uint8_t) * 6);
				continue;
			}
			update_points_camera(app->camera);
			draw_cross(app, (int)app->cursor_x, (int)app->cursor_y, 8, 0xffffff);
			if (app->mouse[SDL_MOUSE_LEFT])
			{
				app->points[app->points_count] = save_point(app, app->cursor_x, app->cursor_y);
				app->points_count++;
			}
			if (app->mouse[SDL_MOUSE_RIGHT] && app->points_count > 0)
				app->points_count--;
			draw_point_mode(app);

			int			i;
			t_sector	*s;

			i = 0;
			while (i < app->sectors_count)
			{
				s = &app->sectors[i];
				draw_points_sector(app, &s->points[0], s->points_count);
				i++;
			}
			if (app->points_count > 0)
				draw_points(app, &app->points[0], app->points_count);

		} else
		{
			process_inputs(app, app->timer->delta);
			if (app->keys[SDL_SCANCODE_R])
			{
				app->camera->fly = !app->camera->fly;
				if (!app->camera->fly)
					app->fall = app->camera->pos.y - app->floor_point.y - app->height;
			}
			update_camera(app, app->camera);
			render_map(app);
			if (app->keys[SDL_SCANCODE_Q] && !app->point_mode)
			{
				app->point_mode = 1;
				app->points_count = 0;
				camera_point_mode(&app->camera->pos, &app->camera->rot);
				ft_bzero(&app->keys, sizeof(uint8_t) * 512);
				ft_bzero(&app->mouse, sizeof(uint8_t) * 6);
				continue;
			}
			draw_cross(app, SCREEN_W / 2, SCREEN_H / 2, 8, 0xffffff);
		}
		update_fps_text(app);
		update_walls_data(app);
		SDL_UpdateWindowSurface(app->sdl->window);
		ft_bzero(&app->keys, sizeof(uint8_t) * 512);
		ft_bzero(&app->mouse, sizeof(uint8_t) * 6);
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

	app->sectors_count = 0;
	app->sectors = (t_sector *)malloc(sizeof(t_sector) * 1000);

//	t_sector *cs;
//
//	cs = &app->sectors[0];
//	cs->floor_y = 0.0;
//	cs->ceil_y = 2.0;
//	cs->delta_y = 2.0;
//
//	cs->walls_count = 0;
//	cs->walls = (t_wall *)malloc(sizeof(t_wall) * 1000);
//	cs->walls[0] = wall_new();
//
//	cs->walls[0].v[0] = new_vector(0.0, 0.0, 0.0);
//	cs->walls[0].v[1] = new_vector(2.0, 2.0, 0.0);
//	cs->walls[0].v[2] = new_vector(2.0, 0.0, 0.0);
//	cs->walls[0].v[3] = new_vector(0.0, 2.0, 0.0);
//	wall_reset_tex(&cs->walls[0]);
//	wall_update_scale(&cs->walls[0]);
//	cs->walls_count++;
//
//	cs->objs_count = 0;
//	cs->objs = (t_wall *)malloc(sizeof(t_wall) * 1000);
//	cs->objs[0] = wall_new();
//	cs->objs[0].size = 1.5;
//	cs->objs[0].pos = new_vector(1.0, cs->walls[0].v[0].y, -4.0);
//	wall_reset_tex(&cs->objs[0]);
//	cs->objs[0].sprite = 499;
//	cs->objs_count++;
//
//	cs->decor = (t_wall *)malloc(sizeof(t_wall) * 1000);
//	cs->decor_count = 0;
//
//	cs->l.power = 5;
//	app->sectors[0].ready = 0;

//	clock_t begin = clock();
	start_the_game(app);
//	clock_t end = clock();
//	printf("UPDATE LOOP TIME -> %f\n", (double)(end - begin) / CLOCKS_PER_SEC);
	quit_properly();
	return (0);
}

