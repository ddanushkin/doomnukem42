#include "doom_nukem.h"

void	make_cube(t_mesh *mesh)
{
	//FRONT PLANE
	//
	set_vector(&mesh->t[0].v[0], 0, 0, 0);
	set_vector(&mesh->t[0].v[1], 1, 0, 0);
	set_vector(&mesh->t[0].v[2], 0, 1, 0);
	//
	set_vector(&mesh->t[1].v[0], 1, 0, 0);
	set_vector(&mesh->t[1].v[1], 1, 1, 0);
	set_vector(&mesh->t[1].v[2], 0, 1, 0);

	//RIGHT PLANE
	//
	set_vector(&mesh->t[2].v[0], 1, 0, 0);
	set_vector(&mesh->t[2].v[1], 1, 0, 1);
	set_vector(&mesh->t[2].v[2], 1, 1, 0);
	//
	set_vector(&mesh->t[3].v[0], 1, 0, 1);
	set_vector(&mesh->t[3].v[1], 1, 1, 1);
	set_vector(&mesh->t[3].v[2], 1, 1, 0);

	//BACK PLANE
	//
	set_vector(&mesh->t[4].v[0], 1, 0, 1);
	set_vector(&mesh->t[4].v[1], 0, 0, 1);
	set_vector(&mesh->t[4].v[2], 1, 1, 1);
	//
	set_vector(&mesh->t[5].v[0], 0, 0, 1);
	set_vector(&mesh->t[5].v[1], 0, 1, 1);
	set_vector(&mesh->t[5].v[2], 1, 1, 1);

	//LEFT PLANE
	//
	set_vector(&mesh->t[6].v[0], 0, 0, 1);
	set_vector(&mesh->t[6].v[1], 0, 0, 0);
	set_vector(&mesh->t[6].v[2], 0, 1, 1);
	//
	set_vector(&mesh->t[7].v[0], 0, 0, 0);
	set_vector(&mesh->t[7].v[1], 0, 1, 0);
	set_vector(&mesh->t[7].v[2], 0, 1, 1);

	//TOP PLANE
	//
	set_vector(&mesh->t[8].v[0], 0, 0, 1);
	set_vector(&mesh->t[8].v[1], 1, 0, 1);
	set_vector(&mesh->t[8].v[2], 0, 0, 0);
	//
	set_vector(&mesh->t[9].v[0], 1, 0, 1);
	set_vector(&mesh->t[9].v[1], 1, 0, 0);
	set_vector(&mesh->t[9].v[2], 0, 0, 0);

	//BOTTOM PLANE
	//
	set_vector(&mesh->t[10].v[0], 0, 0, 0);
	set_vector(&mesh->t[10].v[1], 0, 0, 0);
	set_vector(&mesh->t[10].v[2], 0, 0, 0);
	//
	set_vector(&mesh->t[11].v[0], 0, 0, 0);
	set_vector(&mesh->t[11].v[1], 0, 0, 0);
	set_vector(&mesh->t[11].v[2], 0, 0, 0);
}