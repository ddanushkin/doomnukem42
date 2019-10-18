#include "doom_nukem.h"

void	fill_flat_bot(t_app *app, t_triangle *t)
{
	double	m0;
	double	m1;

	m0 = (t->v[1].x - t->v[0].x) / (t->v[1].y - t->v[0].y);
	m1 = (t->v[2].x - t->v[0].x) / (t->v[2].y - t->v[0].y);

	int		y_start;
	int		y_end;
	int		y;
	int		x;

	y_start = (int)ceilf(t->v[0].y - 0.5);
	y_end = (int)ceilf(t->v[2].y - 0.5);

	y = y_start;
	while (y < y_end)
	{
		int 	x_start;
		int 	x_end;
		double	px0;
		double	px1;

		px0 = m0 * ((double)y + 0.5 - t->v[0].y) + t->v[0].x;
		px1 = m1 * ((double)y + 0.5 - t->v[0].y) + t->v[0].x;

		x_start = (int)ceil(px0 - 0.5);
		x_end = (int)ceil(px1 - 0.5);

		x = x_start;
		while (x < x_end)
		{
			set_pixel(app->sdl->surface, x, y, &t->color, app);
			x++;
		}
		y++;
	}
}

void	fill_flat_top(t_app *app, t_triangle *t)
{
	double	m0;
	double	m1;

	m0 = (t->v[2].x - t->v[0].x) / (t->v[2].y - t->v[0].y);
	m1 = (t->v[2].x - t->v[1].x) / (t->v[2].y - t->v[1].y);

	int		y_start;
	int		y_end;
	int		y;
	int		x;

	y_start = (int)ceil(t->v[0].y - 0.5);
	y_end = (int)ceil(t->v[2].y - 0.5);

	y = y_start;
	while (y < y_end)
	{
		int 	x_start;
		int 	x_end;
		double	px0;
		double	px1;

		px0 = m0 * ((double)y + 0.5 - t->v[0].y) + t->v[0].x;
		px1 = m1 * ((double)y + 0.5 - t->v[1].y) + t->v[1].x;

		x_start = (int)ceil(px0 - 0.5);
		x_end = (int)ceil(px1 - 0.5);

		x = x_start;
		while (x < x_end)
		{
			set_pixel(app->sdl->surface, x, y, &t->color, app);
			x++;
		}
		y++;
	}
}

void	sort_triangle_by_y(t_triangle *in)
{
	if (in->v[1].y < in->v[0].y)
		SWAP(in->v[0], in->v[1], t_v3d);
	if (in->v[2].y < in->v[1].y)
		SWAP(in->v[1], in->v[2], t_v3d);
	if (in->v[1].y < in->v[0].y)
		SWAP(in->v[0], in->v[1], t_v3d);
}

void	fill_triangle(t_app *app, t_triangle t)
{
	sort_triangle_by_y(&t);

	if (t.v[0].y == t.v[1].y)
	{
		if (t.v[1].x < t.v[0].x)
			SWAP(t.v[0], t.v[1], t_v3d);
		fill_flat_top(app, &t);
	}
	else if (t.v[1].y == t.v[2].y)
	{
		if (t.v[2].x < t.v[1].x)
			SWAP(t.v[1], t.v[2], t_v3d);
		fill_flat_bot(app, &t);
	}
	else
	{
		double		split_alpha;
		t_v3d		v_tmp;
		t_triangle	tmp_triangle;

		split_alpha =
			(t.v[1].y - t.v[0].y) /
			(t.v[2].y - t.v[0].y);

		double vx;
		double vy;
		double vz;

		vx = t.v[0].x + (t.v[2].x - t.v[0].x) * split_alpha;
		vy = t.v[0].y + (t.v[2].y - t.v[0].y) * split_alpha;
		vz = t.v[0].z + (t.v[2].z - t.v[0].z) * split_alpha;
		set_vector(&v_tmp, vx, vy, vz);

		tmp_triangle.color = t.color;
		if (t.v[1].x < vx)
		{
			tmp_triangle.v[0] = t.v[0];
			tmp_triangle.v[1] = t.v[1];
			tmp_triangle.v[2] = v_tmp;
			fill_flat_bot(app, &tmp_triangle);

			tmp_triangle.v[0] = t.v[1];
			tmp_triangle.v[1] = v_tmp;
			tmp_triangle.v[2] = t.v[2];
			fill_flat_top(app, &tmp_triangle);
		}
		else
		{
			tmp_triangle.v[0] = t.v[0];
			tmp_triangle.v[1] = v_tmp;
			tmp_triangle.v[2] = t.v[1];
			fill_flat_bot(app, &tmp_triangle);

			tmp_triangle.v[0] = v_tmp;
			tmp_triangle.v[1] = t.v[1];
			tmp_triangle.v[2] = t.v[2];
			fill_flat_top(app, &tmp_triangle);
		}
	}
}