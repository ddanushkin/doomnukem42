#include "doom_nukem.h"

void	fill_flat_bot(t_app *app, t_triangle *t)
{
	float	m0;
	float	m1;

	m0 = (t->v[1]->x - t->v[0]->x) / (t->v[1]->y - t->v[0]->y);
	m1 = (t->v[2]->x - t->v[0]->x) / (t->v[2]->y - t->v[0]->y);

	int		y_start;
	int		y_end;
	int		y;
	int		x;

	y_start = (int)ceilf(t->v[0]->y - 0.5f);
	y_end = (int)ceilf(t->v[2]->y - 0.5f);

	y = y_start;
	while (y < y_end)
	{
		int 	x_start;
		int 	x_end;
		float	px0;
		float	px1;

		px0 = m0 * ((float)y + 0.5f - t->v[0]->y) + t->v[0]->x;
		px1 = m1 * ((float)y + 0.5f - t->v[0]->y) + t->v[0]->x;

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

	m0 = (t->v[2]->x - t->v[0]->x) / (t->v[2]->y - t->v[0]->y);
	m1 = (t->v[2]->x - t->v[1]->x) / (t->v[2]->y - t->v[1]->y);

	int		y_start;
	int		y_end;
	int		y;
	int		x;

	y_start = (int)ceilf(t->v[0]->y - 0.5f);
	y_end = (int)ceilf(t->v[2]->y - 0.5f);

	y = y_start;
	while (y < y_end)
	{
		int 	x_start;
		int 	x_end;
		float	px0;
		float	px1;

		px0 = m0 * ((float)y + 0.5f - t->v[0]->y) + t->v[0]->x;
		px1 = m1 * ((float)y + 0.5f - t->v[1]->y) + t->v[1]->x;

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

void	sort_triangle_by_y(t_triangle *in)
{
	if (in->v[1]->y < in->v[0]->y)
		SWAP(in->v[0], in->v[1], t_vertex *);
	if (in->v[2]->y < in->v[1]->y)
		SWAP(in->v[1], in->v[2], t_vertex *);
	if (in->v[1]->y < in->v[0]->y)
		SWAP(in->v[0], in->v[1], t_vertex *);
}

void	fill_triangle(t_app *app, t_triangle t)
{
	sort_triangle_by_y(&t);

	if (t.v[0]->y == t.v[1]->y)
	{
		if (t.v[1]->x < t.v[0]->x)
			SWAP(t.v[0], t.v[1], t_vertex *);
		fill_flat_top(app, &t);
	}
	else if (t.v[1]->y == t.v[2]->y)
	{
		if (t.v[2]->x < t.v[1]->x)
			SWAP(t.v[1], t.v[2], t_vertex *);
		fill_flat_bot(app, &t);
	}
	else
	{
		float		split_alpha;
		t_vertex	v_tmp;
		t_triangle	tmp_triangle;

		split_alpha =
			(t.v[1]->y - t.v[0]->y) /
			(t.v[2]->y - t.v[0]->y);

		float vx;
		float vy;
		float vz;

		vx = t.v[0]->x + (t.v[2]->x - t.v[0]->x) * split_alpha;
		vy = t.v[0]->y + (t.v[2]->y - t.v[0]->y) * split_alpha;
		vz = t.v[0]->z + (t.v[2]->z - t.v[0]->z) * split_alpha;
		set_vertex(&v_tmp, vx, vy, vz);

		tmp_triangle.color = t.color;
		if (t.v[1]->x < vx)
		{
			tmp_triangle.v[0] = t.v[0];
			tmp_triangle.v[1] = t.v[1];
			tmp_triangle.v[2] = &v_tmp;
			fill_flat_bot(app, &tmp_triangle);

			tmp_triangle.v[0] = t.v[1];
			tmp_triangle.v[1] = &v_tmp;
			tmp_triangle.v[2] = t.v[2];
			fill_flat_top(app, &tmp_triangle);
		}
		else
		{
			tmp_triangle.v[0] = t.v[0];
			tmp_triangle.v[1] = &v_tmp;
			tmp_triangle.v[2] = t.v[1];
			fill_flat_bot(app, &tmp_triangle);

			tmp_triangle.v[0] = &v_tmp;
			tmp_triangle.v[1] = t.v[1];
			tmp_triangle.v[2] = t.v[2];
			fill_flat_top(app, &tmp_triangle);
		}
	}
}

void	fill_flat_delta(t_vector_i *v, t_vertex *v1, t_vertex *v2)
{
	v->x = (int)fabsf(v2->x - v1->x);
	v->y = (int)fabsf(v2->y - v1->y);
	v->z = 0;
}

void fillFlatSideTriangleInt(t_app *app, t_triangle t)
{
	t_vertex v_tmp1;
	t_vertex v_tmp2;
	
	set_vertex(&v_tmp1, t.v[0]->x, t.v[0]->y, 0);
	set_vertex(&v_tmp2, t.v[0]->x, t.v[0]->y, 0);

	int		changed1;
	int		changed2;

	changed1 = 0;
	changed2 = 0;

	t_vector_i	dt1;
	t_vector_i	dt2;

	fill_flat_delta(&dt1, t.v[1], t.v[0]);
	fill_flat_delta(&dt2, t.v[2], t.v[0]);

	int signx1 = SIGN(t.v[1]->x - t.v[0]->x);
	int signx2 = SIGN(t.v[2]->x - t.v[0]->x);

	int signy1 = SIGN(t.v[1]->y - t.v[0]->y);
	int signy2 = SIGN(t.v[2]->y - t.v[0]->y);

	if (dt1.y > dt1.x)
	{
		SWAP(dt1.x, dt1.y, int);
		changed1 = 1;
	}

	if (dt2.y > dt2.x)
	{
		SWAP(dt2.x, dt2.y, int);
		changed2 = 1;
	}

	int e1 = 2 * dt1.y - dt1.x;
	int e2 = 2 * dt2.y - dt2.x;

	int i;

	i = 0;

	while (i <= dt1.x)
	{
		draw_line(app, v_tmp1, v_tmp2, &t.color);

		while (e1 >= 0)
		{
			if (changed1)
				v_tmp1.x += (float)signx1;
			else
				v_tmp1.y += (float)signy1;
			e1 = e1 - 2 * dt1.x;
		}

		if (changed1)
			v_tmp1.y += (float)signy1;
		else
			v_tmp1.x += (float)signx1;

		e1 = e1 + 2 * dt1.y;

		while (v_tmp2.y != v_tmp1.y)
		{
			while (e2 >= 0)
			{
				if (changed2)
					v_tmp2.x += (float)signx2;
				else
					v_tmp2.y += (float)signy2;
				e2 = e2 - 2 * dt2.x;
			}

			if (changed2)
				v_tmp2.y += (float)signy2;
			else
				v_tmp2.x += (float)signx2;

			e2 = e2 + 2 * dt2.y;
		}
		i++;
	}
}

void fill_triangle2(t_app *app, t_triangle t)
{
	t_triangle	t_tmp;

	t_tmp.color = t.color;
	sort_triangle_by_y(&t);

	if (t.v[1]->y == t.v[2]->y)
	{
		set_triangle(&t_tmp, t.v[0], t.v[1], t.v[2]);
		fillFlatSideTriangleInt(app, t_tmp);
	}
	else if (t.v[0]->y == t.v[1]->y)
	{
		set_triangle(&t_tmp, t.v[2], t.v[0], t.v[1]);
		fillFlatSideTriangleInt(app, t_tmp);
	}
	else
	{
		t_vertex	v_tmp;
		float		vx;
		float		vy;
		float		vz;

		vx = (int)(t.v[0]->x + ((float)(t.v[1]->y - t.v[0]->y) / (float)(t.v[2]->y - t.v[0]->y)) * (t.v[2]->x - t.v[0]->x));
		vy = t.v[1]->y;
		vz = 0;
		set_vertex(&v_tmp, vx, vy, vz);

		set_triangle(&t_tmp, t.v[0], t.v[1], &v_tmp);
		fillFlatSideTriangleInt(app, t_tmp);
		set_triangle(&t_tmp, t.v[2], t.v[1], &v_tmp);
		fillFlatSideTriangleInt(app, t_tmp);
	}
}