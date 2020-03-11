/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   collision.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lglover <lglover@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/11 13:03:25 by lglover           #+#    #+#             */
/*   Updated: 2020/03/11 13:22:57 by lglover          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"

int		line_triangle(t_v3d p1, t_v3d p2, t_v3d p3, t_pos_temp q)
{
	double	c[5];
	t_v3d	s[7];

	q.pos1.y = q.pos2.y;
	s[0] = v3d_sub(p1, q.pos1);
	s[1] = v3d_sub(p2, q.pos1);
	s[2] = v3d_sub(p3, q.pos1);
	s[3] = v3d_sub(p1, q.pos2);
	s[4] = v3d_sub(p2, q.pos2);
	s[5] = v3d_sub(p3, q.pos2);
	c[0] = signed_tetra_volume(s[0], s[1], s[2]);
	c[1] = signed_tetra_volume(s[3], s[4], s[5]);
	if (c[0] != c[1])
	{
		s[6] = v3d_sub(q.pos2, q.pos1);
		c[2] = signed_tetra_volume(s[6], s[0], s[1]);
		c[3] = signed_tetra_volume(s[6], s[1], s[2]);
		c[4] = signed_tetra_volume(s[6], s[2], s[0]);
		if (c[2] == c[3] && c[3] == c[4])
			return (1);
	}
	return (0);
}

int		height_collision(t_app *app, t_wall *w, t_v3d *pos, t_v3d *f)
{
	double		y;
	int			r;
	t_pos_temp	pos_temp;

	pos_temp.pos1 = v3d_sum(*pos,
			v3d_mul_by(*f, 1.0 / app->timer->delta * 0.05));
	pos_temp.pos2 = *pos;
	y = pos_temp.pos1.y;
	r = line_triangle(w->v[0], w->v[1], w->v[2], pos_temp);
	r = r ? r : line_triangle(w->v[0], w->v[3], w->v[1], pos_temp);
	pos_temp.pos1.y = y - app->height + 0.55;
	r = r ? r : line_triangle(w->v[0], w->v[1], w->v[2], pos_temp);
	r = r ? r : line_triangle(w->v[0], w->v[3], w->v[1], pos_temp);
	pos_temp.pos1.y = y - (app->height * 0.5);
	r = r ? r : line_triangle(w->v[0], w->v[1], w->v[2], pos_temp);
	r = r ? r : line_triangle(w->v[0], w->v[3], w->v[1], pos_temp);
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

	app->hth = 0;
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
	(t_pos_temp){new_pos, *pos}))
		{
			app->hth = 1;
			*f = new_vector(0.0, 0.0, 0.0);
			return ;
		}
		i++;
	}
}

void	check_collision(t_app *app, t_v3d *pos, t_v3d dir)
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
				break ;
			}
			j++;
		}
		i++;
	}
	check_slope_collision(app, pos, &dir);
	*pos = v3d_sum(*pos, dir);
}
