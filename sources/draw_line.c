#include "doom_nukem.h"

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
		set_pixel(app->sdl->surface, l->cur.x, l->cur.y, &l->color, app);
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
		set_pixel(app->sdl->surface, l->cur.x, l->cur.y, &l->color, app);
	}
}

void		draw_line(t_app *app, t_vector *start, t_vector *end, t_color color)
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
	app->depth = start->z;
	if (line.dir.x > line.dir.y)
		dx_dom(app, &line);
	else
		dy_dom(app, &line);
}