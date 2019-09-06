#include "doom_nukem.h"

t_mat4x4 matrix_multiply_matrix(t_mat4x4 matrix1, t_mat4x4 matrix2)
{
	t_mat4x4	result;
	int 		i;
	int 		j;

	i = -1;
	while (++i < 4)
	{
		j = -1;
		while (++j < 4)
			result.m[i][j] = matrix1.m[j][0] * matrix2.m[0][i] +
					matrix1.m[j][1] * matrix2.m[1][i] +
					matrix1.m[j][2] * matrix2.m[2][i] +
					matrix1.m[j][3] * matrix2.m[3][i];
	}
	return result;
}

t_mat4x4 matrix_summary(t_mat4x4 matrix1, t_mat4x4 matrix2)
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
	return result;
}

t_mat4x4 matrix_subtraction(t_mat4x4 matrix1, t_mat4x4 matrix2)
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
	return result;

}

t_vertex matrix_multiply_vector(t_mat4x4 matrix, t_vertex vector)
{
	t_vertex result;

	result.x = vector.x * matrix.m[0][0] + vector.y * matrix.m[1][0] +
			vector.z * matrix.m[2][0] + vector.w * matrix.m[3][0];
	result.y = vector.x * matrix.m[0][1] + vector.y * matrix.m[1][1] +
			vector.z * matrix.m[2][1] + vector.w * matrix.m[3][1];
	result.x = vector.x * matrix.m[0][2] + vector.y * matrix.m[1][2] +
			vector.z * matrix.m[2][2] + vector.w * matrix.m[3][2];
	result.w = vector.x * matrix.m[0][3] + vector.y * matrix.m[1][3] +
			vector.z * matrix.m[2][3] + vector.w * matrix.m[3][3];
	return result;
}