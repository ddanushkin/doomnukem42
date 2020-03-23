/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sector.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lglover <lglover@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/23 17:28:38 by lglover           #+#    #+#             */
/*   Updated: 2020/03/23 17:28:38 by lglover          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"

void	sector_update_point_h(t_sector *cs, t_v3d *fpts, t_v3d *cpts, int i)
{
	cs->ftrs[i].v[0].y = fpts[cs->ftrs[i].v[0].i].y;
	cs->ftrs[i].v[1].y = fpts[cs->ftrs[i].v[1].i].y;
	cs->ftrs[i].v[2].y = fpts[cs->ftrs[i].v[2].i].y;
	cs->ctrs[i].v[0].y = cpts[cs->ctrs[i].v[0].i].y;
	cs->ctrs[i].v[1].y = cpts[cs->ctrs[i].v[1].i].y;
	cs->ctrs[i].v[2].y = cpts[cs->ctrs[i].v[2].i].y;
}

void	sector_update_shade(t_sector *s)
{
	int			i;

	i = 0;
	while (i < s->walls_count)
		s->walls[i++].shade = s->shade;
	i = 0;
	while (i < s->decor_count)
		s->decor[i++].shade = s->shade;
	s->floor.shade = s->shade;
	s->ceil.shade = s->shade;
}

void	sector_pts_h(t_v3d *pts, int size, double amount)
{
	int		i;

	i = 0;
	while (i < size)
		pts[i++].y += amount;
}

void	sector_update_height(t_sector *cs, t_v3d *fpts, t_v3d *cpts)
{
	int		i;

	i = 0;
	while (i < cs->walls_count)
	{
		cs->walls[i].v[0].y = fpts[cs->walls[i].v[0].i].y;
		cs->walls[i].v[2].y = fpts[cs->walls[i].v[2].i].y;
		cs->walls[i].v[3].y = cpts[cs->walls[i].v[3].i].y;
		cs->walls[i].v[1].y = cpts[cs->walls[i].v[1].i].y;
		wall_update_scale(&cs->walls[i]);
		wall_update_tex(&cs->walls[i]);
		i++;
	}
	i = 0;
	while (i < cs->trs_count)
		sector_update_point_h(cs, fpts, cpts, i++);
	i = 0;
	while (i < cs->npcs_count)
		cs->npc[i++].pos.y = cs->floor_y;
	i = 0;
	while (i < cs->objs_count)
		cs->obj[i++].pos.y = cs->floor_y;
}

void	sector_min_max(t_sector *s)
{
	int		i;
	t_v3d	*v;

	i = 0;
	s->x_min = s->fpts[0].x;
	s->x_max = s->x_min;
	s->z_min = s->fpts[0].z;
	s->z_max = s->z_min;
	while (i < s->pts_count)
	{
		v = &s->fpts[i];
		if (v->x < s->x_min)
			s->x_min = v->x;
		if (v->x > s->x_max)
			s->x_max = v->x;
		if (v->z < s->z_min)
			s->z_min = v->z;
		if (v->z > s->z_max)
			s->z_max = v->z;
		i++;
	}
}
