#include "doom_nukem.h"

void	clear_screen(t_app *app)
{
	int len;

	len = app->sdl->width * app->sdl->height * 4;
	image_clear(app->sdl->surface->pixels, 0, len);
}

void	start_the_game(t_app *app)
{
	/* Set meshes to center */
	app->meshes[0].pos = new_vector(-0.5f, -0.5f, -0.5f);
	SDL_SetRelativeMouseMode(SDL_TRUE);
	while (1)
	{
		get_ticks(app->timer);

		clear_screen(app);

		mouse_update(app);

		if (!event_handling(app))
			break;

		/* Animate world rotation */
		//app->world.rot.x += 1.0f * app->timer->delta;
		//app->world.rot.y += 1.0f * app->timer->delta;
		//app->world.rot.z += 1.0f * app->timer->delta;

		/* Create world rotation matrices */
		app->world->rot_mat_x = rotation_mat_x(app->world->rot.x);
		app->world->rot_mat_y = rotation_mat_y(app->world->rot.y);
		app->world->rot_mat_z = rotation_mat_z(app->world->rot.z);

		/* Create world translation matrix */
		app->world->trans = new_vector(0.0f, 0.0f, 5.0f);
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
		app->camera->target = new_vector(0.0f, 0.0f, 1.0f);
		app->camera->dir = matrix_multiply_vector(app->camera->rot_mat, app->camera->target);
		app->camera->target = vector_sum(app->camera->pos, app->camera->dir);
		app->camera->view_mat = matrix_look_at(app->camera->pos, app->camera->target);
		app->camera->view_mat = matrix_inverse(app->camera->view_mat);

		/* Animate meshes[0] rotation */
		app->meshes[0].rot.x += 1.0f * app->timer->delta;
		//app->meshes[0].rot.y += 1.0f * app->timer->delta;
		//app->meshes[0].rot.z += 1.0f * app->timer->delta;

		/* Animate meshes[0] position */
		app->meshes[0].pos.x = sinf(app->timer->time) * 2.0f;
		//app->meshes[0].pos.y = sinf(app->timer->time) * 2.0f;
		//app->meshes[0].pos.z = sinf(app->timer->time) * 2.0f;

		app->meshes[0].rot_mat_x = rotation_mat_x(app->meshes[0].rot.x);
		app->meshes[0].rot_mat_y = rotation_mat_y(app->meshes[0].rot.y);
		app->meshes[0].rot_mat_z = rotation_mat_z(app->meshes[0].rot.z);

		app->meshes[0].trans_mat = init_translation_mat(app->meshes[0].pos);

		app->meshes[0].transform = matrix_identity();
		app->meshes[0].transform = matrix_multiply_matrix(app->meshes[0].transform, app->meshes[0].rot_mat_x);
		app->meshes[0].transform = matrix_multiply_matrix(app->meshes[0].transform, app->meshes[0].rot_mat_y);
		app->meshes[0].transform = matrix_multiply_matrix(app->meshes[0].transform, app->meshes[0].rot_mat_z);
		app->meshes[0].transform = matrix_multiply_matrix(app->meshes[0].transform, app->meshes[0].trans_mat);

		/* TODO: While number of loaded meshes */
		transform_vertices(app, 0);
		assemble_triangles(app, 0);
		check_triangles(app, 0);

		/* TODO: After all meshed precessed */
		//draw_triangles(app);

		draw_cross(app, 7.0f, 255, 0, 200);

		SDL_UpdateWindowSurface(app->sdl->window);

		get_delta_time(app->timer);
	}
	SDL_Quit();
	SDL_DestroyWindow(app->sdl->window);
}

int		main(int argv, char**argc)
{
	t_app	*app;

	app = (t_app *)malloc(sizeof(t_app));
	app->sdl = (t_sdl *)malloc(sizeof(t_sdl));
	app->inputs = (t_inputs *)malloc(sizeof(t_inputs));
	app->timer = (t_timer *)malloc(sizeof(t_timer));
	app->world = (t_world *)malloc(sizeof(t_world));
	app->camera = (t_camera *)malloc(sizeof(t_camera));

	/* TODO: Set number of meshes */
	int number_of_meshes = 1;
	app->meshes = (t_mesh *)malloc(sizeof(t_mesh) * number_of_meshes);

	/* TODO: While number of meshes */
	app->meshes[0].rot = new_vector(0.0f, 0.0f, 0.0f);
	app->meshes[0].pos = new_vector(0.0f, 0.0f, 0.0f);

	init_app(app);

	/* TODO: Count or read number of vertexes & triangles from meshes */
	int cube_vrs = 3644;
	int cube_trs = 6319;
	read_obj("../teapot.obj", &app->meshes[0], cube_vrs, cube_trs);

	//app->triangles = new_triangle_list(cube_trs * 10);
	//app->triangles_head = app->triangles;

	start_the_game(app);
	quit_properly(app);
	return (0);
}

