#include "doom_nukem.h"

void 	scanline_calc(t_sl_data *d, t_edge *left, t_edge *right, t_render *r)
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

uint32_t	shade(uint8_t shade, uint32_t c)
{
	uint8_t		r;
	uint8_t		g;
	uint8_t		b;

	r = ((c >> 16) & 0xFF) >> shade;
	g = ((c >> 8) & 0xFF) >> shade;
	b = (c & 0xFF) >> shade;
	return ((0xFFu << 25u) | (r << 16u) | (g << 8u) | b);
}

void 	scanline_draw(register t_sl_data *s, register uint32_t *t, register double *depth, register uint32_t *screen)
{
	int			i;
	int			offset;
	uint32_t	c;

	i = s->start;
	offset = s->offset;
	while (i++ < s->end)
	{
		if (depth[offset] - s->d > 0.00001)
		{
			c = t[((uint8_t)(s->y / s->z) << 8u) + (uint8_t)(s->x / s->z)];
			if (c != TRANSPARENCY_COLOR)
			{
				depth[offset] = s->d;
				if (s->shade != 0)
					screen[offset] = shade(s->shade, c);
				else
					screen[offset] = c;
			}
		}
		s->x += s->xs;
		s->y += s->ys;
		s->z += s->zs;
		s->d += s->ds;
		offset++;
	}
}

void	scan_edges(t_edge *a,  t_edge *b, t_render *r)
{
	register t_edge	*left;
	register t_edge	*right;
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
		r->y = y;
		scanline_calc(&r->sl[r->sl_counter++], left, right, r);
		edge_step(left);
		edge_step(right);
		y++;
	}
}

void 	thr_data_set(t_thr_data *thr_data, t_render *r, int start, int end)
{
	thr_data->start = start;
	thr_data->end = end;
	thr_data->r = r;
}

void 	*scanline_thr(register void *ptr)
{
	t_thr_data			*td;
	register t_render	*r;
	int					i;
	int					len;

	td = (t_thr_data *)ptr;
	r = td->r;
	i = td->start;
	len = td->end;
	while (i < len)
		scanline_draw(&r->sl[i++], r->t, r->depth, r->screen);
	return (NULL);
}

void 	scanline_threads(register t_render *r, int size)
{
	int	start;
	int	step;
	int i;
	pthread_t	thr[THREADS_N];
	t_thr_data	thr_data[THREADS_N];

	start = 0;
	step = size / (THREADS_N - 1);
	i = 0;
	while (i < (THREADS_N - 1) && step > 0)
	{
		thr_data_set(&thr_data[i], r, start, start + step);
		pthread_create(&thr[i], NULL, scanline_thr, &thr_data[i]);
		start += step;
		i++;
	}
	if (start < size)
	{
		thr_data_set(&thr_data[i], r, start, size);
		pthread_create(&thr[i], NULL, scanline_thr, &thr_data[i]);
	}
	i = 0;
	while (i < THREADS_N)
		pthread_join(thr[i++], NULL);
}

void	scan_triangle(t_v3d min, t_v3d mid, t_v3d max, t_render *r)
{
	t_edge		top_to_bottom;
	t_edge		top_to_middle;
	t_edge		middle_to_bottom;
	t_gradient	gradient;
	int			i;

	i = 0;
	gradient = gradient_new(min, mid, max);
	top_to_bottom = edge_new(gradient, min, max, 0);
	top_to_middle = edge_new(gradient, min, mid, 0);
	middle_to_bottom = edge_new(gradient, mid, max, 1);
	r->sl_counter = 0;
	scan_edges(&top_to_bottom, &top_to_middle, r);
	scan_edges(&top_to_bottom, &middle_to_bottom, r);
#ifndef MULTITHREAD
	while (i < r->sl_counter)
		scanline_draw(&r->sl[i++], r->t, r->depth, r->screen);
#else
	scanline_threads(r, r->sl_counter);
#endif
}