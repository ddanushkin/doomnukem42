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

void 	scanline_set_pixel(t_app *app, t_scanline *d, Uint32 *tex, int offset)
{
	Uint32	c;
	double	x;
	double	y;
	double	z;
	Uint8	*color;

	z = 1.0 / d->tex_z;
	x = d->tex_x * z;
	y = d->tex_y * z;
	c = (tex[(((uint32_t)(y * d->scale_y) >> 8) & 0xff00) +
			 (((uint32_t)(x * d->scale_x) >> 16) & 0xff)]);
	if (c != TRANSPARENCY_COLOR)
	{
		color = (Uint8 *)&c;
		app->shade = app->rw->shade[(int)(y * 10.0) * 10 +
									(int)(x * 10.0)];
		color[0] *= app->shade;
		color[1] *= app->shade;
		color[2] *= app->shade;
		app->depth_buffer[offset] = d->depth;
		app->screen[offset] = c;
	}
}

void	scanline(t_app *app, t_edge *left, t_edge *right, int y)
{
	t_wall		*w;
	Uint32		*texture;
	int			offset;
	t_scanline	d;

	w = app->rw;
	texture = app->sprites[w->sprite].pixels;
	scanline_calc(&d, left, right);
	offset = y * SCREEN_W + (d.start);
	d.scale_x = w->scale_x * 16777216;
	d.scale_y = w->scale_y * 16777216;
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

void	*scanline_thr(void *ptr)
{
	t_thread_data	*data;
	t_scanline_data	*sl;
	int		i;

	data = (t_thread_data *)ptr;
	i = data->start;
	while (i < data->end)
	{
		sl = &data->data[i];
		scanline(data->app, &(sl->left), &(sl->right), sl->y);
		i++;
	}
	return (NULL);
}

void 	start_scanline_thread(t_app *app, int thr_id, int start, int end)
{
	t_thread_data *thr_data;

	thr_data = &app->thr_data[thr_id];
	thr_data->app = app;
	thr_data->data = &app->sl_data[0];
	thr_data->start = start;
	thr_data->end = end;
	pthread_create(&app->thr[thr_id], NULL, scanline_thr, thr_data);
}

void 	scanline_threads(t_app *app, int size)
{
	int	start;
	int	step;
	int i;

	start = 0;
	step = size / (THREADS_N - 1);
	i = 0;
	while (i < (THREADS_N - 1) && step > 0)
	{
		start_scanline_thread(app, i, start, start + step);
		start += step;
		i++;
	}
	if (start < size)
		start_scanline_thread(app, i, start, size);
	i = 0;
	while (i < THREADS_N)
		pthread_join(app->thr[i++], NULL);
}

void	scan_edges_thread(t_app *app, t_edge *a, t_edge *b, int handedness)
{
	t_edge	*left;
	t_edge	*right;
	int		y_start;
	int		y_end;

	left = a;
	right = b;
	if (handedness)
		SWAP(left, right, t_edge *);
	y_start = b->y_start;
	y_end = b->y_end;
	while (y_start < y_end)
	{
		app->sl_data[app->sl_counter].left = *left;
		app->sl_data[app->sl_counter].right = *right;
		app->sl_data[app->sl_counter].y = y_start;
		edge_step(left);
		edge_step(right);
		y_start++;
		app->sl_counter++;
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
	app->sl_counter = 0;
	scan_edges_thread(app, &top_to_bottom, &top_to_middle, handedness);
	scan_edges_thread(app, &top_to_bottom, &middle_to_bottom, handedness);
	scanline_threads(app, app->sl_counter);
}