#include "doom_nukem.h"

void	transform_vertices(t_app *app, int mesh_id)
{
	int			i;
	t_mesh		*mesh;
	t_v3d	v;

	mesh = &app->meshes[mesh_id];
	i = 0;
	while (i < mesh->v_count)
	{
		v = matrix_multiply_vector(mesh->transform, mesh->vo[i]);
		v = matrix_multiply_vector(app->world->mat, v);
		mesh->vb[i] = v;
		i++;
	}
}

int	triangle_fill_data(t_triangle *tr, t_v3d *vr, t_v2d *tx)
{
	tr->v[0] = vr[tr->iv[0]];
	tr->t[0] = tx[tr->it[0]];
	tr->v[1] = vr[tr->iv[1]];
	tr->t[1] = tx[tr->it[1]];
	tr->v[2] = vr[tr->iv[2]];
	tr->t[2] = tx[tr->it[2]];
	return (1);
}

void	assemble_triangles(t_app *app, int mesh_id)
{
	int		i;
	t_mesh	*mesh;
	t_v3d	*vertexes;
	t_v2d	*texels;

	mesh = &app->meshes[mesh_id];
	vertexes = mesh->vb;
	texels = mesh->tx;
	i = 0;
	while (i < mesh->tr_count)
	{
		triangle_fill_data(&mesh->tr[i], vertexes, texels);
		mesh->tr[i].color = color_new(128, 128, 128);
		i++;
	}
}

void	check_triangles(t_app *app, int mesh_id)
{
	t_triangle	*tr;
	int			t_idx;
	t_mesh		*mesh;

	mesh = &app->meshes[mesh_id];
	t_idx = 0;
	while (t_idx < mesh->tr_count)
	{
		tr = &mesh->tr[t_idx];
		check_triangle(app, tr);
		if (tr->visible)
			render_triangle(app, *tr);
		t_idx++;
	}
}
