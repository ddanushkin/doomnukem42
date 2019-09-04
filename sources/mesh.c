#include "doom_nukem.h"

void	set_triangle(t_triangle *t, t_vertex *v0, t_vertex *v1, t_vertex *v2)
{
	t->v[0] = v0;
	t->v[1] = v1;
	t->v[2] = v2;
}

void	make_face(t_mesh *m, int *vt_id)
{
	set_triangle(
			&m->t[m->last_tr],
			&m->v[vt_id[0]],
			&m->v[vt_id[1]],
			&m->v[vt_id[2]]);
	m->last_tr++;
	set_triangle(
			&m->t[m->last_tr],
			&m->v[vt_id[0]],
			&m->v[vt_id[2]],
			&m->v[vt_id[3]]);
	m->last_tr++;
}

void	make_cube(t_mesh *m, float size)
{
	int vt_id[4];
	float half_size = size * 0.5f;
	m->v = (t_vertex *)malloc(sizeof(t_vertex) * 8);

	set_vertex(&m->v[0], -half_size, -half_size, -half_size);
	set_vertex(&m->v[1], -half_size, half_size, -half_size);
	set_vertex(&m->v[2], half_size, half_size, -half_size);
	set_vertex(&m->v[3], half_size, -half_size, -half_size);
	set_vertex(&m->v[4], -half_size, -half_size, half_size);
	set_vertex(&m->v[5], -half_size, half_size, half_size);
	set_vertex(&m->v[6], half_size, half_size, half_size);
	set_vertex(&m->v[7], half_size, -half_size, half_size);

	m->last_vt = 7;
	m->last_tr = 0;

	vt_id[0] = 0;
	vt_id[1] = 1;
	vt_id[2] = 2;
	vt_id[3] = 3;
	make_face(m, vt_id);

	vt_id[0] = 3;
	vt_id[1] = 2;
	vt_id[2] = 6;
	vt_id[3] = 7;
	make_face(m, vt_id);

	vt_id[0] = 7;
	vt_id[1] = 6;
	vt_id[2] = 5;
	vt_id[3] = 4;
	make_face(m, vt_id);

	vt_id[0] = 4;
	vt_id[1] = 5;
	vt_id[2] = 1;
	vt_id[3] = 0;
	make_face(m, vt_id);

	vt_id[0] = 1;
	vt_id[1] = 5;
	vt_id[2] = 6;
	vt_id[3] = 2;
	make_face(m, vt_id);

	vt_id[0] = 4;
	vt_id[1] = 0;
	vt_id[2] = 3;
	vt_id[3] = 7;
	make_face(m, vt_id);
}