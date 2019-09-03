#include "doom_nukem.h"

void	set_vector(t_vector *vector, float x, float y, float z)
{
	vector->x = x;
	vector->y = y;
	vector->z = z;
}

float 	cross_product(t_vector *v_1, t_vector *v_2)
{
	return (v_1->x * v_2->x +
			v_1->y * v_2->y +
			v_1->z * v_2->z);
}
