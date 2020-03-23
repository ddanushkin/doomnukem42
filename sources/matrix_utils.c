/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   matrix_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lglover <lglover@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/23 17:28:02 by lglover           #+#    #+#             */
/*   Updated: 2020/03/23 17:28:02 by lglover          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"

t_v3d		matrix_transform(t_mat4x4 mat, t_v3d v)
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

t_mat4x4	matrix_multiply(t_mat4x4 m1, t_mat4x4 m2)
{
	t_mat4x4	result;
	int			i;
	int			j;
	int			offset;

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
