#include "doom_nukem.h"

t_mat4x4	matrix_look_at(t_vector from, t_vector to)
{
	t_vector	forward;
	t_vector	right;
	t_vector	up;
	t_vector	tmp;
	t_mat4x4	mat;

	tmp = vector_new(0.0f, 1.0f, 0.0f, 1.0f);
	forward = vector_normalize(vector_sub(from, to));
	right = vector_cross_product(vector_normalize(tmp), forward);
	up = vector_cross_product(forward, right);
	mat.m[0][0] = right.x;
	mat.m[0][1] = right.y;
	mat.m[0][2] = right.z;
	mat.m[1][0] = up.x;
	mat.m[1][1] = up.y;
	mat.m[1][2] = up.z;
	mat.m[2][0] = forward.x;
	mat.m[2][1] = forward.y;
	mat.m[2][2] = forward.z;
	mat.m[3][0] = from.x;
	mat.m[3][1] = from.y;
	mat.m[3][2] = from.z;
	return (mat);
}

t_mat4x4	matrix_multiply_matrix(t_mat4x4 matrix1, t_mat4x4 matrix2)
{
	t_mat4x4	result;
	int 		i;
	int 		j;

	i = -1;
	while (++i < 4)
	{
		j = -1;
		while (++j < 4)
			result.m[i][j] =
					matrix1.m[j][0] * matrix2.m[0][i] +
					matrix1.m[j][1] * matrix2.m[1][i] +
					matrix1.m[j][2] * matrix2.m[2][i] +
					matrix1.m[j][3] * matrix2.m[3][i];
	}
	return (result);
}

t_mat4x4	matrix_summary(t_mat4x4 matrix1, t_mat4x4 matrix2)
{
	t_mat4x4	result;
	int 		i;
	int 		j;

	i = -1;
	while (++i < 4)
	{
		j = -1;
		while (++j < 4)
			result.m[i][j] = matrix1.m[i][j] + matrix2.m[i][j];
	}
	return (result);
}

t_mat4x4	matrix_subtraction(t_mat4x4 matrix1, t_mat4x4 matrix2)
{
	t_mat4x4	result;
	int 		i;
	int 		j;

	i = -1;
	while (++i < 4)
	{
		j = -1;
		while (++j < 4)
			result.m[i][j] = matrix1.m[i][j] - matrix2.m[i][j];
	}
	return (result);

}

t_vector	matrix_multiply_vector(t_mat4x4 matrix, t_vector vector)
{
	t_vector	result;

	result.x = vector.x * matrix.m[0][0] + vector.y * matrix.m[1][0] +
			vector.z * matrix.m[2][0] + vector.w * matrix.m[3][0];
	result.y = vector.x * matrix.m[0][1] + vector.y * matrix.m[1][1] +
			vector.z * matrix.m[2][1] + vector.w * matrix.m[3][1];
	result.z = vector.x * matrix.m[0][2] + vector.y * matrix.m[1][2] +
			vector.z * matrix.m[2][2] + vector.w * matrix.m[3][2];
	result.w = vector.x * matrix.m[0][3] + vector.y * matrix.m[1][3] +
			vector.z * matrix.m[2][3] + vector.w * matrix.m[3][3];
	return (result);
}