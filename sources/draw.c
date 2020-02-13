#include "doom_nukem.h"

void 	draw_line_3d(t_app *app, t_v3d start, t_v3d end, uint32_t c)
{
	t_v3d		tmp1;
	t_v3d		tmp2;

	tmp1 = matrix_transform(app->camera->transform, start);
	tmp2 = matrix_transform(app->camera->transform, end);
	if (vertex_inside(&tmp1) &&
		vertex_inside(&tmp2))
	{
		tmp1 = matrix_transform(app->camera->screen_space, tmp1);
		vertex_perspective_divide(&tmp1);
		tmp2 = matrix_transform(app->camera->screen_space, tmp2);
		vertex_perspective_divide(&tmp2);
		draw_line(app, &tmp1, &tmp2, c);
	}
}

void 	draw_points_sector(t_app *app, t_v3d *p, int size)
{
	int		i;

	if (size == 1)
		point_draw(app, p[0], 0xff00ff);
	else
	{
		i = 0;
		while (i < size - 1)
		{
			draw_line_3d(app, p[i], p[i + 1], 0xffffff);
			i++;
		}
		draw_line_3d(app, p[size - 1], p[0], 0xffffff);
	}
}

void	draw_sectors(t_app *app)
{
	int			i;
	t_sector	*s;

	i = 0;
	while (i < app->sectors_count)
	{
		s = &app->sectors[i];
		draw_points_sector(app, &s->points[0], s->points_count);
		i++;
	}
}

void 	draw_points(t_app *app, t_v3d *p, int size)
{
	int		i;

	if (size == 1)
		point_draw(app, p[0], 0xff00ff);
	else
	{
		i = 0;
		while (i < size - 1)
		{
			draw_line_3d(app, p[i], p[i + 1], 0xff00ff);
			i++;
		}
		draw_line_3d(app, p[size - 1], p[0], 0xffff00);
	}
}