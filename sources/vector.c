#include "doom_nukem.h"

void	set_vertex(t_vertex *vertex, float x, float y, float z)
{
	vertex->x = x;
	vertex->y = y;
	vertex->z = z;
}

float 	cross_product(t_vertex *v_1, t_vertex *v_2)
{
	return (v_1->x * v_2->x +
			v_1->y * v_2->y +
			v_1->z * v_2->z);
}
