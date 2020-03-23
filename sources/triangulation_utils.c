/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   triangulation_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lglover <lglover@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/23 17:28:56 by lglover           #+#    #+#             */
/*   Updated: 2020/03/23 17:28:56 by lglover          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"

int		is_convex(t_v3d p1, t_v3d p2, t_v3d p3)
{
	return (((p3.z - p1.z) * (p2.x - p1.x) -
	(p3.x - p1.x) * (p2.z - p1.z)) > 0);
}

int		is_inside(t_v3d p, t_v3d p1, t_v3d p2, t_v3d p3)
{
	if (is_convex(p1, p, p2))
		return (0);
	if (is_convex(p2, p, p3))
		return (0);
	if (is_convex(p3, p, p1))
		return (0);
	return (1);
}

t_v3d	normalize(t_v3d v)
{
	t_v3d	res;
	double	len;

	res = new_vector(0, 0, 0);
	len = sqrt(v.x * v.x + v.z * v.z);
	if (len != 0)
		res = v3d_div_by(v, len);
	return (res);
}

double	get_angle(t_v3d v0, t_v3d v1, t_v3d v2)
{
	t_v3d	vec1;
	t_v3d	vec2;

	vec1 = normalize(v3d_sub(v0, v1));
	vec2 = normalize(v3d_sub(v2, v1));
	return (vec1.x * vec2.x + vec1.z * vec2.z);
}
