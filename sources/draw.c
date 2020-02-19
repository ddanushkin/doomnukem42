#include "doom_nukem.h"

double norm2(t_v3d v)
{
	return (v.x * v.x + v.y * v.y + v.z * v.z);
}

int		line_intersection(t_v3d v0, t_v3d v1, t_v3d v2, t_v3d v3)
{
	t_v3d	da;
	t_v3d	db;
	double	s;
	double	t;
	t_v3d	tmp;

	da = v3d_sub(v1, v0);
	db = v3d_sub(v3, v2);
	if (v3d_dot(v3d_sub(v2, v0), v3d_cross(da, db)) != 0.0)
		return 0;
	s = v3d_dot(v3d_cross(v3d_sub(v2, v0), db),
			v3d_cross(da, db)) / norm2(v3d_cross(da, db));
	t = v3d_dot(v3d_cross(v3d_sub(v2, v0), da),
			v3d_cross(da, db)) / norm2(v3d_cross(da, db));
	if ((s >= 0.0 && s <= 1.0) && (t >= 0.0 && t <= 1.0))
	{
		tmp = v3d_mul_by(v3d_sum(v0, da), s);
		if ((tmp.x == v0.x && tmp.z == v0.z) ||
			(tmp.x == v1.x && tmp.z == v1.z) ||
			(tmp.x == v3.x && tmp.z == v3.z))
			return 0;
		return 1;
	}
	return 0;
}

void	clip_point_by_x(t_v3d *v)
{
	double	prev;
	double	factor;

	prev = v->x;
	v->x = SIGNF(v->x) * v->w;
	factor = v->x / prev;
	v->z *= factor;
	v->y *= factor;
}

void	clip_point_by_y(t_v3d *v)
{
	double	prev;
	double	factor;

	prev = v->y;
	v->y = SIGNF(v->y) * v->w;
	factor = v->y / prev;
	v->z *= factor;
	v->x *= factor;
}

void	clip_point_by_z(t_v3d *v)
{
	double	prev;
	double	factor;

	prev = v->z;
	v->z = SIGNF(v->z) * v->w;
	factor = v->z / prev;
	v->x *= factor;
	v->y *= factor;
}

void 	draw_line_3d(t_app *app, t_v3d start, t_v3d end, uint32_t c)
{
	t_v3d	tmp1;
	t_v3d	tmp2;

	tmp1 = start;
	tmp2 = end;
	tmp1.y = 0.0;
	tmp2.y = 0.0;
	tmp1 = matrix_transform(app->camera->transform, tmp1);
	tmp2 = matrix_transform(app->camera->transform, tmp2);
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
			draw_line_3d(app, p[i], p[i + 1], 0x5c5c5c);
			i++;
		}
		draw_line_3d(app, p[size - 1], p[0], 0x5c5c5c);
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
		draw_points_sector(app, &s->fpts[0], s->pts_count);
		i++;
	}
}

void 	draw_points(t_app *app, t_v3d *p, int size)
{
	int		i;

	if (size == 1)
		point_draw(app, p[0], 0xff0000);
	else
	{
		i = 0;
		while (i < size - 1)
		{
			draw_line_3d(app, p[i], p[i + 1], 0xff00ff);
			i++;
		}
		if (app->bad_close >= 0.0)
		{
			draw_line_3d(app, p[size - 1], p[0], app->bclr[app->bflag]);
			app->bflag = !app->bflag;
			app->bad_close -= app->timer->delta;
		}
		else
		{
			draw_line_3d(app, p[size - 1], p[0], 0xffff00);
		}
	}
}