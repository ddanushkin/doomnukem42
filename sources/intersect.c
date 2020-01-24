#include "doom_nukem.h"

void 		hit_fill_data(t_app *app, double distance)
{
	app->hit_point = vector_sum(
			app->camera->pos,
			vector_mul_by(app->camera->dir, distance));
	app->hit_dist = distance;
	app->hit_wall = app->rw;
	app->hit = 1;
}

void		ray_intersect(t_app *app, t_v3d v0, t_v3d v1, t_v3d v2)
{
	t_intersect	i;

	i.v0v1 = vector_sub(v1, v0);
	i.v0v2 = vector_sub(v2, v0);
	i.pvec = vector_cross_product(app->camera->dir, i.v0v2);
	i.det = vector_dot_product(i.v0v1, i.pvec);
	if (fabs(i.det) < 0.0)
		return ;
	i.det = 1 / i.det;
	i.tvec = vector_sub(app->camera->pos, v0);
	i.u = vector_dot_product(i.tvec, i.pvec) * i.det;
	if (i.u < 0 || i.u > 1)
		return ;
	i.qvec = vector_cross_product(i.tvec, i.v0v1);
	i.v = vector_dot_product(app->camera->dir, i.qvec) * i.det;
	if (i.v < 0 || i.u + i.v > 1)
		return ;
	i.t = vector_dot_product(i.v0v2, i.qvec) * i.det;
	if (i.t < app->hit_dist && i.t > 0.0)
		hit_fill_data(app, i.t);
}