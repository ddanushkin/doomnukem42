/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector_handling.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lglover <lglover@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/23 17:29:07 by lglover           #+#    #+#             */
/*   Updated: 2020/03/23 17:29:07 by lglover          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"

t_v3d	new_vector(double x, double y, double z)
{
	t_v3d	v;

	v.x = x;
	v.y = y;
	v.z = z;
	v.w = 1;
	return (v);
}

t_v3d	v3d_sum(t_v3d vector1, t_v3d vector2)
{
	t_v3d result;

	result.x = vector1.x + vector2.x;
	result.y = vector1.y + vector2.y;
	result.z = vector1.z + vector2.z;
	result.w = 1.0;
	return (result);
}

t_v3d	v3d_sub(t_v3d vector1, t_v3d vector2)
{
	t_v3d result;

	result.x = vector1.x - vector2.x;
	result.y = vector1.y - vector2.y;
	result.z = vector1.z - vector2.z;
	result.w = 1.0;
	return (result);
}

t_v3d	v3d_mul_by(t_v3d v, double k)
{
	t_v3d result;

	result.x = v.x * k;
	result.y = v.y * k;
	result.z = v.z * k;
	result.w = 1.0;
	return (result);
}

t_v3d	v3d_div_by(t_v3d v, double k)
{
	t_v3d result;

	result.x = v.x / k;
	result.y = v.y / k;
	result.z = v.z / k;
	result.w = 1.0;
	return (result);
}
