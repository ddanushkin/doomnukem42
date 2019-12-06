#include "doom_nukem.h"

t_mat4x4	matrix_identity()
{
	t_mat4x4 mat;

	ft_bzero(&mat, sizeof(t_mat4x4));
	mat.m[0][0] = 1.0;
	mat.m[1][1] = 1.0;
	mat.m[2][2] = 1.0;
	mat.m[3][3] = 1.0;
	return (mat);
}

t_mat4x4	matrix_perspective(double fov, double aps_ratio, double z_near, double z_far)
{
	t_mat4x4	mat;
	double 		tan_half_fov = tan(fov * 0.5);
	double 		z_range = z_near - z_far;

	ft_bzero(&mat, sizeof(t_mat4x4));
	mat.m[0][0] = 1.0 / (tan_half_fov * aps_ratio);
	mat.m[1][1] = 1.0 / tan_half_fov;
	mat.m[2][2] = (-z_near - z_far) / z_range;
	mat.m[2][3] = 2 * z_far * z_near / z_range;
	mat.m[3][2] = 1.0;
	return (mat);
}

t_mat4x4	matrix_screen_space()
{
	t_mat4x4	mat;
	double		half_height;
	double		half_width;

	half_width = (double)SCREEN_W * 0.5;
	half_height = (double)SCREEN_H * 0.5;
	ft_bzero(&mat, sizeof(t_mat4x4));
	mat.m[0][0] = half_width - 0.5;
	mat.m[0][3] = half_width;
	mat.m[1][1] = -half_height;
	mat.m[1][3] = half_height - 0.5;
	mat.m[2][2] = 1.0;
	mat.m[3][3] = 1.0;
	return (mat);
}

t_mat4x4	matrix_translation(double x, double y, double z)
{
	t_mat4x4	mat;

	mat = matrix_identity();
	mat.m[0][3] = x;
	mat.m[1][3] = y;
	mat.m[2][3] = z;
	mat.m[3][3] = 1.0;
	return (mat);
}

t_v3d	matrix_transform(t_mat4x4 mat, t_v3d v)
{
	t_v3d	new_v;

	new_v.x = mat.m[0][0] * v.x + mat.m[0][1] * v.y + mat.m[0][2] * v.z + mat.m[0][3] * v.w;
	new_v.y = mat.m[1][0] * v.x + mat.m[1][1] * v.y + mat.m[1][2] * v.z + mat.m[1][3] * v.w;
	new_v.z = mat.m[2][0] * v.x + mat.m[2][1] * v.y + mat.m[2][2] * v.z + mat.m[2][3] * v.w;
	new_v.w = mat.m[3][0] * v.x + mat.m[3][1] * v.y + mat.m[3][2] * v.z + mat.m[3][3] * v.w;
	new_v.tex_x = v.tex_x;
	new_v.tex_y = v.tex_y;
	return (new_v);
}

t_mat4x4	matrix_rotation_z(double angle)
{
	t_mat4x4	mat;
	double temp_cos;
	double temp_sin;

	temp_cos = cos(angle);
	temp_sin = sin(angle);
	mat.m[0][0] = temp_cos;
	mat.m[0][1] = -temp_sin;
	mat.m[0][2] = 0;
	mat.m[0][3] = 0;
	mat.m[1][0] = temp_sin;
	mat.m[1][1] = temp_cos;
	mat.m[1][2] = 0;
	mat.m[1][3] = 0;
	mat.m[2][0] = 0;
	mat.m[2][1] = 0;
	mat.m[2][2] = 1;
	mat.m[2][3] = 0;
	mat.m[3][0] = 0;
	mat.m[3][1] = 0;
	mat.m[3][2] = 0;
	mat.m[3][3] = 1;
	return (mat);
}

t_mat4x4	matrix_rotation_y(double angle)
{
	t_mat4x4	mat;
	double temp_cos;
	double temp_sin;

	temp_cos = cos(angle);
	temp_sin = sin(angle);
	mat.m[0][0] = temp_cos;
	mat.m[0][1] = 0;
	mat.m[0][2] = -temp_sin;
	mat.m[0][3] = 0;
	mat.m[1][0] = 0;
	mat.m[1][1] = 1;
	mat.m[1][2] = 0;
	mat.m[1][3] = 0;
	mat.m[2][0] = temp_sin;
	mat.m[2][1] = 0;
	mat.m[2][2] = temp_cos;
	mat.m[2][3] = 0;
	mat.m[3][0] = 0;
	mat.m[3][1] = 0;
	mat.m[3][2] = 0;
	mat.m[3][3] = 1;
	return (mat);
}

