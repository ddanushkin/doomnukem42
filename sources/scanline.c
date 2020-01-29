#include "doom_nukem.h"

void 	blend_color(Uint8 	*c0, Uint8 	*c1, double value)
{
	double delta;

	delta = 1.0 - value;
	c0[0] = delta * c0[0] + value * c1[0];
	c0[1] = delta * c0[1] + value * c1[1];
	c0[2] = delta * c0[2] + value * c1[2];
}

void 	shade_color(t_app *app, int x, int y, Uint32 *c)
{
	Uint8 	*color;
	double	shade;
	double 	*sh;

	sh = &app->rw->shade[0];
	color = (Uint8 *)c;
	shade = sh[y * 10 + x];
	color[0] *= shade;
	color[1] *= shade;
	color[2] *= shade;
}

void 	scanline_set_pixel(t_app *app, t_scanline *d, Uint32 *tex, int offset)
{
	Uint32	c;
	Uint32	img_x;
	Uint32	img_y;
	double	x;
	double	y;

	x = d->tex_x / d->tex_z;
	y = d->tex_y / d->tex_z;
	img_x = (Uint32)(x * d->scale_x * 256.0) % 256;
	img_y = (Uint32)(y * d->scale_y * 256.0) % 256;
	c = tex[((img_y << 8u) + img_x)];
	if (c != TRANSPARENCY_COLOR)
	{
		if (app->cs->ready && !app->is_skybox)
			shade_color(app, x * 10.0, y * 10.0, &c);
		app->depth_buffer[offset] = d->depth;
		set_pixel_uint32(app->sdl->surface, offset, c);
	}
}

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

void	*scanline_thread(void *ptr)
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

void	scan_edges_threads(t_app *app, t_edge *a, t_edge *b, int handedness)
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

	int threads = 16;

	pthread_t thr[threads];
	t_thread_data	thr_data[threads];

	t_scanline_data sl_data[SCREEN_H];
	int	i;

	i = 0;
	while (y < y_end)
	{
		sl_data[i].left = *left;
		sl_data[i].right = *right;
		sl_data[i].y = y;
		edge_step(left);
		edge_step(right);
		y++;
		i++;
	}

	int i_s;
	int i_step;

	i_s = 0;
	i_step = abs(y_end - y_start) / (threads - 1);
	i = 0;
	while (i < (threads - 1) && i_step > 0)
	{
		thr_data[i].app = app;
		thr_data[i].data = &sl_data[0];
		thr_data[i].start = i_s;
		thr_data[i].end = i_s + i_step;
		i_s += i_step;
		pthread_create(&thr[i], NULL, scanline_thread, &thr_data[i]);
		i++;
	}
	if (i_s < abs(y_end - y_start))
	{
		thr_data[i].app = app;
		thr_data[i].data = &sl_data[0];
		thr_data[i].start = i_s;
		thr_data[i].end = abs(y_end - y_start);
		pthread_create(&thr[i], NULL, scanline_thread, &thr_data[i]);
	}
	i = 0;
	while (i < threads)
	{
		pthread_join(thr[i], NULL);
		i++;
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
	scan_edges_threads(app, &top_to_bottom, &top_to_middle, handedness);
	scan_edges_threads(app, &top_to_bottom, &middle_to_bottom, handedness);
}