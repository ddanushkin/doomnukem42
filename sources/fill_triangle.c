#include "doom_nukem.h"

void	fill_flat_bot(t_app *app, t_triangle *t)
{
	float	m0;
	float	m1;

	m0 = (t->v[1].x - t->v[0].x) / (t->v[1].y - t->v[0].y);
	m1 = (t->v[2].x - t->v[0].x) / (t->v[2].y - t->v[0].y);

	int		y_start;
	int		y_end;
	int		y;
	int		x;

	y_start = (int)ceilf(t->v[0].y - 0.5f);
	y_end = (int)ceilf(t->v[2].y - 0.5f);

	y = y_start;
	while (y < y_end)
	{
		int 	x_start;
		int 	x_end;
		float	px0;
		float	px1;

		px0 = m0 * ((float)y + 0.5f - t->v[0].y) + t->v[0].x;
		px1 = m1 * ((float)y + 0.5f - t->v[0].y) + t->v[0].x;

		x_start = (int)ceilf(px0 - 0.5f);
		x_end = (int)ceilf(px1 - 0.5f);

		x = x_start;
		while (x < x_end)
		{
			set_pixel(&app->screen, x, y, &t->color);
			x++;
		}
		y++;
	}
}

void	fill_flat_top(t_app *app, t_triangle *t)
{
	float	m0;
	float	m1;

	m0 = (t->v[2].x - t->v[0].x) / (t->v[2].y - t->v[0].y);
	m1 = (t->v[2].x - t->v[1].x) / (t->v[2].y - t->v[1].y);

	int		y_start;
	int		y_end;
	int		y;
	int		x;

	y_start = (int)ceilf(t->v[0].y - 0.5f);
	y_end = (int)ceilf(t->v[2].y - 0.5f);

	y = y_start;
	while (y < y_end)
	{
		int 	x_start;
		int 	x_end;
		float	px0;
		float	px1;

		px0 = m0 * ((float)y + 0.5f - t->v[0].y) + t->v[0].x;
		px1 = m1 * ((float)y + 0.5f - t->v[1].y) + t->v[1].x;

		x_start = (int)ceilf(px0 - 0.5f);
		x_end = (int)ceilf(px1 - 0.5f);

		x = x_start;
		while (x < x_end)
		{
			set_pixel(&app->screen, x, y, &t->color);
			x++;
		}
		y++;
	}
}

void	sort_triangle_by_y(t_triangle *in, t_triangle *out)
{
	set_vector(&out->v[0], in->v[0].x, in->v[0].y, in->v[0].z);
	set_vector(&out->v[1], in->v[1].x, in->v[1].y, in->v[1].z);
	set_vector(&out->v[2], in->v[2].x, in->v[2].y, in->v[2].z);
	if (out->v[1].y < out->v[0].y)
		SWAP(out->v[0], out->v[1], t_vector);
	if (out->v[2].y < out->v[1].y)
		SWAP(out->v[1], out->v[2], t_vector);
	if (out->v[1].y < out->v[0].y)
		SWAP(out->v[0], out->v[1], t_vector);
}

void	fill_triangle(t_app *app, t_triangle *t)
{
	t_triangle	sorted;

	sort_triangle_by_y(t, &sorted);
	sorted.color = t->color;

	if (sorted.v[0].y == sorted.v[1].y)
	{
		if (sorted.v[1].x < sorted.v[0].x)
			SWAP(sorted.v[0], sorted.v[1], t_vector);
		fill_flat_top(app, &sorted);
	}
	else if (sorted.v[1].y == sorted.v[2].y)
	{
		if (sorted.v[2].x < sorted.v[1].x)
			SWAP(sorted.v[1], sorted.v[2], t_vector);
		fill_flat_bot(app, &sorted);
	}
	else
	{
		float		split_alpha;
		t_vector	v_tmp;
		t_triangle	tmp_triangle;

		split_alpha =
			(sorted.v[1].y - sorted.v[0].y) /
			(sorted.v[2].y - sorted.v[0].y);

		float vx;
		float vy;
		float vz;

		vx = sorted.v[0].x + (sorted.v[2].x - sorted.v[0].x) * split_alpha;
		vy = sorted.v[0].y + (sorted.v[2].y - sorted.v[0].y) * split_alpha;
		vz = sorted.v[0].z + (sorted.v[2].z - sorted.v[0].z) * split_alpha;
		set_vector(&v_tmp, vx, vy, vz);

		tmp_triangle.color = t->color;
		if (sorted.v[1].x < vx)
		{
			tmp_triangle.v[0] = sorted.v[0];
			tmp_triangle.v[1] = sorted.v[1];
			tmp_triangle.v[2] = v_tmp;
			fill_flat_bot(app, &tmp_triangle);

			tmp_triangle.v[0] = sorted.v[1];
			tmp_triangle.v[1] = v_tmp;
			tmp_triangle.v[2] = sorted.v[2];
			fill_flat_top(app, &tmp_triangle);
		}
		else
		{
			tmp_triangle.v[0] = sorted.v[0];
			tmp_triangle.v[1] = v_tmp;
			tmp_triangle.v[2] = sorted.v[1];
			fill_flat_bot(app, &tmp_triangle);

			tmp_triangle.v[0] = v_tmp;
			tmp_triangle.v[1] = sorted.v[1];
			tmp_triangle.v[2] = sorted.v[2];
			fill_flat_top(app, &tmp_triangle);
		}
	}
}
