#include "doom_nukem.h"

void	clear_screen(t_app *app)
{
	int len;

	len = app->sdl->width * app->sdl->height * 4;
	image_clear(app->sdl->surface->pixels, 200, len);
}


static void capFrameRate(t_app *app, long *then, double *remainder)
{
	long wait, frameTime;

	wait = 16.0 + *remainder;
	*remainder -= (int)*remainder;
	frameTime = SDL_GetTicks() - *then;
	wait -= frameTime;
	if (wait < 1)
		wait = 1;
	SDL_Delay(wait);
	*remainder += 0.667;
	*then = SDL_GetTicks();
	app->timer->delta = 0.01;
}

t_color	sprite_get_color(t_sprite *s, int x, int y)
{
	size_t	offset;
	t_color	c;

	offset = 3 * (y * s->header.width_px + x);
	c.r = s->pixels[offset];
	c.g = s->pixels[offset + 1];
	c.b = s->pixels[offset + 2];
	return (c);
}

void	start_the_game(t_app *app)
{
	SDL_SetRelativeMouseMode(SDL_TRUE);

	app->camera->pos = new_vector(0.0, 25.0, -120.0);
	while (1)
	{
		get_ticks(app->timer);
		mouse_update(app);
		if (!event_handling(app))
			break;
		clear_screen(app);

		int		x;
		int		y;
		t_color	c;

		x = 0;
		while (x < app->sprites->header.width_px)
		{
			y = 0;
			while (y < app->sprites->header.height_px)
			{
				//T_COLOR TO UNIT32: t_color.r << 16 | t_color.g << 8 | t_color.b;
				c = sprite_get_color(&app->sprites[0], x, y);
				set_pixel(app->sdl->surface, x, y, &c);
				y++;
			}
			x++;
		}
		SDL_UpdateWindowSurface(app->sdl->window);
		continue;

		/* Animate world rotation */
		//app->world.rot.x += 1.0 * app->timer->delta;
		//app->world.rot.y += 1.0 * app->timer->delta;
		//app->world.rot.z += 1.0 * app->timer->delta;

		/* Create world rotation matrices */
		app->world->rot_mat_x = rotation_mat_x(app->world->rot.x);
		app->world->rot_mat_y = rotation_mat_y(app->world->rot.y);
		app->world->rot_mat_z = rotation_mat_z(app->world->rot.z);

		/* Create world translation matrix */
		app->world->trans = new_vector(0.0, 0.0, 5.0);
		app->world->trans_mat = init_translation_mat(app->world->trans);

		/* Create world matrix */
		app->world->mat = matrix_identity();
		app->world->mat = matrix_multiply_matrix(app->world->mat, app->world->rot_mat_x);
		app->world->mat = matrix_multiply_matrix(app->world->mat, app->world->rot_mat_y);
		app->world->mat = matrix_multiply_matrix(app->world->mat, app->world->rot_mat_z);
		app->world->mat = matrix_multiply_matrix(app->world->mat, app->world->trans_mat);

		/* Create camera rotation matrices */
		app->camera->rot_mat_x = rotation_mat_x(app->camera->rot.x);
		app->camera->rot_mat_y = rotation_mat_y(app->camera->rot.y);
		app->camera->rot_mat_z = rotation_mat_z(app->camera->rot.z);

		/* Create camera rotation matrix */
		app->camera->rot_mat = matrix_identity();
		app->camera->rot_mat = matrix_multiply_matrix(app->camera->rot_mat, app->camera->rot_mat_x);
		app->camera->rot_mat = matrix_multiply_matrix(app->camera->rot_mat, app->camera->rot_mat_y);
		app->camera->rot_mat = matrix_multiply_matrix(app->camera->rot_mat, app->camera->rot_mat_z);

		/* Create camera view matrix */
		app->camera->target = new_vector(0.0, 0.0, 1.0);
		app->camera->dir = matrix_multiply_vector(app->camera->rot_mat, app->camera->target);
		app->camera->target = vector_sum(app->camera->pos, app->camera->dir);
		app->camera->view_mat = matrix_look_at(app->camera->pos, app->camera->target);
		app->camera->view_mat = matrix_inverse(app->camera->view_mat);

		/* Animate meshes[0] rotation */
		//app->meshes[0].rot.x += 1.0 * app->timer->delta;
		//app->meshes[0].rot.y += 1.0 * app->timer->delta;
		//app->meshes[0].rot.z += 1.0 * app->timer->delta;

		/* Animate meshes[0] position */
		//app->meshes[0].pos.x = sin(app->timer->time) * 2.0;
		//app->meshes[0].pos.y = sin(app->timer->time) * 2.0;
		//app->meshes[0].pos.z = sin(app->timer->time) * 2.0;

		int i = 0;
		while (i < 2)
		{
			app->meshes[i].pos = new_vector(-0.5, -0.5, -0.5);
			if (i == 0)
			{
				app->meshes[i].rot.x += 1.0 * app->timer->delta;
				app->meshes[i].pos.x = sin(app->timer->time) * 2.0;
			}
			app->meshes[i].rot_mat_x = rotation_mat_x(app->meshes[i].rot.x);
			app->meshes[i].rot_mat_y = rotation_mat_y(app->meshes[i].rot.y);
			app->meshes[i].rot_mat_z = rotation_mat_z(app->meshes[i].rot.z);
			app->meshes[i].trans_mat = init_translation_mat(app->meshes[i].pos);
			app->meshes[i].transform = matrix_identity();
			app->meshes[i].transform = matrix_multiply_matrix(app->meshes[i].transform, app->meshes[i].rot_mat_x);
			app->meshes[i].transform = matrix_multiply_matrix(app->meshes[i].transform, app->meshes[i].rot_mat_y);
			app->meshes[i].transform = matrix_multiply_matrix(app->meshes[i].transform, app->meshes[i].rot_mat_z);
			app->meshes[i].transform = matrix_multiply_matrix(app->meshes[i].transform, app->meshes[i].trans_mat);
			transform_vertices(app, i);
			assemble_triangles(app, i);
			check_triangles(app, i);
			i++;
		}
		draw_cross(app, 7.0, 255, 0, 200);
		SDL_UpdateWindowSurface(app->sdl->window);
		get_delta_time(app->timer);
	}
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

void	bpm_read_pixels(int fd, t_bmp_header h, t_sprite *sprite)
{
	size_t	len;
	uint8_t	*tmp_read;
	size_t	i;
	size_t	j;

	len = h.width_px * h.height_px * 3;
	sprite->header = h;
	sprite->pixels = (uint8_t *)malloc(len * sizeof(uint8_t));
	tmp_read = (uint8_t *)malloc(len * sizeof(uint8_t));
	read(fd, tmp_read, len * sizeof(uint8_t));
	i = 0;
	j = len - 1;
	while (i < len)
		sprite->pixels[i++] = tmp_read[j--];
	free(tmp_read);

	int	row;

	row = 0;
	while (row < h.height_px)
	{
		int x_start;
		int x_end;
		int end_half;

		x_start = row * h.width_px * 3;
		x_end = x_start + h.width_px * 3 - 1;
		end_half = (x_end - x_start) / 2;
		while (x_start < x_start + end_half)
		{
			t_color sc;
			t_color ec;

			sc = color_new(
					sprite->pixels[x_start],
					sprite->pixels[x_start + 1],
					sprite->pixels[x_start + 2]);
			ec = color_new(
					sprite->pixels[x_end - 2],
					sprite->pixels[x_end - 1],
					sprite->pixels[x_end]);
			sprite->pixels[x_start] = ec.r;
			sprite->pixels[x_start + 1] = ec.g;
			sprite->pixels[x_start + 2] = ec.b;
			sprite->pixels[x_end - 2] = sc.r;
			sprite->pixels[x_end - 1] = sc.g;
			sprite->pixels[x_end] = sc.b;
			x_start += 3;
			end_half -= 3;
		}
		row++;
	}
}

t_bmp_header	bmp_read_header(int fd)
{
	t_bmp_header	h;

	ft_memset(&h, 0, sizeof(h));
	read(fd, &(h.type), sizeof(h.type));
	read(fd, &(h.size), sizeof(h.size));
	read(fd, &(h.reserved1), sizeof(h.reserved1));
	read(fd, &(h.reserved2), sizeof(h.reserved2));
	read(fd, &(h.offset), sizeof(h.offset));
	read(fd, &(h.dib_header_size), sizeof(h.dib_header_size));
	read(fd, &(h.width_px), sizeof(h.width_px));
	read(fd, &(h.height_px), sizeof(h.height_px));
	read(fd, &(h.num_planes), sizeof(h.num_planes));
	read(fd, &(h.bits_per_pixel), sizeof(h.bits_per_pixel));
	read(fd, &(h.compression), sizeof(h.compression));
	read(fd, &(h.image_size_bytes), sizeof(h.image_size_bytes));
	read(fd, &(h.x_resolution_ppm), sizeof(h.x_resolution_ppm));
	read(fd, &(h.y_resolution_ppm), sizeof(h.y_resolution_ppm));
	read(fd, &(h.num_colors), sizeof(h.num_colors));
	read(fd, &(h.important_colors), sizeof(h.important_colors));
	return (h);
}

void	bmp_load(char *path, t_sprite *sprite)
{
	int				fd;

	t_bmp_header	header;
	fd = open(path, O_RDONLY);
	if (fd < 3)
		exit_with_status(STATUS_FILE_NOT_FOUND, path);
	header = bmp_read_header(fd);
	bpm_read_pixels(fd, header, sprite);
	close(fd);
}

int		main(int argv, char**argc)
{
	t_app	*app;

	//if (!check_resources())
	//	exit_with_status(STATUS_BAD_RESOURCES, NULL);
	app = (t_app *)malloc(sizeof(t_app));
	app->sdl = (t_sdl *)malloc(sizeof(t_sdl));
	app->inputs = (t_inputs *)malloc(sizeof(t_inputs));
	app->timer = (t_timer *)malloc(sizeof(t_timer));
	app->world = (t_world *)malloc(sizeof(t_world));
	app->camera = (t_camera *)malloc(sizeof(t_camera));

	init_app(app);

	/* TODO: Set number of meshes */
	int number_of_meshes = 2;
	app->meshes = (t_mesh *)malloc(sizeof(t_mesh) * number_of_meshes);
	obj_reader("resources/plane.obj", &app->meshes[0]);
	obj_reader("resources/cube.obj", &app->meshes[1]);

	/* TODO: Set number of meshes */
	int number_of_sprites = 1;
	app->sprites = (t_sprite *)malloc(sizeof(t_sprite) * number_of_sprites);
	bmp_load("resources/FLAG.bmp", &app->sprites[0]);

	start_the_game(app);
	quit_properly(app);
	return (0);
}

