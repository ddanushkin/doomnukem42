#include "doom_nukem.h"

void 	scanline_calc(t_sl_data *d, t_edge *left, t_edge *right, int y)
{
	double	dist;
	double	pre_step;

	d->start = ceil(left->x);
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
	d->offset = y * SCREEN_W + d->start;
}

void 	scanline_draw(register t_sl_data *d, uint32_t *t, double *depth, uint32_t *screen)
{
	int			i;
	int 		offset;
	uint32_t 	c;

	i = d->start;
	offset = d->offset;
	while (i < d->end)
	{
		if (d->d < depth[offset])
		{
			c = t[((uint8_t)(d->y / d->z) << 8u) + (uint8_t)(d->x / d->z)];
			if (c != TRANSPARENCY_COLOR)
			{
				depth[offset] = d->d;
				screen[offset] = c;
			}
		}
		d->x += d->xs;
		d->y += d->ys;
		d->z += d->zs;
		d->d += d->ds;
		offset++;
		i++;
	}
}

void	scan_edges(t_edge *a, t_edge *b, t_render *r)
{
	t_edge	*left;
	t_edge	*right;
	int		y_start;
	int		y_end;
	int 	y;

	left = a;
	right = b;
	if (r->handedness)
		SWAP(left, right, t_edge *);
	y_start = b->y_start;
	y_end = b->y_end;
	y = y_start;
	while (y < y_end)
	{
		scanline_calc(&r->sl[r->sl_counter++], left, right, y);
		edge_step(left);
		edge_step(right);
		y++;
	}
}

void	scan_triangle(t_v3d min, t_v3d mid, t_v3d max, t_render *r)
{
	t_edge		top_to_bottom;
	t_edge		top_to_middle;
	t_edge		middle_to_bottom;
	t_gradient	gradient;
	int			i;

	gradient = gradient_new(min, mid, max);
	top_to_bottom = edge_new(gradient, min, max, 0);
	top_to_middle = edge_new(gradient, min, mid, 0);
	middle_to_bottom = edge_new(gradient, mid, max, 1);
	r->sl_counter = 0;
	scan_edges(&top_to_bottom, &top_to_middle, r);
	scan_edges(&top_to_bottom, &middle_to_bottom, r);
	i = 0;
	while (i < r->sl_counter)
	{
		r->sl[i].x *= r->scale_x;
		r->sl[i].y *= r->scale_y;
		r->sl[i].xs *= r->scale_x;
		r->sl[i].ys *= r->scale_y;
		scanline_draw(&r->sl[i++], r->t, r->depth, r->screen);
	}
}