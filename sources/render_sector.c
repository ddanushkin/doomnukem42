/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_sector.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lglover <lglover@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/11 16:30:03 by lglover           #+#    #+#             */
/*   Updated: 2020/03/11 16:32:44 by lglover          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"

void	render_floor_ceil(t_app *app, t_triangle *tr, t_wall *w)
{
	t_v3d	v0;
	t_v3d	v1;
	t_v3d	v2;

	if (!w->active)
		return ;
	v0 = matrix_transform(app->camera->transform, tr->v[0]);
	v1 = matrix_transform(app->camera->transform, tr->v[1]);
	v2 = matrix_transform(app->camera->transform, tr->v[2]);
	w->inside = (vertex_inside(&v0) << 24u) +
				(vertex_inside(&v1) << 16u) +
				(vertex_inside(&v2) << 8u) + 1u;
	app->rw = w;
	ray_intersect(app, tr->v[0], tr->v[1], tr->v[2]);
	render_triangle_0(app, v0, v1, v2);
}

void	render_sector_floor_ceil(t_app *app, t_sector *s)
{
	int		j;

	j = 0;
	app->render_type = floor_ceil;
	while (j < s->trs_count)
	{
		if (s->lava)
		{
			s->ftrs[j].v[0].tex_x += 0.1 * app->timer->delta;
			s->ftrs[j].v[1].tex_x += 0.1 * app->timer->delta;
			s->ftrs[j].v[2].tex_x += 0.1 * app->timer->delta;
		}
		if (s->floor.active)
			render_floor_ceil(app, &s->ftrs[j], &s->floor);
		if (s->ceil.active)
			render_floor_ceil(app, &s->ctrs[j], &s->ceil);
		j++;
	}
}

void	update_door(t_app *app, t_sector *s, double dt)
{
	double amount;

	amount = 2.5 * s->door_dir * dt;
	sector_pts_h(&s->fpts[0], s->pts_count, -amount);
	s->floor_y -= amount;
	if (s->door_dir < 0.0 && fabs(s->ceil_y - s->floor_y) <= 0.15)
	{
		s->floor_y = s->ceil_y - 0.15;
		s->door_anim = 0;
		s->door_dir *= -1.0;
		Mix_HaltChannel(1);
		Mix_PlayChannel(2, app->sfx[25], 0);
	}
	else if (s->door_dir > 0.0 && fabs(s->ceil_y - s->floor_y) >= s->delta_y)
	{
		s->floor_y = s->ceil_y - s->delta_y;
		s->door_anim = 0;
		s->door_dir *= -1.0;
		Mix_HaltChannel(1);
		Mix_PlayChannel(2, app->sfx[25], 0);
	}
	sector_update_height(s, &s->fpts[0], &s->cpts[0]);
}

void	render_sector(t_app *app, t_sector *s)
{
	int		j;

	if (s->door && s->door_anim)
		update_door(app, s, app->timer->delta);
	j = 0;
	app->render_type = npc;
	while (j < s->npcs_count)
		render_billboard(app, &s->npc[j++]);
	j = 0;
	app->render_type = obj;
	while (j < s->objs_count)
		render_billboard(app, &s->obj[j++]);
	j = 0;
	app->render_type = decor;
	while (j < s->decor_count)
		render_wall(app, &s->decor[j++]);
	j = 0;
	app->render_type = wall;
	while (j < s->walls_count)
		render_wall(app, &s->walls[j++]);
	render_sector_floor_ceil(app, s);
}
