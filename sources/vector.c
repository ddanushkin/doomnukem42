#include "doom_nukem.h"

void		set_vector(t_vector *v, float x, float y, float z)
{
	v->x = x;
	v->y = y;
	v->z = z;
}

float 		vector_length(t_vector v)
{
	return (sqrtf(vector_dot_product(v, v)));
}

t_vector	vector_normalise(t_vector v)
{
	t_vector	result;
	float		length;

	length = vector_length(v);
	result.x = v.x / length;
	result.y = v.y / length;
	result.z = v.z / length;
	return (result);
}

float		vector_dot_product(t_vector v1, t_vector v2)
{
	return (v1.x * v2.x +
			v1.y * v2.y +
			v1.z * v2.z);
}

t_vector	vector_cross_product(t_vector v1, t_vector v2)
{
	t_vector	result;

	result.x = v1.y * v2.z - v1.z * v2.y;
	result.y = v1.z * v2.x - v1.x * v2.z;
	result.z = v1.x * v2.y - v1.y * v2.x;
	return (result);
}