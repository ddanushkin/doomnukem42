/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   intersect.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lglover <lglover@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/11 13:03:36 by lglover           #+#    #+#             */
/*   Updated: 2020/03/11 15:34:10 by lglover          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"

void	ray_floor_ceil(t_app *app, t_v3d p, t_triangle tr)
{
	ray_floor(app, p, tr);
	ray_ceil(app, p, tr);
}

int		ray_intersect(t_app *app, t_v3d v0, t_v3d v1, t_v3d v2)
{
	t_intersect	i;

	i.v0v1 = v3d_sub(v1, v0);
	i.v0v2 = v3d_sub(v2, v0);
	i.pvec = v3d_cross(app->camera->dir, i.v0v2);
	i.det = v3d_dot(i.v0v1, i.pvec);
	if (fabs(i.det) < 0.0)
		return (0);
	i.det = 1 / i.det;
	i.tvec = v3d_sub(app->camera->pos, v0);
	i.u = v3d_dot(i.tvec, i.pvec) * i.det;
	if (i.u < 0 || i.u > 1)
		return (0);
	i.qvec = v3d_cross(i.tvec, i.v0v1);
	i.v = v3d_dot(app->camera->dir, i.qvec) * i.det;
	if (i.v < 0 || i.u + i.v > 1)
		return (0);
	i.t = v3d_dot(i.v0v2, i.qvec) * i.det;
	if (i.t < app->hit_dist && i.t > 0.0)
		return (hit_fill_data(app, i.t));
	return (0);
}

int		ray_ceil(t_app *app, t_v3d pos, t_triangle tr)
{
	t_intersect	i;
	t_v3d		dir;

	dir = new_vector(0.0, 1.0, 0.0);
	i.v0v1 = v3d_sub(tr.v[1], tr.v[0]);
	i.v0v2 = v3d_sub(tr.v[2], tr.v[0]);
	i.pvec = v3d_cross(dir, i.v0v2);
	i.det = v3d_dot(i.v0v1, i.pvec);
	if (fabs(i.det) < 0.0)
		return (0);
	i.det = 1 / i.det;
	i.tvec = v3d_sub(pos, tr.v[0]);
	i.u = v3d_dot(i.tvec, i.pvec) * i.det;
	if (i.u < 0 || i.u > 1)
		return (0);
	i.qvec = v3d_cross(i.tvec, i.v0v1);
	i.v = v3d_dot(dir, i.qvec) * i.det;
	if (i.v < 0 || i.u + i.v > 1)
		return (0);
	i.t = v3d_dot(i.v0v2, i.qvec) * i.det;
	if (i.t < app->ceil_dist && i.t > 0.0)
		return (hit_fill_ceil(app, pos, dir, i.t));
	return (0);
}

int		ray_floor(t_app *app, t_v3d pos, t_triangle tr)
{
	t_intersect	i;
	t_v3d		dir;

	dir = new_vector(0.0, -1.0, 0.0);
	i.v0v1 = v3d_sub(tr.v[1], tr.v[0]);
	i.v0v2 = v3d_sub(tr.v[2], tr.v[0]);
	i.pvec = v3d_cross(dir, i.v0v2);
	i.det = v3d_dot(i.v0v1, i.pvec);
	if (fabs(i.det) < 0.0)
		return (0);
	i.det = 1 / i.det;
	i.tvec = v3d_sub(pos, tr.v[0]);
	i.u = v3d_dot(i.tvec, i.pvec) * i.det;
	if (i.u < 0 || i.u > 1)
		return (0);
	i.qvec = v3d_cross(i.tvec, i.v0v1);
	i.v = v3d_dot(dir, i.qvec) * i.det;
	if (i.v < 0 || i.u + i.v > 1)
		return (0);
	i.t = v3d_dot(i.v0v2, i.qvec) * i.det;
	if (i.t < app->floor_dist && i.t > 0.0)
		return (hit_fill_floor(app, pos, dir, i.t));
	return (0);
}
