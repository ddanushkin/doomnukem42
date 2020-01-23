#include "doom_nukem.h"

void 	scanline_set_pixel(t_app *app, t_scanline *d, Uint32 *tex, int offset)
{
	Uint32	c;
	Uint8 	*color;
	Uint32	img_x;
	Uint32	img_y;

	img_x = (Uint32)(d->tex_x / d->tex_z * d->scale_x * 256.0) % 256;
	img_y = (Uint32)(d->tex_y / d->tex_z * d->scale_y * 256.0) % 256;
	c = tex[((img_y << 8u) + img_x)];
	if (c != TRANSPARENCY_COLOR)
	{
		color = (Uint8 *)&c;
		double shade = d->tex_z;
		shade = CLAMP(shade, 0.0, 1.0);
		color[0] *= shade;
		color[1] *= shade;
		color[2] *= shade;
		app->depth_buffer[offset] = d->depth;
		set_pixel_uint32(app->sdl->surface, offset, c);
	}
}

void 	scanline_calc(t_scanline *d, t_edge *left, t_edge *right)
{
	d->start = (int)ceil(left->x);
	d->end = (int)ceil(right->x);
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

void	scanline(t_app *app, t_edge *left, t_edge *right, int y)
{
	t_wall		*w;
	Uint32		*texture;
	int			offset;
	t_scanline	d;

	w = app->render_wall;
	texture = app->sprites[w->sprite_index].pixels;
	scanline_calc(&d, left, right);
	offset = y * SCREEN_W + d.start;
	d.scale_x = w->scale_x;
	d.scale_y = w->scale_y;
	while (d.start < d.end)
	{
		if (d.depth < app->depth_buffer[offset])
			scanline_set_pixel(app, &d, texture, offset);
		d.tex_x += d.x_step;
		d.tex_y += d.y_step;
		d.tex_z += d.z_step;
		d.depth += d.d_step;
		d.start++;
		offset++;
	}
}

void	scan_edges(t_app *app, t_edge *a, t_edge *b, int handedness)
{
	t_edge	*left;
	t_edge	*right;
	int		y_start;
	int		y_end;
	int		y;

	left = a;
	right = b;
	if (handedness)
		SWAP(left, right, t_edge *);
	y_start = b->y_start;
	y_end = b->y_end;
	y = y_start;
	while (y < y_end)
	{
		scanline(app, left, right, y);
		edge_step(left);
		edge_step(right);
		y++;
	}
}

void	scan_triangle(t_app *app, t_v3d min, t_v3d mid, t_v3d max, int handedness)
{
	t_edge		top_to_bottom;
	t_edge		top_to_middle;
	t_edge		middle_to_bottom;
	t_gradient	gradient;

	gradient = gradient_new(min, mid, max);
	top_to_bottom = edge_new(gradient, min, max, 0);
	top_to_middle = edge_new(gradient, min, mid, 0);
	middle_to_bottom = edge_new(gradient, mid, max, 1);
	scan_edges(app, &top_to_bottom, &top_to_middle, handedness);
	scan_edges(app, &top_to_bottom, &middle_to_bottom, handedness);
}