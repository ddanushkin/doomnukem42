#include "doom_nukem.h"

void	delta_vector(t_vector start, t_vector end, t_vector_i *out)
{
	out->x = abs((int)end.x - (int)start.x);
	out->y = -abs((int)end.y - (int)start.y);
}

void	direction_vector(t_vector start, t_vector end, t_vector_i *out)
{
	out->x = start.x < end.x ? 1 : -1;
	out->y = start.y < end.y ? 1 : -1;
}

void	draw_line(t_app *app, t_vector start, t_vector end, t_color *c)
{
	t_vector_i	dt;
	t_vector_i	dir;
	int			error;
	int			error_tmp;

	delta_vector(start, end, &dt);
	direction_vector(start, end, &dir);
	error = dt.x + dt.y;
	while (1)
	{
		set_pixel(&app->screen, start.x, start.y, c);
		if ((int)start.x == (int)end.x && (int)start.y == (int)end.y)
			break;
		error_tmp = error << 1;
		if (error_tmp >= dt.y)
		{
			error += dt.y;
			start.x += dir.x;
		}
		if (error_tmp <= dt.x)
		{
			error += dt.x;
			start.y += dir.y;
		}
	}
}

void	line_(t_app *app, t_vector p0, t_vector p1, t_color *c)
{
	int		steep;
	int		x;
	int 	y;
	float	tmp;

	steep = 0;
	if (ABS(p0.x-p1.x) < ABS(p0.y-p1.y))
	{
		SWAP(p0.x, p0.y, float);
		SWAP(p1.x, p1.y, float);
		steep = 1;
	}
	if (p0.x > p1.x)
		SWAP(p0, p1, t_vector);

	x = p0.x;
	while (x <= p1.x)
	{
		tmp = (x - p0.x) / (float)(p1.x - p0.x);
		y = p0.y * (1.0f - tmp) + p1.y * tmp;
		if (steep)
			set_pixel(&app->screen, y, x, c);
		else
			set_pixel(&app->screen, x, y, c);
		x++;
	}
}