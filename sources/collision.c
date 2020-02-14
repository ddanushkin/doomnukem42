#include "doom_nukem.h"

int is_colliding(t_v3d c0, double radius, t_v3d v0, t_v3d v1)
{
	double	dist;
	double	u;
	t_v3d	v1v0;
	t_v3d	c0v0;
	t_v3d	tmp;

	v1v0 = v3d_sub(v1, v0);
	c0v0 = v3d_sub(c0, v0);
	u = (c0v0.x * v1v0.x + c0v0.z * v1v0.z) /
		(v1v0.z * v1v0.z + v1v0.x * v1v0.x);
	if (u >= 0.0 && u <= 1.0)
	{
		dist = (v0.x + v1v0.x * u - c0.x) * (v0.x + v1v0.x * u - c0.x) +
			   (v0.z + v1v0.z * u - c0.z) * (v0.z + v1v0.z * u - c0.z);
	} else
	{
		tmp = u < 0.0 ? v3d_sub(v0, c0) : v3d_sub(v1, c0);
		dist = (tmp.x * tmp.x) + (tmp.z * tmp.z);
	}
	return (dist < (radius * radius));
}

t_v3d 	get_triangle_normal(t_v3d v0, t_v3d v1, t_v3d v2)
{
	return (vector_normalise(v3d_cross(v3d_sub(v0, v1), v3d_sub(v0, v2))));
}

int		wall_check_collision(t_wall *w, t_v3d *pos, t_v3d *f)
{
	int		collide;
	t_v3d	normal;

	collide = is_colliding(v3d_sum(*pos, *f), 0.15, w->v[2], w->v[0]);
	if (collide)
	{
		normal = get_triangle_normal(w->v[1], w->v[2], w->v[0]);
		normal = v3d_mul_by(normal, v3d_dot(*f, normal));
		*f = v3d_sub(*f, normal);
		*pos = v3d_sub(*pos, normal);
	}
	return (collide);
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
			if (w->active)
				count += wall_check_collision(w, pos, &dir);
			if (count >= 2)
				return (stop_collision(pos, prev));
		}
		i++;
	}
}