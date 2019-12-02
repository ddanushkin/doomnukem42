#include "doom_nukem.h"

void	clear_screen(t_app *app)
{
	image_clear(app->sdl->surface->pixels, 0, app->screen_length_x_4);
}

void	clear_z_buffer(t_app *app)
{
	int i;

	i = 0;
	while (i < app->screen_length)
	{
		app->z_buf[i] = 0;
		i++;
	}
}

void	update_fps_data(t_app *app, TTF_Font *font_ptr, SDL_Color font_color)
{
	char fps_text[50];
	itoa(app->timer->fps, fps_text, 10);
	SDL_Surface *font_surface = TTF_RenderText_Solid(font_ptr, fps_text, font_color);
	SDL_BlitSurface(font_surface, NULL, app->sdl->surface, NULL);
	SDL_FreeSurface(font_surface);
}

void	start_the_game(t_app *app)
{
	TTF_Init();
	TTF_Font *font_ptr = TTF_OpenFont("resources/calibrib.ttf", 16);
	SDL_Color font_color = {255, 255, 255};

	SDL_SetRelativeMouseMode(SDL_TRUE);
	app->camera->pos = new_vector(0.0, 0.0, -2.5);
	app->screen_length = SCREEN_W * SCREEN_H;
	app->screen_length_x_4 = app->screen_length * 4;
	while (1)
	{
		get_ticks(app->timer);
		clear_screen(app);
		clear_z_buffer(app);
		mouse_update(app);
		if (!event_handling(app))
			break;

		int i = 0;
		while (i < 1)
		{
			app->meshes[i].pos = new_vector(-0.5, -0.5, 0);
			app->meshes[i].rot.y += 1.0 * app->timer->delta;
			app->meshes[i].trans_mat = init_translation_mat(app->meshes[i].pos);
			app->meshes[i].rot_mat = init_rotation_matrix(app->meshes[i].rot);
			app->meshes[i].transform = matrix_multiply_matrix(app->meshes[i].rot_mat, app->meshes[i].trans_mat);
			transform_vertices(app, i);
			assemble_triangles(app, i);
			check_triangles(app, i);
			i++;
		}
		draw_cross(app, 7.0, 255, 0, 200);
		get_delta_time(app->timer);
		update_fps_data(app, font_ptr, font_color);
		SDL_UpdateWindowSurface(app->sdl->window);
	}
	TTF_CloseFont(font_ptr);
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
	bmp_load("resources/2.bmp", &app->sprites[0]);

	start_the_game(app);
	quit_properly(app);
	return (0);
}

