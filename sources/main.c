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

void	transform_vertices(t_app *app, int mesh_id)
{
	int			i;
	t_mesh		mesh;
	t_vector	v;

	mesh = app->mesh[mesh_id];
	i = 0;
	while (i < mesh.v_count)
	{
		//v = matrix_multiply_vector(mesh.transform, mesh.v[i]);
		v = matrix_multiply_vector(app->world.mat, mesh.v[i]);
		v = matrix_multiply_vector(app->camera.view_mat, v);
		app->vertexes[mesh_id][i] = v;
		i++;
	}
}

void	assemble_triangles(t_app *app, int mesh_id)
{
	int			i;
	t_triangle	*t;
	t_mesh		mesh;
	t_vector	*vertexes;

	i = 0;
	mesh = app->mesh[mesh_id];
	vertexes = app->vertexes[mesh_id];
	while (i < app->mesh[mesh_id].t_count)
	{
		t = &mesh.t[i];
		t->color = new_color(128, 128, 128);
		t->v[0] = vertexes[t->i[0]];
		t->v[1] = vertexes[t->i[1]];
		t->v[2] = vertexes[t->i[2]];
		i++;
	}
}

void	draw_mesh(t_app *app, int mesh_id)
{
	t_triangle	tr;
	int			t_idx;
	int			tr_idx;
	t_triangle	to_render[5000];
	t_mesh		mesh;

	mesh = app->mesh[mesh_id];

	tr_idx = 0;
	t_idx = 0;

	while (t_idx < mesh.t_count)
	{
		tr = check_triangle(app, mesh.t[t_idx]);
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

t_mat4x4	init_translation_mat(t_vector trans_v)
{
	t_mat4x4	mat;

	mat = matrix_identity();
	mat.m[0][3] = trans_v.x;
	mat.m[1][3] = trans_v.y;
	mat.m[2][3] = trans_v.z;
	return (mat);
}

void		mouse_update(t_app *app)
{
	Uint32	mouse_state;

	mouse_state = SDL_GetRelativeMouseState(&app->mouse.x, &app->mouse.y);
	if (mouse_state & SDL_BUTTON(SDL_BUTTON_LEFT))
		app->mouse.left = 1;
	else
		app->mouse.left = 0;
	if (mouse_state & SDL_BUTTON(SDL_BUTTON_RIGHT))
		app->mouse.right = 1;
	else
		app->mouse.right = 0;
	if (mouse_state & SDL_BUTTON(SDL_BUTTON_MIDDLE))
		app->mouse.middle = 1;
	else
		app->mouse.middle = 0;
}

void	draw_cross(t_app *app, float size, int r, int g, int b)
{
	t_vector	cross_hl_s;
	t_vector	cross_hl_e;
	t_vector	cross_vl_s;
	t_vector	cross_vl_e;
	t_color		cross_color;

	if (size <= 0.0f)
		size = 5.0f;

	set_color(&cross_color, r, g, b);
	cross_hl_s = new_vector(app->sdl->half_width - size, app->sdl->half_height, 0.0f);
	cross_hl_e = new_vector(app->sdl->half_width + size, app->sdl->half_height, 0.0f);
	cross_vl_s = new_vector(app->sdl->half_width, app->sdl->half_height - size, 0.0f);
	cross_vl_e = new_vector(app->sdl->half_width, app->sdl->half_height + size, 0.0f);
	draw_line(app, cross_hl_s, cross_hl_e, &cross_color);
	draw_line(app, cross_vl_s, cross_vl_e, &cross_color);
}

void	start_the_game(t_app *app)
{
	/* Set camera position */
	app->camera.pos = new_vector(0.0f, 0.0f, -5.0f);

	/* Set mesh to center */
	app->mesh[0].pos = new_vector(-0.5f, -0.5f, -0.5f);

	SDL_SetRelativeMouseMode(SDL_TRUE);
	while (1)
	{
		get_ticks(&app->timer);

		clear_screen(app);

		mouse_update(app);

		if (!event_handling(app))
			break;

		/* Animate world rotation */
		//app->world.rot.x += 0.001f;
		//app->world.rot.y += 0.001f;
		//app->world.rot.z += 0.001f;

		/* Create world rotation matrices */
		app->world.rot_mat_x = rotation_mat_x(app->world.rot.x);
		app->world.rot_mat_y = rotation_mat_y(app->world.rot.y);
		app->world.rot_mat_z = rotation_mat_z(app->world.rot.z);

		/* Create world translation matrix */
		app->world.trans = new_vector(0.0f, 0.0f, 0.0f);
		app->world.trans_mat = init_translation_mat(app->world.trans);

		/* Create world matrix */
		app->world.mat = matrix_identity();
		app->world.mat = matrix_multiply_matrix(app->world.mat, app->world.rot_mat_x);
		app->world.mat = matrix_multiply_matrix(app->world.mat, app->world.rot_mat_y);
		app->world.mat = matrix_multiply_matrix(app->world.mat, app->world.rot_mat_z);
		app->world.mat = matrix_multiply_matrix(app->world.mat, app->world.trans_mat);

		/* Create camera rotation matrices */
		app->camera.rot_mat_x = rotation_mat_x(app->camera.rot.x);
		app->camera.rot_mat_y = rotation_mat_y(app->camera.rot.y);
		app->camera.rot_mat_z = rotation_mat_z(app->camera.rot.z);

		/* Create camera rotation matrix */
		app->camera.rot_mat = matrix_identity();
		app->camera.rot_mat = matrix_multiply_matrix(app->camera.rot_mat, app->camera.rot_mat_x);
		app->camera.rot_mat = matrix_multiply_matrix(app->camera.rot_mat, app->camera.rot_mat_y);
		app->camera.rot_mat = matrix_multiply_matrix(app->camera.rot_mat, app->camera.rot_mat_z);

		/* Create camera view matrix */
		app->camera.target = new_vector(0.0f, 0.0f, 1.0f);
		app->camera.dir = matrix_multiply_vector(app->camera.rot_mat, app->camera.target);
		app->camera.target = vector_sum(app->camera.pos, app->camera.dir);
		app->camera.view_mat = matrix_look_at(app->camera.pos, app->camera.target);
		app->camera.view_mat = matrix_inverse(app->camera.view_mat);

		/* Animate mesh[0] rotation */
		//app->mesh[0].rot.x += 1.0f * app->timer.delta;
		app->mesh[0].rot.y += 1.0f * app->timer.delta;
		//app->mesh[0].rot.z += 1.0f * app->timer.delta;

		/* Animate mesh[0] position */
		//app->mesh[0].pos.x = sinf(app->timer.time) * 2.0f;
		//app->mesh[0].pos.y = sinf(app->timer.time) * 2.0f;
		//app->mesh[0].pos.z = sinf(app->timer.time) * 2.0f;

		app->mesh[0].rot_mat_x = rotation_mat_x(app->mesh[0].rot.x);
		app->mesh[0].rot_mat_y = rotation_mat_y(app->mesh[0].rot.y);
		app->mesh[0].rot_mat_z = rotation_mat_z(app->mesh[0].rot.z);

		app->mesh[0].trans_mat = init_translation_mat(app->mesh[0].pos);

		app->mesh[0].transform = matrix_identity();
		app->mesh[0].transform = matrix_multiply_matrix(app->mesh[0].transform, app->mesh[0].rot_mat_z);
		app->mesh[0].transform = matrix_multiply_matrix(app->mesh[0].transform, app->mesh[0].rot_mat_y);
		app->mesh[0].transform = matrix_multiply_matrix(app->mesh[0].transform, app->mesh[0].rot_mat_x);
		app->mesh[0].transform = matrix_multiply_matrix(app->mesh[0].transform, app->mesh[0].trans_mat);

		transform_vertices(app, 0);
		assemble_triangles(app, 0);
		draw_mesh(app, 0);

		draw_cross(app, 7.0f, 255, 0, 200);

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

	/* TODO: Set number of meshes*/
	int number_of_meshes = 1;
	app->mesh = (t_mesh *)malloc(sizeof(t_mesh) * number_of_meshes);

	/* TODO: While number of meshes*/
	app->mesh[0].rot = new_vector(0.0f, 0.0f, 0.0f);
	app->mesh[0].pos = new_vector(0.0f, 0.0f, 0.0f);

	init_app(app);

	/* TODO: Count or read number of vertexes & triangles from mesh */
	int cube_vrs = 8;
	int cube_trs = 12;
	read_obj("../cube.obj", &app->mesh[0], cube_vrs, cube_trs);

	app->triangles = new_triangle_list(cube_trs * 9);
	app->vertexes = (t_vector **)malloc(sizeof(t_vector *) * number_of_meshes);

	/* TODO: While number of loaded meshes*/
	app->vertexes[0] = (t_vector *)malloc(sizeof(t_vector) * app->mesh[0].v_count);

	start_the_game(app);
	quit_properly(app);
	return (0);
}

