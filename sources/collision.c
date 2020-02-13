#include "doom_nukem.h"

int is_colliding(t_v3d c0, double radius, t_v3d v0, t_v3d v1)
{
	double	dist;
	double	u;
	t_v3d	v1v0;
	t_v3d	c0v0;
	t_v3d	tmp;

	v1v0 = vector_sub(v1, v0);
	c0v0 = vector_sub(c0, v0);
	u = (c0v0.x * v1v0.x + c0v0.z * v1v0.z) /
		(v1v0.z * v1v0.z + v1v0.x * v1v0.x);
	if (u >= 0.0 && u <= 1.0)
	{
		dist = (v0.x + v1v0.x * u - c0.x) * (v0.x + v1v0.x * u - c0.x) +
			   (v0.z + v1v0.z * u - c0.z) * (v0.z + v1v0.z * u - c0.z);
	} else
	{
		tmp = u < 0.0 ? vector_sub(v0, c0) : vector_sub(v1, c0);
		dist = (tmp.x * tmp.x) + (tmp.z * tmp.z);
	}
	return (dist < (radius * radius));
}

t_v3d 	get_triangle_normal(t_v3d v0, t_v3d v1, t_v3d v2)
{
	return (vector_normalise(vector_cross_product(vector_sub(v0, v1), vector_sub(v0, v2))));
}

void 	wall_check_collision(t_wall *w, t_v3d *pos, t_v3d *f)
{
	int		collide;
	t_v3d	normal;
	t_v3d	tmp_pos;
	t_v3d	tmp;

	collide = is_colliding(vector_sum(*pos, *f), 0.5, w->v[2], w->v[0]);
	if (collide)
	{
		normal = get_triangle_normal(w->v[1], w->v[2], w->v[0]);
		tmp = new_vector(f->x * normal.x, f->y * normal.y, f->z * normal.z);
		normal = vector_mul_by(normal, -vector_length(tmp));
		tmp_pos = vector_sum(*pos, vector_sub(*f, normal));
		*f = vector_sub(tmp_pos, *pos);
	}
}

void 	check_collision(t_app *app, t_v3d *pos, t_v3d dir)
{
	int	i;
	int j;
	t_wall *w;

	i = 0;
	while (i < app->sectors_count)
	{
		j = 0;
		while (j < app->sectors[i].walls_count)
		{
			w = &app->sectors[i].walls[j++];
			if (w->active)
				wall_check_collision(w, pos, &dir);
		}
		i++;
	}
	*pos = vector_sum(*pos, dir);
}