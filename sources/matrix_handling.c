/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   matrix_handling.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lglover <lglover@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/23 17:27:59 by lglover           #+#    #+#             */
/*   Updated: 2020/03/23 17:27:59 by lglover          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"

t_mat4x4	matrix_identity(void)
{
	t_mat4x4	mat;

	ft_bzero(&mat, sizeof(t_mat4x4));
	mat.m[0] = 1.0;
	mat.m[5] = 1.0;
	mat.m[10] = 1.0;
	mat.m[15] = 1.0;
	return (mat);
}

t_mat4x4	matrix_persp(double fov, double ar, double z_near, double z_far)
{
	t_mat4x4	mat;
	double		tan_half_fov;
	double		z_range;

	tan_half_fov = tan(fov * 0.5);
	z_range = z_near - z_far;
	ft_bzero(&mat, sizeof(t_mat4x4));
	mat.m[0] = 1.0 / (tan_half_fov * ar);
	mat.m[5] = 1.0 / tan_half_fov;
	mat.m[10] = (-z_near - z_far) / z_range;
	mat.m[11] = 2 * z_far * z_near / z_range;
	mat.m[14] = 1.0;
	return (mat);
}

t_mat4x4	matrix_screen_space(void)
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

t_mat4x4	matrix_rotation(double x, double y, double z)
{
	return (
	matrix_multiply(
	matrix_rotation_x(x),
	matrix_multiply(
	matrix_rotation_y(y),
	matrix_rotation_z(z))));
}
