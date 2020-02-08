#include "doom_nukem.h"

void 	shade_color(double shade, register uint32_t *c)
{
	uint8_t	*color;

	color = (uint8_t *)c;
	color[0] *= shade;
	color[1] *= shade;
	color[2] *= shade;
}

void 	scanline_calc(t_sl_data *d, t_edge *left, t_edge *right, int y)
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
	d->offset = y * SCREEN_W + d->start;
	d->shx = d->x * 256;
	d->shy = d->y * 256;
	d->shsx = d->xs * 256;
	d->shsy = d->ys * 256;
}

void 	scanline_draw(register t_sl_data *d, register uint32_t *t, register double *depth, register uint32_t *screen, register double *shade)
{
	register int	i;
	register int	offset;
	uint32_t		c;
	uint16_t 		t_offset;

	i = d->start;
	offset = d->offset;
	while (i++ < d->end)
	{
		if (d->d < depth[offset])
			{
			t_offset = ((uint8_t)(d->y / d->z) << 8u) + (d->x / d->z);
			c = t[t_offset];
			if (c != TRANSPARENCY_COLOR)
			{
				t_offset = ((uint)(d->shy / d->z) << 8u) + (d->shx / d->z);
				shade_color(shade[t_offset], &c);
				depth[offset] = d->d;
				screen[offset] = c;
			}
		}
		d->x += d->xs;
		d->y += d->ys;
		d->z += d->zs;
		d->d += d->ds;
		d->shx += d->shsx;
		d->shy += d->shsy;
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
		scanline_calc(&r->sl[r->sl_counter], left, right, y);
		r->sl[r->sl_counter].x *= r->scale_x;
		r->sl[r->sl_counter].y *= r->scale_y;
		r->sl[r->sl_counter].xs *= r->scale_x;
		r->sl[r->sl_counter++].ys *= r->scale_y;
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
	register t_thr_data	*td;
	register t_render	*r;
	int					i;
	int 				len;

	td = (t_thr_data *)ptr;
	r = td->r;
	i = td->start;
	len = td->end;
	while (i < len)
		scanline_draw(&r->sl[i++], r->t, r->depth, r->screen, r->shade);
	return (NULL);
}

void 	scanline_threads(t_render *r, int size)
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
//	int			i;

	gradient = gradient_new(min, mid, max);
	top_to_bottom = edge_new(gradient, min, max, 0);
	top_to_middle = edge_new(gradient, min, mid, 0);
	middle_to_bottom = edge_new(gradient, mid, max, 1);
	r->sl_counter = 0;
	scan_edges(&top_to_bottom, &top_to_middle, r);
	scan_edges(&top_to_bottom, &middle_to_bottom, r);
	scanline_threads(r, r->sl_counter);
//	i = 0;
//	while (i < r->sl_counter)
//		scanline_draw(&r->sl[i++], r->t, r->depth, r->screen);
}