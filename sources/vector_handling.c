#include "doom_nukem.h"

t_vector	vector_new(float x, float y, float z)
{
	t_vector	v;

	v.x = x;
	v.y = y;
	v.z = z;
	v.w = 1;
	return (v);
}

t_vector	vector_sum(t_vector vector1, t_vector vector2)
{
	t_vector result;

	result.x = vector1.x + vector2.x;
	result.y = vector1.y + vector2.y;
	result.z = vector1.z + vector2.z;
	return result;
}

t_vector vector_sub(t_vector vector1, t_vector vector2)
{
	t_vector result;

	result.x = vector1.x - vector2.x;
	result.y = vector1.y - vector2.y;
	result.z = vector1.z - vector2.z;
	return result;
}

t_vector vector_mul_by(t_vector v, float k)
{
	t_vector result;

	result.x = v.x * k;
	result.y = v.y * k;
	result.z = v.z * k;
	return result;
}

t_vector vector_div_by(t_vector v, float k)
{
	t_vector result;

	result.x = v.x / k;
	result.y = v.y / k;
	result.z = v.z / k;
	return result;
}
