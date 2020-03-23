/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   intersect.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lglover <lglover@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/23 17:27:36 by lglover           #+#    #+#             */
/*   Updated: 2020/03/23 17:27:36 by lglover          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"

double	norm2(t_v3d v)
{
	return (v.x * v.x + v.y * v.y + v.z * v.z);
}

int		point_on_line(t_v3d v0, t_v3d v1, t_v3d p)
{
	if (v0.x == p.x)
		return (v1.x == p.x);
	if (v0.z == p.z)
		return (v1.z == p.z);
	return ((v0.x - p.x) * (v0.z - p.z) == (p.x - v1.x) * (p.z - v1.z));
}

int		line_intersection(t_v3d v0, t_v3d v1, t_v3d v2, t_v3d v3)
{
	t_v3d	da;
	t_v3d	db;
	double	s;
	double	t;
	t_v3d	tmp;

	da = v3d_sub(v1, v0);
	db = v3d_sub(v3, v2);
	if (v3d_dot(v3d_sub(v2, v0), v3d_cross(da, db)) != 0.0)
		return (0);
	s = v3d_dot(v3d_cross(v3d_sub(v2, v0), db),
				v3d_cross(da, db)) / norm2(v3d_cross(da, db));
	t = v3d_dot(v3d_cross(v3d_sub(v2, v0), da),
				v3d_cross(da, db)) / norm2(v3d_cross(da, db));
	if ((s >= 0.0 && s <= 1.0) && (t >= 0.0 && t <= 1.0))
	{
		tmp = v3d_mul_by(v3d_sum(v0, da), s);
		if ((tmp.x == v0.x && tmp.z == v0.z) ||
			(tmp.x == v1.x && tmp.z == v1.z) ||
			(tmp.x == v3.x && tmp.z == v3.z))
			return (0);
		return (1);
	}
	return (0);
}
