#include "doom_nukem.h"

t_v3d 	get_triangle_normal(t_v3d v0, t_v3d v1, t_v3d v2)
{
	return (v3d_normalise(v3d_cross(v3d_sub(v0, v1), v3d_sub(v0, v2))));
}

double signed_tetra_volume(t_v3d ba, t_v3d ca, t_v3d da)
{
	return (SIGNF(v3d_dot(v3d_cross(ba, ca), da)*0.166666));
}

int line_triangle(t_v3d p1, t_v3d p2, t_v3d p3, t_v3d q1, t_v3d q2)
{
	double	c[5];
	t_v3d	s[7];

	q2.y = q1.y;
	s[0] = v3d_sub(p1, q1);
	s[1] = v3d_sub(p2, q1);
	s[2] = v3d_sub(p3, q1);
	s[3] = v3d_sub(p1, q2);
	s[4] = v3d_sub(p2, q2);
	s[5] = v3d_sub(p3, q2);
	c[0] = signed_tetra_volume(s[0], s[1], s[2]);
	c[1] = signed_tetra_volume(s[3], s[4], s[5]);
	if (c[0] != c[1])
	{
		s[6] = v3d_sub(q2, q1);
		c[2] = signed_tetra_volume(s[6], s[0], s[1]);
		c[3] = signed_tetra_volume(s[6], s[1], s[2]);
		c[4] = signed_tetra_volume(s[6], s[2], s[0]);
		if (c[2] == c[3] && c[3] == c[4])
			return (1);
	}
	return (0);
}

int 	height_collision(t_app *app, t_wall *w, t_v3d *pos, t_v3d *f)
{
	double 	y;
	t_v3d	new_pos;
	int		r;

	new_pos = v3d_sum(*pos, v3d_mul_by(*f, 1.0/app->timer->delta*0.05));
	y = new_pos.y;
	r = line_triangle(w->v[0], w->v[1], w->v[2], new_pos, *pos);
	r = r ? r : line_triangle(w->v[0], w->v[3], w->v[1], new_pos, *pos);
	new_pos.y = y - app->height + 0.55;
	r = r ? r : line_triangle(w->v[0], w->v[1], w->v[2], new_pos, *pos);
	r = r ? r : line_triangle(w->v[0], w->v[3], w->v[1], new_pos, *pos);
	new_pos.y = y - (app->height * 0.5);
	r = r ? r : line_triangle(w->v[0], w->v[1], w->v[2], new_pos, *pos);
	r = r ? r : line_triangle(w->v[0], w->v[3], w->v[1], new_pos, *pos);
	return (r);
}

int		wall_check_collision(t_app *app, t_wall *w, t_v3d *pos, t_v3d *f)
{
	int		res;
	t_v3d	n;

	res = height_collision(app, w, pos, f);
	if (res)
	{
		n = get_triangle_normal(w->v[0], w->v[1], w->v[2]);
		if (isnan(n.x) || isnan(n.y) || isnan(n.z))
			return (0);
		*f = v3d_sub(*f, v3d_mul_by(n, v3d_dot(n, *f)));
	}
	return (res);
}

void	check_slope_collision(t_app *app, t_v3d *pos, t_v3d *f)
{
	int			i;
	t_triangle	*tr_arr;
	t_v3d		new_pos;

	app->head_too_high = 0;
	if (!app->ceil_sector || fabs(pos->y - app->ceil_point.y) > PLAYER_HEIGHT)
		return ;
	new_pos = v3d_sum(*pos, v3d_mul_by(*f, 1.0 / app->timer->delta * 0.05));
	if (app->ceil_point.y > app->ceil_sector->floor_y)
		tr_arr = &app->ceil_sector->ctrs[0];
	else
		tr_arr = &app->ceil_sector->ftrs[0];
	i = 0;
	while (i < app->ceil_sector->trs_count)
	{
		if (line_triangle(tr_arr[i].v[0], tr_arr[i].v[1], tr_arr[i].v[2],
				new_pos, *pos))
		{
			app->head_too_high = 1;
			*f = new_vector(0.0, 0.0, 0.0);
			return ;
		}
		i++;
	}
}

void 	check_collision(t_app *app, t_v3d *pos, t_v3d dir)
{
	int		i;
	int		j;
	t_wall	*w;

	i = 0;
	while (i < app->sectors_count)
	{
		j = 0;
		while (j < app->sectors[i].walls_count)
		{
			w = &app->sectors[i].walls[j];
			if (w->active && wall_check_collision(app, w, pos, &dir))
			{
				i = -1;
				break;
			}
			j++;
		}
		i++;
	}
	check_slope_collision(app, pos, &dir);
	*pos = v3d_sum(*pos, dir);
}
