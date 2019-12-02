#include "doom_nukem.h"

void	transform_vertices(t_app *app, int mesh_id)
{
	int			i;
	t_mesh		*mesh;

	mesh = &app->meshes[mesh_id];
	i = 0;
	while (i < mesh->v_count)
	{
		mesh->vb[i] = matrix_multiply_vector(mesh->transform, mesh->vo[i]);
		i++;
	}
}

void	triangle_fill_data(t_triangle *tr, t_v3d *vr, t_v2d *tx)
{
	int		j;

	j = 0;
	while (j < 3)
	{
		tr->v[j] = vr[tr->iv[j]];
		tr->t[j] = tx[tr->it[j]];
		j++;
	}
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
