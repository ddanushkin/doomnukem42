#include "doom_nukem.h"

void 		coll_t1_t2(t_coll_data	*c, t_v3d* v)
{
	double	min;
	double	max;
	int 	div;

	div = 0;
	c->y = 0.0;
	max = MAX(v[3].y, v[1].y);
	min = MIN(v[3].y, v[1].y);
	c->t1 = (-c->b - sqrt(c->d)) / (2.0 * c->a);
	c->t2 = (-c->b + sqrt(c->d)) / (2.0 * c->a);
	if (c->t1 < 1 && c->t1 > 0)
	{
		c->y += max * (1.0 - c->t1) + c->t1 * min;
		c->coll = 1;
	}
	if (c->y != 0.0)
		div = 1;
	if (c->t2 < 1 && c->t2 > 0)
	{
		c->y += max * (1.0 - c->t2) + c->t2 * min;
		c->coll = 1;
		if (div)
			c->y *= 0.5;
	}
}

t_coll_data	is_colliding(t_v3d c0, double radius, t_v3d* v)
{
	t_v3d		dp;
	t_coll_data	c;

	c.coll = 0;
	c0.y = MIN(v[0].y, v[2].y) + fabs(v[0].y - v[2].y) * 0.5;
	dp = v3d_sub(v[2], v[0]);
	c.a = v3d_dot(dp, dp);
	c.b = 2.0 *
		(v3d_dot(v[0], dp) - dp.x * c0.x - dp.y * c0.y - dp.z * c0.z);
	c.c = v[0].x * v[0].x -
		2 * v[0].x * c0.x + c0.x * c0.x + v[0].y * v[0].y -
		2 * v[0].y * c0.y + c0.y * c0.y + v[0].z * v[0].z -
		2 * v[0].z * c0.z + c0.z * c0.z - radius * radius;
	c.d = c.b * c.b - 4 * c.a * c.c;
	if (c.d < 0)
		return (c);
	coll_t1_t2(&c, v);
	return (c);
}

t_v3d 	get_triangle_normal(t_v3d v0, t_v3d v1, t_v3d v2)
{
	return (vector_normalise(v3d_cross(v3d_sub(v0, v1), v3d_sub(v0, v2))));
}

int		wall_check_collision(t_wall *w, t_v3d *pos, t_v3d *f)
{
	t_coll_data	c;
	t_v3d		normal;

	c = is_colliding(*pos, 0.15, &w->v[0]);
	if (c.coll && (pos->y - 0.3) >= c.y)
		return (0);
	if (c.coll)
	{
		normal = get_triangle_normal(w->v[1], w->v[2], w->v[0]);
		normal = v3d_mul_by(normal, v3d_dot(*f, normal));
		*f = v3d_sub(*f, normal);
		*pos = v3d_sub(*pos, normal);
	}
	return (c.coll);
}

void 	stop_collision(t_v3d *pos, t_v3d prev)
{
	*pos = prev;
}

void 	check_collision(t_app *app, t_v3d *pos, t_v3d dir)
{
	int		i;
	int		j;
	t_wall	*w;
	int		count;
	t_v3d	prev;

	i = 0;
	count = 0;
	prev = *pos;
	*pos = v3d_sum(*pos, dir);
	while (i < app->sectors_count)
	{
		j = 0;
		while (j < app->sectors[i].walls_count)
		{
			w = &app->sectors[i].walls[j++];
			if ((w->active && (pos->y + 0.25) > MIN(w->v[0].y, w->v[2].y)))
				count += wall_check_collision(w, pos, &dir);
			if (count >= 2)
				return (stop_collision(pos, prev));
		}
		i++;
	}
}