t_mat4x4	matrix_rotation_x(double angle)
{
	t_mat4x4	mat;
	double temp_cos;
	double temp_sin;

	temp_cos = cos(angle);
	temp_sin = sin(angle);
	mat.m[0][0] = 1;
	mat.m[0][1] = 0;
	mat.m[0][2] = 0;
	mat.m[0][3] = 0;
	mat.m[1][0] = 0;
	mat.m[1][1] = temp_cos;
	mat.m[1][2] = -temp_sin;
	mat.m[1][3] = 0;
	mat.m[2][0] = 0;
	mat.m[2][1] = temp_sin;
	mat.m[2][2] = temp_cos;
	mat.m[2][3] = 0;
	mat.m[3][0] = 0;
	mat.m[3][1] = 0;
	mat.m[3][2] = 0;
	mat.m[3][3] = 1;
	return (mat);
}

t_mat4x4	matrix_rotation(double x, double y, double z)
{
	t_mat4x4 rot;

	rot = matrix_multiply(matrix_identity(), matrix_rotation_x(x));
	rot = matrix_multiply(rot, matrix_rotation_y(y));
	rot = matrix_multiply(rot, matrix_rotation_z(z));
	return (rot);
}

t_mat4x4	matrix_inverse(t_mat4x4 m)
{
	t_mat4x4	mat;

	mat.m[0][0] = m.m[0][0];
	mat.m[0][1] = m.m[1][0];
	mat.m[0][2] = m.m[2][0];
	mat.m[0][3] = 0.0;
	mat.m[1][0] = m.m[0][1];
	mat.m[1][1] = m.m[1][1];
	mat.m[1][2] = m.m[2][1];
	mat.m[1][3] = 0.0;
	mat.m[2][0] = m.m[0][2];
	mat.m[2][1] = m.m[1][2];
	mat.m[2][2] = m.m[2][2];
	mat.m[2][3] = 0.0;
	mat.m[3][0] = -(m.m[3][0] * mat.m[0][0] + m.m[3][1] *
					mat.m[1][0] + m.m[3][2] * mat.m[2][0]);
	mat.m[3][1] = -(m.m[3][0] * mat.m[0][1] + m.m[3][1] *
					mat.m[1][1] + m.m[3][2] * mat.m[2][1]);
	mat.m[3][2] = -(m.m[3][0] * mat.m[0][2] + m.m[3][1] *
					mat.m[1][2] + m.m[3][2] * mat.m[2][2]);
	mat.m[3][3] = 1.0;
	return (mat);
}

t_mat4x4	matrix_look_at(t_v3d from, t_v3d to)
{
	t_v3d	forward;
	t_v3d	right;
	t_v3d	up;
	t_v3d	tmp;
	t_mat4x4	mat;

	tmp = new_vector(0.0, 1.0, 0.0);

	forward = vector_sub(to, from);
	forward = vector_normalise(forward);

	up = vector_mul_by(forward, vector_dot_product(tmp, forward));
	up = vector_sub(tmp, up);
	up = vector_normalise(up);

	right = vector_cross_product(up, forward);

	mat.m[0][0] = right.x;
	mat.m[0][1] = right.y;
	mat.m[0][2] = right.z;
	mat.m[0][3] = 0.0;
	mat.m[1][0] = up.x;
	mat.m[1][1] = up.y;
	mat.m[1][2] = up.z;
	mat.m[1][3] = 0.0;
	mat.m[2][0] = forward.x;
	mat.m[2][1] = forward.y;
	mat.m[2][2] = forward.z;
	mat.m[2][3] = 0.0;
	mat.m[3][0] = from.x;
	mat.m[3][1] = from.y;
	mat.m[3][2] = from.z;
	mat.m[3][3] = 1.0;
	return (mat);
}

t_mat4x4	matrix_multiply(t_mat4x4 m1, t_mat4x4 m2)
{
	t_mat4x4	result;
	int 		i;
	int 		j;

	i = 0;
	while (i < 4)
	{
		j = 0;
		while (j < 4)
		{
			result.m[i][j] =
				m1.m[i][0] * m2.m[0][j] +
				m1.m[i][1] * m2.m[1][j] +
				m1.m[i][2] * m2.m[2][j] +
				m1.m[i][3] * m2.m[3][j];
			j++;
		}
		i++;
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