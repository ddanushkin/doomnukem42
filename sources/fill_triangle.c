#include "doom_nukem.h"

void	fill_bottom_flat(t_app *app, t_triangle *triangle)
{
	float	invslope1;
	float	invslope2;
	float	curx1;
	float	curx2;
	int		scanlineY;

	invslope1 = (triangle->v[1].x - triangle->v[0].x) / (triangle->v[1].y - triangle->v[0].y);
	invslope2 = (triangle->v[2].x - triangle->v[0].x) / (triangle->v[2].y - triangle->v[0].y);
	curx1 = triangle->v[0].x;
	curx2 = triangle->v[0].x;
	scanlineY = (int)triangle->v[0].y;
	while (scanlineY <= (int)triangle->v[1].y)
	{
		t_vector start;
		t_vector end;

		set_vector(&start, curx1, (float)scanlineY, 0);
		set_vector(&end, curx2, (float)scanlineY, 0);
		draw_line(app, start, end, &triangle->color);
		curx1 += invslope1;
		curx2 += invslope2;
		scanlineY++;
	}
}

void	fill_top_flat(t_app *app, t_triangle *triangle)
{
	float	invslope1;
	float	invslope2;
	float	curx1;
	float	curx2;
	int		scanlineY;

	invslope1 = (triangle->v[2].x - triangle->v[0].x) / (triangle->v[2].y - triangle->v[0].y);
	invslope2 = (triangle->v[2].x - triangle->v[1].x) / (triangle->v[2].y - triangle->v[1].y);
	curx1 = triangle->v[2].x;
	curx2 = triangle->v[2].x;
	scanlineY = (int)triangle->v[2].y;
	while (scanlineY > (int)triangle->v[0].y)
	{
		t_vector start;
		t_vector end;

		set_vector(&start, curx1, (float)scanlineY, 0);
		set_vector(&end, curx2, (float)scanlineY, 0);
		draw_line(app, start, end, &triangle->color);
		curx1 -= invslope1;
		curx2 -= invslope2;
		scanlineY--;
	}
}

void	sort_triangle_by_y(t_triangle *in, t_triangle *out)
{
	float	tmp_y;
	
	set_vector(&out->v[0], in->v[0].x, in->v[0].y, in->v[0].z);
	set_vector(&out->v[1], in->v[1].x, in->v[1].y, in->v[1].z);
	set_vector(&out->v[2], in->v[2].x, in->v[2].y, in->v[2].z);
	if (out->v[0].y > out->v[1].y)
	{
		tmp_y = out->v[0].y;
		out->v[0].y = out->v[1].y;
		out->v[1].y = tmp_y;
	}
	if (out->v[0].y > out->v[2].y)
	{
		tmp_y = out->v[0].y;
		out->v[0].y = out->v[2].y;
		out->v[2].y = tmp_y;
	}
	if (out->v[1].y > out->v[2].y)
	{
		tmp_y = out->v[1].y;
		out->v[1].y = out->v[2].y;
		out->v[2].y = tmp_y;
	}
}

void	fill_triangle(t_app *app, t_triangle *triangle)
{
	t_triangle sorted;

	sort_triangle_by_y(triangle, &sorted);
	if (sorted.v[1].y == sorted.v[2].y)
		fill_bottom_flat(app, &sorted);
	else if (sorted.v[0].y == sorted.v[1].y)
		fill_top_flat(app, &sorted);
	else
	{
		t_vector tmp_vector;
		t_triangle tmp_triangle;

		tmp_vector.x = (int)(sorted.v[0].x + ((float)(sorted.v[1].y - sorted.v[0].y) / (float)(sorted.v[2].y - sorted.v[0].y)) * (sorted.v[2].x - sorted.v[0].x));
		tmp_vector.y = sorted.v[1].y;

		tmp_triangle.v[0] = triangle->v[0];
		tmp_triangle.v[1] = triangle->v[1];
		tmp_triangle.v[2] = tmp_vector;

		fill_bottom_flat(app, &tmp_triangle);

		tmp_triangle.v[0] = triangle->v[1];
		tmp_triangle.v[1] = tmp_vector;
		tmp_triangle.v[2] = triangle->v[2];

		fill_top_flat(app, &tmp_triangle);
	}
}

