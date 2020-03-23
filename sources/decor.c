/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   decor.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lglover <lglover@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/23 17:26:28 by lglover           #+#    #+#             */
/*   Updated: 2020/03/23 17:26:28 by lglover          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"

void	decor_set_pos(t_wall *decor, t_v3d lp, t_v3d tp, t_v3d forward)
{
	decor->v[0] = lp;
	decor->v[1] = tp;
	decor->v[2] = decor->v[1];
	decor->v[2].y = decor->v[0].y;
	decor->v[3] = decor->v[0];
	decor->v[3].y = decor->v[1].y;
	move(&decor->v[0], forward, -0.01);
	move(&decor->v[1], forward, -0.01);
	move(&decor->v[2], forward, -0.01);
	move(&decor->v[3], forward, -0.01);
}

void	calc_top_point(t_v3d *lp, t_v3d *tp, double dx, double dz)
{
	tp->x = ((lp->x / DECOR_LEN) + dx / sqrt(dx * dx + dz * dz)) * DECOR_LEN;
	tp->z = ((lp->z / DECOR_LEN) + dz / sqrt(dx * dx + dz * dz)) * DECOR_LEN;
	tp->y = lp->y + DECOR_LEN;
}

t_wall	*decor_add(t_v3d lp, t_sector *cs, t_wall *hit_w, t_camera *cam)
{
	t_wall	*w;
	double	dz;
	double	dx;
	t_v3d	tp;

	w = &cs->decor[cs->decor_next];
	*w = wall_new();
	w->type = decor;
	if (tr_area(hit_w->v[0], hit_w->v[1], hit_w->v[2]) >= 0.0)
		w->flip = 1;
	w->shade = cs->shade;
	dx = hit_w->v[1].x - hit_w->v[0].x;
	dz = hit_w->v[1].z - hit_w->v[0].z;
	calc_top_point(&lp, &tp, dx, dz);
	lp.x -= (tp.x - lp.x) * 0.5;
	lp.z -= (tp.z - lp.z) * 0.5;
	lp.y -= DECOR_LEN_HALF;
	calc_top_point(&lp, &tp, dx, dz);
	decor_set_pos(w, lp, tp, cam->forward);
	wall_reset_tex(w);
	w->sprite = DECOR_SPRITE;
	return (w);
}
