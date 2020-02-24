#include "doom_nukem.h"

t_mat4x4	matrix_identity()
{
	t_mat4x4 mat;

	ft_bzero(&mat, sizeof(t_mat4x4));
	mat.m[0] = 1.0;
	mat.m[5] = 1.0;
	mat.m[10] = 1.0;
	mat.m[15] = 1.0;
	return (mat);
}

t_mat4x4	matrix_perspective(double fov, double aps_ratio, double z_near, double z_far)
{
	t_mat4x4	mat;
	double 		tan_half_fov = tan(fov * 0.5);
	double 		z_range = z_near - z_far;

	ft_bzero(&mat, sizeof(t_mat4x4));
	mat.m[0] = 1.0 / (tan_half_fov * aps_ratio);
	mat.m[5] = 1.0 / tan_half_fov;
	mat.m[10] = (-z_near - z_far) / z_range;
	mat.m[11] = 2 * z_far * z_near / z_range;
	mat.m[14] = 1.0;
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
	mat.m[0] = half_width - 0.5;
	mat.m[3] = half_width;
	mat.m[5] = -half_height;
	mat.m[7] = half_height - 0.5;
	mat.m[10] = 1.0;
	mat.m[15] = 1.0;
	return (mat);
}

t_mat4x4	matrix_translation(double x, double y, double z)
{
	t_mat4x4	mat;

	mat = matrix_identity();
	mat.m[3] = x;
	mat.m[7] = y;
	mat.m[11] = z;
	return (mat);
}

t_v3d	matrix_transform(t_mat4x4 mat, t_v3d v)
{
	t_v3d	new_v;
	double	x;
	double	y;
	double	z;
	double	w;

	x = v.x;
	y = v.y;
	z = v.z;
	w = v.w;
	new_v.x = mat.m[0] * x + mat.m[1] * y + mat.m[2] * z + mat.m[3] * w;
	new_v.y = mat.m[4] * x + mat.m[5] * y + mat.m[6] * z + mat.m[7] * w;
	new_v.z = mat.m[8] * x + mat.m[9] * y + mat.m[10] * z + mat.m[11] * w;
	new_v.w = mat.m[12] * x + mat.m[13] * y + mat.m[14] * z + mat.m[15] * w;
	new_v.tex_x = v.tex_x;
	new_v.tex_y = v.tex_y;
	return (new_v);
}

t_mat4x4	matrix_rotation_z(double angle)
{
	t_mat4x4	mat;
	double		temp_cos;
	double		temp_sin;

	temp_cos = cos(angle);
	temp_sin = sin(angle);
	mat.m[0] = temp_cos;
	mat.m[1] = -temp_sin;
	mat.m[2] = 0;
	mat.m[3] = 0;
	mat.m[4] = temp_sin;
	mat.m[5] = temp_cos;
	mat.m[6] = 0;
	mat.m[7] = 0;
	mat.m[8] = 0;
	mat.m[9] = 0;
	mat.m[10] = 1;
	mat.m[11] = 0;
	mat.m[12] = 0;
	mat.m[13] = 0;
	mat.m[14] = 0;
	mat.m[15] = 1;
	return (mat);
}

t_mat4x4	matrix_rotation_y(double angle)
{
	t_mat4x4	mat;
	double		temp_cos;
	double		temp_sin;

	temp_cos = cos(angle);
	temp_sin = sin(angle);
	mat.m[0] = temp_cos;
	mat.m[1] = 0;
	mat.m[2] = -temp_sin;
	mat.m[3] = 0;
	mat.m[4] = 0;
	mat.m[5] = 1;
	mat.m[6] = 0;
	mat.m[7] = 0;
	mat.m[8] = temp_sin;
	mat.m[9] = 0;
	mat.m[10] = temp_cos;
	mat.m[11] = 0;
	mat.m[12] = 0;
	mat.m[13] = 0;
	mat.m[14] = 0;
	mat.m[15] = 1;
	return (mat);
}

t_mat4x4	matrix_rotation_x(double angle)
{
	t_mat4x4	mat;
	double		temp_cos;
	double		temp_sin;

	temp_cos = cos(angle);
	temp_sin = sin(angle);
	mat.m[0] = 1;
	mat.m[1] = 0;
	mat.m[2] = 0;
	mat.m[3] = 0;
	mat.m[4] = 0;
	mat.m[5] = temp_cos;
	mat.m[6] = -temp_sin;
	mat.m[7] = 0;
	mat.m[8] = 0;
	mat.m[9] = temp_sin;
	mat.m[10] = temp_cos;
	mat.m[11] = 0;
	mat.m[12] = 0;
	mat.m[13] = 0;
	mat.m[14] = 0;
	mat.m[15] = 1;
	return (mat);
}

t_mat4x4	matrix_rotation(double x, double y, double z)
{
	return (matrix_multiply(
			matrix_rotation_x(x),
			matrix_multiply(
					matrix_rotation_y(y),
					matrix_rotation_z(z))));
}

t_mat4x4	matrix_multiply(t_mat4x4 m1, t_mat4x4 m2)
{
	t_mat4x4	result;
	int 		i;
	int 		j;
	int 		offset;

	i = 0;
	while (i < 4)
	{
		j = 0;
		while (j < 4)
		{
			offset = 4 * i;
			result.m[offset + j] =
				m1.m[offset] * m2.m[j] +
				m1.m[offset + 1] * m2.m[4 + j] +
				m1.m[offset + 2] * m2.m[8 + j] +
				m1.m[offset + 3] * m2.m[12 + j];
			j++;
		}
		i++;
	}
	return (result);
}

t_v3d	matrix_multiply_vector(t_mat4x4 m, t_v3d v)
{
	t_v3d r;
	
	r.x = v.x * m.m[0] + v.y * m.m[1] + v.z * m.m[2] + v.w * m.m[3];
	r.y = v.x * m.m[4] + v.y * m.m[5] + v.z * m.m[6] + v.w * m.m[7];
	r.z = v.x * m.m[8] + v.y * m.m[9] + v.z * m.m[10] + v.w * m.m[11];
	r.w = v.x * m.m[12] + v.y * m.m[13] + v.z * m.m[14] + v.w * m.m[15];
	return (r);
}
