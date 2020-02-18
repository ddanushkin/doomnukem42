#include "doom_nukem.h"

int	hit_fill_data(t_app *app, double distance)
{
	app->hit_point = v3d_sum(
			app->camera->pos,
			v3d_mul_by(app->camera->dir, distance));
	app->hit_dist = distance;
	app->hit_wall = app->rw;
	app->hit_sector = app->cs;
	app->hit_type = app->render_type;
	return (1);
}

int	hit_fill_floor(t_app *app, t_v3d dir, double distance)
{
	app->floor_point = v3d_sum(
			app->camera->pos,
			v3d_mul_by(dir, distance));
	app->floor_dist = distance;
	app->floor_sector = app->cs;
	return (1);
}

int	hit_fill_ceil(t_app *app, t_v3d dir, double distance)
{
	app->ceil_point = v3d_sum(
			app->camera->pos,
			v3d_mul_by(dir, distance));
	app->ceil_dist = distance;
	app->ceil_sector = app->cs;
	return (1);
}

int		ray_intersect(t_app *app, t_v3d v0, t_v3d v1, t_v3d v2)
{
	t_intersect	i;

	i.v0v1 = v3d_sub(v1, v0);
	i.v0v2 = v3d_sub(v2, v0);
	i.pvec = v3d_cross(app->camera->dir, i.v0v2);
	i.det = v3d_dot(i.v0v1, i.pvec);
	if (fabs(i.det) < 0.0)
		return 0;
	i.det = 1 / i.det;
	i.tvec = v3d_sub(app->camera->pos, v0);
	i.u = v3d_dot(i.tvec, i.pvec) * i.det;
	if (i.u < 0 || i.u > 1)
		return 0;
	i.qvec = v3d_cross(i.tvec, i.v0v1);
	i.v = v3d_dot(app->camera->dir, i.qvec) * i.det;
	if (i.v < 0 || i.u + i.v > 1)
		return 0;
	i.t = v3d_dot(i.v0v2, i.qvec) * i.det;
	if (i.t < app->hit_dist && i.t > 0.0)
		return (hit_fill_data(app, i.t));
	return 0;
}

int		ray_ceil(t_app *app, t_v3d v0, t_v3d v1, t_v3d v2)
{
	t_intersect	i;
	t_v3d		dir;

	dir = new_vector(0.0, 1.0, 0.0);
	i.v0v1 = v3d_sub(v1, v0);
	i.v0v2 = v3d_sub(v2, v0);
	i.pvec = v3d_cross(dir, i.v0v2);
	i.det = v3d_dot(i.v0v1, i.pvec);
	if (fabs(i.det) < 0.0)
		return 0;
	i.det = 1 / i.det;
	i.tvec = v3d_sub(app->camera->pos, v0);
	i.u = v3d_dot(i.tvec, i.pvec) * i.det;
	if (i.u < 0 || i.u > 1)
		return 0;
	i.qvec = v3d_cross(i.tvec, i.v0v1);
	i.v = v3d_dot(dir, i.qvec) * i.det;
	if (i.v < 0 || i.u + i.v > 1)
		return 0;
	i.t = v3d_dot(i.v0v2, i.qvec) * i.det;
	if (i.t < app->ceil_dist && i.t > 0.0)
		return (hit_fill_ceil(app, dir, i.t));
	return 0;
}

int		ray_floor(t_app *app, t_v3d v0, t_v3d v1, t_v3d v2)
{
	t_intersect	i;
	t_v3d		dir;

	dir = new_vector(0.0, -1.0, 0.0);
	i.v0v1 = v3d_sub(v1, v0);
	i.v0v2 = v3d_sub(v2, v0);
	i.pvec = v3d_cross(dir, i.v0v2);
	i.det = v3d_dot(i.v0v1, i.pvec);
	if (fabs(i.det) < 0.0)
		return 0;
	i.det = 1 / i.det;
	i.tvec = v3d_sub(app->camera->pos, v0);
	i.u = v3d_dot(i.tvec, i.pvec) * i.det;
	if (i.u < 0 || i.u > 1)
		return 0;
	i.qvec = v3d_cross(i.tvec, i.v0v1);
	i.v = v3d_dot(dir, i.qvec) * i.det;
	if (i.v < 0 || i.u + i.v > 1)
		return 0;
	i.t = v3d_dot(i.v0v2, i.qvec) * i.det;
	if (i.t < app->floor_dist && i.t > 0.0)
		return (hit_fill_floor(app, dir, i.t));
	return 0;
}