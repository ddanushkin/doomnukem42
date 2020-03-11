/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sector_close.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lglover <lglover@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/11 16:42:44 by lglover           #+#    #+#             */
/*   Updated: 2020/03/11 16:51:01 by lglover          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"

void	sector_copy_points(t_sector *s, t_v3d *p, int len)
{
	if (get_orientation(p, len) < 0)
		sector_copy_v_1(s, p, len);
	else
		sector_copy_v_2(s, p, len);
}

void	sector_make_walls(t_sector *s)
{
	int		i;
	t_wall	*w;

	i = 0;
	while (i < s->pts_count)
	{
		w = &s->walls[i];
		*w = wall_new();
		w->v[0] = s->fpts[i];
		w->v[2] = s->fpts[i + 1 != s->pts_count ? i + 1 : 0];
		w->v[3] = s->cpts[i];
		w->v[1] = s->cpts[i + 1 != s->pts_count ? i + 1 : 0];
		wall_reset_tex(w);
		wall_update_scale(w);
		i++;
	}
	s->walls_count = s->pts_count;
}

int		sector_close_check(t_v3d *points, int size)
{
	int		i;

	if (size < 3)
		return (1);
	i = 1;
	while (i < size - 1)
	{
		if (line_intersection(
				points[i],
				points[i + 1],
				points[size - 1],
				points[0]) ||
			point_on_line(
					points[size - 1],
					points[0],
					points[i]))
		{
			return (0);
		}
		i++;
	}
	return (1);
}

void	copy_triangles(t_sector *s)
{
	int			i;
	t_triangle	tr;

	i = 0;
	while (i < s->trs_count)
	{
		SWAP(s->ftrs[i].v[0], s->ftrs[i].v[2], t_v3d);
		tr = s->ftrs[i];
		SWAP(tr.v[0], tr.v[2], t_v3d);
		tr.v[0].y = s->ceil_y;
		tr.v[1].y = s->ceil_y;
		tr.v[2].y = s->ceil_y;
		s->ctrs[i++] = tr;
	}
}

void	sector_close(t_app *app, t_sector *s)
{
	if (!sector_close_check(&app->points[0], app->points_count))
	{
		app->bad_close = 0.25;
		return ;
	}
	sector_new(app, s);
	sector_copy_points(s, &app->points[0], app->points_count);
	sector_min_max(s);
	s->floor.sx = fabs(s->x_min - s->x_max) * 0.5;
	s->floor.sy = fabs(s->z_min - s->z_max) * 0.5;
	s->ceil.sx = fabs(s->x_min - s->x_max) * 0.5;
	s->ceil.sy = fabs(s->z_min - s->z_max) * 0.5;
	triangulate(&s->ftrs[0], &s->trs_count, points_to_list(s));
	copy_triangles(s);
	sector_make_walls(s);
	sector_update_shade(s);
	app->cs = s;
	app->sectors_count++;
	app->points_count = 0;
}
