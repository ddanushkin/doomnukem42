#include "doom_nukem.h"

t_vertex vector_summary(t_vertex vector1, t_vertex vector2)
{
	t_vertex result;

	result.x = vector1.x + vector2.x;
	result.y = vector1.y + vector2.y;
	result.z = vector1.z + vector2.z;
	return result;
}

t_vertex vector_subtract(t_vertex vector1, t_vertex vector2)
{
	t_vertex result;

	result.x = vector1.x - vector2.x;
	result.y = vector1.y - vector2.y;
	result.z = vector1.z - vector2.z;
	return result;
}

t_vertex vector_multiply(t_vertex vector1, float k)
{
	t_vertex result;

	result.x = vector1.x * k;
	result.y = vector1.y * k;
	result.z = vector1.z * k;
	return result;
}

t_vertex vector_divide(t_vertex vector1, float k)
{
	t_vertex result;

	result.x = vector1.x / k;
	result.y = vector1.y / k;
	result.z = vector1.z / k;
	return result;
}

float	dot_product(t_vertex vector1, t_vertex vector2)
{
	return (vector1.x * vector2.x +
			vector1.y * vector2.y +
			vector1.z * vector2.z);
}

float 	vector_length(t_vertex vector)
{
	return (sqrtf(dot_product(vector, vector)));
}

t_vertex	normalise_vector(t_vertex vector)
{
	t_vertex	result;
	float		length;

	length = vector_length(vector);
	result.x = vector.x / length;
	result.y = vector.y / length;
	result.z = vector.z / length;
	return (result);
}

t_vertex	vector_cross_product(t_vertex vector1, t_vertex vector2)
{
	t_vertex result;

	result.x = vector1.y * vector2.z - vector1.z * vector2.y;
	result.y = vector1.z * vector2.x - vector1.x * vector2.z;
	result.z = vector1.x * vector2.y - vector1.y * vector2.x;
	return (result);
}

float 	cross_product(t_vertex *v_1, t_vertex *v_2)
{
	return (v_1->x * v_2->x +
			v_1->y * v_2->y +
			v_1->z * v_2->z);
}
