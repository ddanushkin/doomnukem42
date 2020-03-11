/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_map.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lglover <lglover@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/11 13:03:40 by lglover           #+#    #+#             */
/*   Updated: 2020/03/11 16:33:03 by lglover          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"

void	billboard_face_player(t_app *app, t_wall *w)
{
	t_camera	*c;
	double		half_size;
	t_v3d		r;
	t_v3d		u;
	t_v3d		pos;

	pos = w->pos;
	pos.y += w->size * 0.5;
	c = app->camera;
	half_size = 0.5 * w->size;
	r = v3d_mul_by(c->right, half_size);
	u = v3d_mul_by(c->up, half_size);
	w->v[0] = v3d_sub(v3d_sub(pos, r), u);
	w->v[1] = v3d_sum(v3d_sum(pos, r), u);
	w->v[3] = v3d_sum(v3d_sub(pos, r), u);
	w->v[2] = v3d_sub(v3d_sum(pos, r), u);
	wall_reset_tex(w);
}

void	billboard_switch_sprite(t_app *app, t_wall *w)
{
	int		quad;

	quad = app->camera->quad + w->ori;
	quad = (quad % 8 + 8) % 8;
	if (quad == 2)
		w->sprite = 482;
	if (quad == 6)
		w->sprite = 486;
	if (quad == 5 || quad == 7)
		w->sprite = 485;
	if (quad == 1 || quad == 3)
		w->sprite = 483;
	if (quad == 4 || quad == 0)
		w->sprite = 484;
	if (quad == 1 || quad == 0 || quad == 7)
	{
		SWAP(w->v[3].tex_x, w->v[1].tex_x, double);
		SWAP(w->v[3].tex_y, w->v[1].tex_y, double);
		SWAP(w->v[0].tex_x, w->v[2].tex_x, double);
		SWAP(w->v[0].tex_y, w->v[2].tex_y, double);
	}
}

void	render_billboard(t_app *app, t_wall *w)
{
	t_v3d	v0;
	t_v3d	v1;
	t_v3d	v2;
	t_v3d	v3;

	billboard_face_player(app, w);
	if (w->rotate)
		billboard_switch_sprite(app, w);
	v0 = matrix_transform(app->camera->transform, w->v[0]);
	v1 = matrix_transform(app->camera->transform, w->v[1]);
	v2 = matrix_transform(app->camera->transform, w->v[2]);
	v3 = matrix_transform(app->camera->transform, w->v[3]);
	if (wall_outside(&v0, &v1, &v2, &v3))
		return ;
	w->inside = wall_inside(&v0, &v1, &v2, &v3);
	app->rw = w;
	render_triangle_0(app, v0, v1, v2);
	render_triangle_1(app, v0, v3, v1);
	if (!ray_intersect(app, w->v[0], w->v[1], w->v[2]))
		ray_intersect(app, w->v[0], w->v[3], w->v[1]);
}

void	render_wall(register t_app *app, register t_wall *w)
{
	t_v3d	v0;
	t_v3d	v1;
	t_v3d	v2;
	t_v3d	v3;

	if (!w->active)
		return ;
	v0 = matrix_transform(app->camera->transform, w->v[0]);
	v1 = matrix_transform(app->camera->transform, w->v[1]);
	v2 = matrix_transform(app->camera->transform, w->v[2]);
	v3 = matrix_transform(app->camera->transform, w->v[3]);
	if (wall_outside(&v0, &v1, &v2, &v3))
		return ;
	w->inside = wall_inside(&v0, &v1, &v2, &v3);
	app->rw = w;
	render_triangle_0(app, v0, v1, v2);
	render_triangle_1(app, v0, v3, v1);
	if (!ray_intersect(app, w->v[0], w->v[1], w->v[2]))
		ray_intersect(app, w->v[0], w->v[3], w->v[1]);
}

void	render_map(t_app *app)
{
	int		i;

	i = 0;
	app->hit_dist = 10000.0;
	app->hit_wall = NULL;
	app->hit_sector = NULL;
	if (app->md.card_set && !app->md.card_picked)
	{
		app->cs = &app->sectors[app->md.card_sector];
		app->render_type = obj;
		render_billboard(app, &app->card_w);
	}
	while (i < app->sectors_count)
	{
		app->cs = &app->sectors[i];
		render_sector(app, &app->sectors[i++]);
	}
	render_skybox(app, app->skybox);
}
