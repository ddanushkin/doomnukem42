/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lglover <lglover@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/23 17:29:04 by lglover           #+#    #+#             */
/*   Updated: 2020/03/23 17:29:04 by lglover          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"

double	v3d_length(t_v3d v)
{
	return (sqrt(v.x * v.x + v.y * v.y + v.z * v.z));
}

t_v3d	v3d_normalise(t_v3d v)
{
	t_v3d	result;
	double	length;

	length = v3d_length(v);
	result.x = v.x / length;
	result.y = v.y / length;
	result.z = v.z / length;
	result.w = 1.0;
	return (result);
}

double	v3d_dot(t_v3d v1, t_v3d v2)
{
	return (v1.x * v2.x +
			v1.y * v2.y +
			v1.z * v2.z);
}

t_v3d	v3d_cross(t_v3d v1, t_v3d v2)
{
	t_v3d	result;

	result.x = v1.y * v2.z - v1.z * v2.y;
	result.y = v1.z * v2.x - v1.x * v2.z;
	result.z = v1.x * v2.y - v1.y * v2.x;
	result.w = 1.0;
	return (result);
}
