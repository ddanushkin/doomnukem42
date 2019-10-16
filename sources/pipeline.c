#include "doom_nukem.h"

void	transform_vertices(t_app *app, int mesh_id)
{
	int			i;
	t_mesh		*mesh;
	t_vector	v;

	mesh = &app->meshes[mesh_id];
	i = 0;
	while (i < mesh->v_count)
	{
		v = matrix_multiply_vector(mesh->transform, mesh->v_orig[i]);
		v = matrix_multiply_vector(app->world->mat, v);
		v = matrix_multiply_vector(app->camera->view_mat, v);
		mesh->v_buff[i] = v;
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
	mesh = app->meshes[mesh_id];
	vertexes = mesh.v_buff;
	while (i < app->meshes[mesh_id].t_count)
	{
		t = &mesh.t[i];
		t->v[0] = vertexes[t->i[0]];
		t->v[1] = vertexes[t->i[1]];
		t->v[2] = vertexes[t->i[2]];
		t->color = new_color(128, 128, 128);
		i++;
	}
}

void	check_triangles(t_app *app, int mesh_id)
{
	t_triangle	*tr;
	int			t_idx;
	t_mesh		*mesh;
	t_triangle	list[10000];
	int			list_index = 0;

	mesh = &app->meshes[mesh_id];
	t_idx = 0;
	while (t_idx < mesh->t_count)
	{
		tr = &mesh->t[t_idx];
		check_triangle(app, tr);
		if (tr->visible)
		{
			list[list_index] = *tr;
			list_index++;
		}
		t_idx++;
	}
	list_index--;
	while (list_index >= 0)
	{
		render_triangle(app, list[list_index]);
		list_index--;
	}
}

void	draw_triangles(t_app *app)
{
	t_tr_list *cursor;

	if (app->triangles_head->last)
		return ;
	cursor = app->triangles_head;
	while (cursor)
	{
		render_triangle(app, cursor->tr);
		if (cursor->last)
			break ;
		cursor = cursor->next;
	}
	app->triangles = app->triangles_head;
	app->triangles_head->last = 1;
}