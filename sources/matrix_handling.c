#include "doom_nukem.h"

t_mat4x4	matrix_identity()
{
	t_mat4x4 mat;

	ft_bzero(&mat, sizeof(t_mat4x4));
	mat.m[0][0] = 1.0f;
	mat.m[1][1] = 1.0f;
	mat.m[2][2] = 1.0f;
	mat.m[3][3] = 1.0f;
	return (mat);
}

t_mat4x4	init_translation_mat(t_v3d trans_v)
{
	t_mat4x4	mat;

	mat = matrix_identity();
	mat.m[0][3] = trans_v.x;
	mat.m[1][3] = trans_v.y;
	mat.m[2][3] = trans_v.z;
	return (mat);
}

t_mat4x4	matrix_inverse(t_mat4x4 m)
{
	t_mat4x4	mat;

	mat.m[0][0] = m.m[0][0];
	mat.m[0][1] = m.m[1][0];
	mat.m[0][2] = m.m[2][0];
	mat.m[0][3] = 0.0f;
	mat.m[1][0] = m.m[0][1];
	mat.m[1][1] = m.m[1][1];
	mat.m[1][2] = m.m[2][1];
	mat.m[1][3] = 0.0f;
	mat.m[2][0] = m.m[0][2];
	mat.m[2][1] = m.m[1][2];
	mat.m[2][2] = m.m[2][2];
	mat.m[2][3] = 0.0f;
	mat.m[3][0] = -(m.m[3][0] * mat.m[0][0] + m.m[3][1] *
					mat.m[1][0] + m.m[3][2] * mat.m[2][0]);
	mat.m[3][1] = -(m.m[3][0] * mat.m[0][1] + m.m[3][1] *
					mat.m[1][1] + m.m[3][2] * mat.m[2][1]);
	mat.m[3][2] = -(m.m[3][0] * mat.m[0][2] + m.m[3][1] *
					mat.m[1][2] + m.m[3][2] * mat.m[2][2]);
	mat.m[3][3] = 1.0f;
	return (mat);
}

t_mat4x4	matrix_look_at(t_v3d from, t_v3d to)
{
	t_v3d	forward;
	t_v3d	right;
	t_v3d	up;
	t_v3d	tmp;
	t_mat4x4	mat;

	tmp = new_vector(0.0f, 1.0f, 0.0f);

	forward = vector_sub(to, from);
	forward = vector_normalise(forward);

	up = vector_mul_by(forward, vector_dot_product(tmp, forward));
	up = vector_sub(tmp, up);
	up = vector_normalise(up);

	right = vector_cross_product(up, forward);

	mat.m[0][0] = right.x;
	mat.m[0][1] = right.y;
	mat.m[0][2] = right.z;
	mat.m[0][3] = 0.0f;
	mat.m[1][0] = up.x;
	mat.m[1][1] = up.y;
	mat.m[1][2] = up.z;
	mat.m[1][3] = 0.0f;
	mat.m[2][0] = forward.x;
	mat.m[2][1] = forward.y;
	mat.m[2][2] = forward.z;
	mat.m[2][3] = 0.0f;
	mat.m[3][0] = from.x;
	mat.m[3][1] = from.y;
	mat.m[3][2] = from.z;
	mat.m[3][3] = 1.0f;
	return (mat);

//	t_v3d	forward;
//	t_v3d	right;
//	t_v3d	up;
//	t_v3d	tmp;
//	t_mat4x4	mat;
//
//	tmp = new_vector(0.0f, 1.0f, 0.0f);
//	forward = vector_normalise(vector_sub(from, to));
//	right = vector_cross_product(vector_normalise(tmp), forward);
//	up = vector_cross_product(forward, right);
//	ft_bzero(&mat, sizeof(t_mat4x4));
//	mat.m[0][0] = right.x;
//	mat.m[0][1] = right.y;
//	mat.m[0][2] = right.z;
//	mat.m[1][0] = up.x;
//	mat.m[1][1] = up.y;
//	mat.m[1][2] = up.z;
//	mat.m[2][0] = forward.x;
//	mat.m[2][1] = forward.y;
//	mat.m[2][2] = forward.z;
//	mat.m[3][0] = from.x;
//	mat.m[3][1] = from.y;
//	mat.m[3][2] = from.z;
//	return (mat);
}

t_mat4x4	matrix_multiply_matrix(t_mat4x4 m1, t_mat4x4 m2)
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
					m1.m[j][0] * m2.m[0][i] +
					m1.m[j][1] * m2.m[1][i] +
					m1.m[j][2] * m2.m[2][i] +
					m1.m[j][3] * m2.m[3][i];
	}
	return (result);
}

t_mat4x4	matrix_summary(t_mat4x4 m1, t_mat4x4 m2)
{
	t_mat4x4	result;
	int 		i;
	int 		j;

	i = -1;
	while (++i < 4)
	{
		j = -1;
		while (++j < 4)
			result.m[i][j] = m1.m[i][j] + m2.m[i][j];
	}
	return (result);
}

t_mat4x4	matrix_subtraction(t_mat4x4 m1, t_mat4x4 m2)
{
	t_mat4x4	result;
	int 		i;
	int 		j;

	i = -1;
	while (++i < 4)
	{
		j = -1;
		while (++j < 4)
			result.m[i][j] = m1.m[i][j] - m2.m[i][j];
	}
	return (result);

}

t_v3d	matrix_multiply_vector(t_mat4x4 m, t_v3d v)
{
	t_v3d r;
	
	r.x = v.x * m.m[0][0] + v.y * m.m[1][0] + v.z * m.m[2][0] + v.w * m.m[3][0];
	r.y = v.x * m.m[0][1] + v.y * m.m[1][1] + v.z * m.m[2][1] + v.w * m.m[3][1];
	r.z = v.x * m.m[0][2] + v.y * m.m[1][2] + v.z * m.m[2][2] + v.w * m.m[3][2];
	r.w = v.x * m.m[0][3] + v.y * m.m[1][3] + v.z * m.m[2][3] + v.w * m.m[3][3];
	return (r);
}