#include "doom_nukem.h"

void 	scanline_calc(t_scanline *d, t_edge *left, t_edge *right)
{
	d->start = ceil(left->x);
	d->end = ceil(right->x);
	d->pre_step = d->start - left->x;
	d->dist = 1.0 / (right->x - left->x);
	d->x_step = (right->tex_x - left->tex_x) * d->dist;
	d->y_step = (right->tex_y - left->tex_y) * d->dist;
	d->z_step = (right->tex_z - left->tex_z) * d->dist;
	d->d_step = (right->depth - left->depth) * d->dist;
	d->tex_x = left->tex_x + d->x_step * d->pre_step;
	d->tex_y = left->tex_y + d->y_step * d->pre_step;
	d->tex_z = left->tex_z + d->z_step * d->pre_step;
	d->depth = left->depth + d->d_step * d->pre_step;
}

void 	scanline_set_pixel(t_render *r, t_scanline *d, int offset)
{
	Uint32	c;
	double	x;
	double	y;

	x = d->tex_x / d->tex_z;
	y = d->tex_y / d->tex_z;
	c = r->t[((uint8_t)(y * r->scale_y) << 8u) + (uint8_t)(x * r->scale_x)];
	if (c != TRANSPARENCY_COLOR)
	{
		r->depth[offset] = d->depth;
		r->screen[offset] = c;
	}
}

void	scanline(t_edge *left, t_edge *right, t_render *r)
{
	int			offset;
	t_scanline	d;

	scanline_calc(&d, left, right);
	offset = r->y * SCREEN_W + (d.start);
	while (d.start < d.end)
	{
		if (d.depth < r->depth[offset])
			scanline_set_pixel(r, &d, offset);
		d.tex_x += d.x_step;
		d.tex_y += d.y_step;
		d.tex_z += d.z_step;
		d.depth += d.d_step;
		d.start++;
		offset++;
	}
}

void	scan_edges(t_edge *a, t_edge *b, t_render *r)
{
	t_edge	*left;
	t_edge	*right;
	int		y_start;
	int		y_end;

	left = a;
	right = b;
	if (r->handedness)
		SWAP(left, right, t_edge *);
	y_start = b->y_start;
	y_end = b->y_end;
	r->y = y_start;
	while (r->y < y_end)
	{
		scanline(left, right, r);
		edge_step(left);
		edge_step(right);
		r->y++;
	}
}

void	scan_triangle(t_v3d min, t_v3d mid, t_v3d max, t_render *r)
{
	t_edge		top_to_bottom;
	t_edge		top_to_middle;
	t_edge		middle_to_bottom;
	t_gradient	gradient;

	gradient = gradient_new(min, mid, max);
	top_to_bottom = edge_new(gradient, min, max, 0);
	top_to_middle = edge_new(gradient, min, mid, 0);
	middle_to_bottom = edge_new(gradient, mid, max, 1);
	scan_edges(&top_to_bottom, &top_to_middle, r);
	scan_edges(&top_to_bottom, &middle_to_bottom, r);
}