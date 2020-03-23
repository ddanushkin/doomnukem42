/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scanline_triangle.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lglover <lglover@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/23 17:28:36 by lglover           #+#    #+#             */
/*   Updated: 2020/03/23 17:28:36 by lglover          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"

void		scanline_calc(t_sl_data *d, t_edge *left, t_edge *right,
		t_render *r)
{
	double	dist;
	double	pre_step;

	d->start = ceil(left->x);
	d->start--;
	d->end = ceil(right->x);
	pre_step = d->start - left->x;
	dist = 1.0 / (right->x - left->x);
	d->xs = (right->tex_x - left->tex_x) * dist;
	d->ys = (right->tex_y - left->tex_y) * dist;
	d->zs = (right->tex_z - left->tex_z) * dist;
	d->ds = (right->depth - left->depth) * dist;
	d->x = left->tex_x + d->xs * pre_step;
	d->y = left->tex_y + d->ys * pre_step;
	d->z = left->tex_z + d->zs * pre_step;
	d->d = left->depth + d->ds * pre_step;
	d->offset = r->y * SCREEN_W + d->start;
	d->x *= 256 * r->scale_x;
	d->y *= 256 * r->scale_y;
	d->xs *= 256 * r->scale_x;
	d->ys *= 256 * r->scale_y;
	d->shade = r->shade;
}

void		scan_edges(t_edge *a, t_edge *b, t_render *r)
{
	register t_edge	*left;
	register t_edge	*right;
	int				y_start;
	int				y_end;
	int				y;

	left = a;
	right = b;
	if (r->handedness)
		SWAP(left, right, t_edge *);
	y_start = b->y_start;
	y_end = b->y_end;
	y = y_start;
	while (y < y_end)
	{
		r->y = y;
		scanline_calc(&r->sl[r->sl_counter++], left, right, r);
		edge_step(left);
		edge_step(right);
		y++;
	}
}

void		scan_triangle(t_v3d min, t_v3d mid, t_v3d max, t_render *r)
{
	t_edge		top_to_bottom;
	t_edge		top_to_middle;
	t_edge		middle_to_bottom;
	t_gradient	gradient;

	gradient = gradient_new(min, mid, max);
	top_to_bottom = edge_new(gradient, min, max, 0);
	top_to_middle = edge_new(gradient, min, mid, 0);
	middle_to_bottom = edge_new(gradient, mid, max, 1);
	r->sl_counter = 0;
	scan_edges(&top_to_bottom, &top_to_middle, r);
	scan_edges(&top_to_bottom, &middle_to_bottom, r);
	scanline_threads(r, r->sl_counter);
}
