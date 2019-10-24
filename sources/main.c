#include "doom_nukem.h"

void	clear_screen(t_app *app)
{
	int len;
	len = app->sdl->width * app->sdl->height * 4;
	image_clear(app->sdl->surface->pixels, 200, len);
}

void	clear_z_buffer(t_app *app)
{
	int len;
	len = app->sdl->width * app->sdl->height;
	image_clear(app->z_buf, 0, len);
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
		clear_z_buffer(app);
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
		while (i < 1)
		{
			app->meshes[i].pos = new_vector(-0.5, -0.5, -0.5);
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
	app->z_buf = (double *)malloc(sizeof(double) * SCREEN_W * SCREEN_H);
	init_app(app);

	/* TODO: Set number of meshes */
	int number_of_meshes = 2;
	app->meshes = (t_mesh *)malloc(sizeof(t_mesh) * number_of_meshes);
	obj_load("resources/plane.obj", &app->meshes[0]);
	obj_load("resources/cube.obj", &app->meshes[1]);

	/* TODO: Set number of meshes */
	int number_of_sprites = 1;
	app->sprites = (t_sprite *)malloc(sizeof(t_sprite) * number_of_sprites);
	bmp_load("resources/FLAG.bmp", &app->sprites[0]);

	start_the_game(app);
	quit_properly(app);
	return (0);
}

