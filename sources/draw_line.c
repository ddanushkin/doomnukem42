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
	float	error;
	float	error_tmp;

	delta_vector(start, end, &dt);
	direction_vector(start, end, &dir);
	error = dt.x + dt.y;
	while (1)
	{
		set_pixel(app->sdl->surface, start.x, start.y, c);
		if ((int)start.x == (int)end.x && (int)start.y == (int)end.y)
			break;
		error_tmp = (int)error << 1;
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
//		printf("sx -> %f, sy -> %f\n", start.x, start.y);
//		printf("ex -> %f, ey -> %f\n", end.x, end.y);
//		printf("dx -> %d, dy -> %d\n\n", dt.x, dt.y);
	}
}

static void	dx_dom(t_app *app, t_line *l)
{
	double		stack;
	int			i;

	stack = l->dir.x / 2.0;
	i = 0;
	while (++i <= l->dir.x)
	{
		l->cur.x += l->inc.x;
		stack += l->dir.y;
		if (stack >= l->dir.x)
		{
			stack -= l->dir.x;
			l->cur.y += l->inc.y;
		}
		if ((l->inc.x > 0 && l->cur.x >= SCREEN_W)
			|| (l->inc.x < 0 && l->cur.x < 0) || (l->inc.y < 0 && l->cur.y < 0)
			|| (l->inc.y > 0 && l->cur.y >= SCREEN_H))
			break ;
		set_pixel(app->sdl->surface, l->cur.x, l->cur.y, &l->color);
	}
}

static void	dy_dom(t_app *app, t_line *l)
{
	double		stack;
	int			i;

	stack = l->dir.y / 2.0;
	i = 0;
	while (++i <= l->dir.y)
	{
		l->cur.y += l->inc.y;
		stack += l->dir.x;
		if (stack >= l->dir.y)
		{
			stack -= l->dir.y;
			l->cur.x += l->inc.x;
		}
		if ((l->inc.x > 0 && l->cur.x >= SCREEN_W)
			|| (l->inc.x < 0 && l->cur.x < 0) || (l->inc.y < 0 && l->cur.y < 0)
			|| (l->inc.y > 0 && l->cur.y >= SCREEN_H))
			break ;
		set_pixel(app->sdl->surface, l->cur.x, l->cur.y, &l->color);
	}
}

void		draw_line_2(t_app *app, t_vector *start, t_vector *end, t_color color)
{
	t_line line;

	line.cur.x = start->x;
	line.cur.y = start->y;
	line.dir.x = end->x - start->x;
	line.dir.y = end->y - start->y;
	line.inc.x = (line.dir.x > 0) ? 1 : -1;
	line.inc.y = (line.dir.y > 0) ? 1 : -1;
	line.dir.x = fabsf(line.dir.x);
	line.dir.y = fabsf(line.dir.y);
	line.color = color;
	if (line.dir.x > line.dir.y)
		dx_dom(app, &line);
	else
		dy_dom(app, &line);
}