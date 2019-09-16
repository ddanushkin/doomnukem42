#include "doom_nukem.h"

void	clear_screen(t_app *app)
{
	int len;

	len = app->sdl->width * app->sdl->height * 4;
	image_clear(app->sdl->surface->pixels, 0, len);
}

int 	tr_cmpr(const void *p, const void *q)
{
	t_triangle	t1;
	t_triangle	t2;
	float		z1;
	float		z2;

	t1 = *(t_triangle *)p;
	t2 = *(t_triangle *)q;
	z1 = (t1.v[0].z + t1.v[1].z + t1.v[2].z) / 3.0f;
	z2 = (t2.v[0].z + t2.v[1].z + t2.v[2].z) / 3.0f;
	return (z1 > z2);
}

void	draw_mesh(t_app *app, t_mesh *mesh)
{
	t_triangle	tr;
	int			t_idx;
	int			tr_idx;
	t_triangle	to_render[5000];

	tr_idx = 0;
	t_idx = 0;

	while (t_idx < mesh->t_idx)
	{
		tr = check_triangle(app, mesh->t[t_idx]);
		if (tr.visible)
		{
			to_render[tr_idx] = tr;
			tr_idx++;
		}
		t_idx++;
	}
	qsort((void*)to_render, tr_idx, sizeof(t_triangle), tr_cmpr);
	tr_idx--;
	while (tr_idx >= 0)
	{
		render_triangle(app, to_render[tr_idx]);
		tr_idx--;
	}
}

t_mat4x4	init_translation_mat(float x, float y, float z)
{
	t_mat4x4	mat;

	ft_bzero(&mat, sizeof(t_mat4x4));
	mat.m[0][0] = 1.0f;
	mat.m[1][1] = 1.0f;
	mat.m[2][2] = 1.0f;
	mat.m[3][3] = 1.0f;
	mat.m[3][0] = x;
	mat.m[3][1] = y;
	mat.m[3][2] = z;
	return (mat);
}

void	start_the_game(t_app *app)
{
	while (1)
	{
		get_ticks(&app->timer);

		clear_screen(app);

		if (!event_handling(app))
			break;

		t_mat4x4 mesh_rot_mat_x;
		t_mat4x4 mesh_rot_mat_y;
		t_mat4x4 mesh_rot_mat_z;
		t_mat4x4 trans_mat;

		/* Animate mesh rotation */
		app->mesh[0].rot.x += 0.001f;
		app->mesh[0].rot.z += 0.001f;
		app->mesh[0].rot.y += 0.001f;

		mesh_rot_mat_x = rotation_mat_x(app->mesh[0].rot.x);
		mesh_rot_mat_y = rotation_mat_y(app->mesh[0].rot.y);
		mesh_rot_mat_z = rotation_mat_z(app->mesh[0].rot.z);

		/* КАК ЭТО РАБОТАЕТ У ДЕДА????!!! */
		trans_mat = init_translation_mat(0.0f, 0.0f, 0.0f);

		app->camera.world_mat = matrix_multiply_matrix(mesh_rot_mat_z, mesh_rot_mat_x);
		app->camera.world_mat = matrix_multiply_matrix(app->camera.world_mat, mesh_rot_mat_y);
		app->camera.world_mat = matrix_multiply_matrix(app->camera.world_mat, trans_mat);

		t_mat4x4 cam_rot_mat_x;
		t_mat4x4 cam_rot_mat_y;
		t_mat4x4 cam_rot_mat_z;

		cam_rot_mat_x = rotation_mat_x(app->camera.rot.x);
		cam_rot_mat_z = rotation_mat_z(app->camera.rot.z);
		cam_rot_mat_y = rotation_mat_y(app->camera.rot.y);

		app->camera.rot_mat = matrix_multiply_matrix(cam_rot_mat_z, cam_rot_mat_x);
		app->camera.rot_mat = matrix_multiply_matrix(app->camera.rot_mat, cam_rot_mat_y);

		app->camera.target = vector_new(0.0f, 0.0f, -1.0f, 1.0f);
		app->camera.dir = matrix_multiply_vector(app->camera.rot_mat, app->camera.target);
		app->camera.target = vector_sum(app->camera.pos, app->camera.dir);
		app->camera.view_mat = matrix_look_at(app->camera.pos, app->camera.target);
		app->camera.view_mat = matrix_inverse(app->camera.view_mat);

		draw_mesh(app, &app->mesh[0]);

		SDL_UpdateWindowSurface(app->sdl->window);

		get_delta_time(&app->timer);
	}
	SDL_Quit();
	SDL_DestroyWindow(app->sdl->window);
}

int		main(int argv, char**argc)
{
	t_app	*app;

	app = (t_app *)malloc(sizeof(t_app));
	app->mesh = (t_mesh *)malloc(sizeof(t_mesh) * 1);

	app->mesh[0].rot = vector_new(0.0f, 0.0f, 0.0f, 1.0f);
	app->mesh[0].pos = vector_new(0.0f, 0.0f, 0.0f, 1.0f);
	read_obj("../axis.obj", &app->mesh[0]);

	init_app(app);
	start_the_game(app);
	quit_properly(app);
	return (0);
}

