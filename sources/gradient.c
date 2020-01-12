#include "doom_nukem.h"

double 		gradient_calc_x_step(double coords[3], t_triangle tr, double one_over_dx)
{
	return (((coords[1] - coords[2]) * (tr.v[0].y - tr.v[2].y) -
			(coords[0] - coords[2]) * (tr.v[1].y - tr.v[2].y)) *
			one_over_dx);
}

double 		gradient_calc_y_step(double coords[3], t_triangle tr, double one_over_dy)
{
	return (((coords[1] - coords[2]) * (tr.v[0].x - tr.v[2].x) -
			(coords[0] - coords[2]) * (tr.v[1].x - tr.v[2].x)) *
			one_over_dy);
}

void 		gradient_set_steps(t_gradient *g, t_v3d min, t_v3d mid, t_v3d max)
{
	t_triangle tr;

	tr.v[0] = min;
	tr.v[1] = mid;
	tr.v[2] = max;
	g->x_x_step = gradient_calc_x_step(g->x, tr, g->one_over_dx);
	g->x_y_step = gradient_calc_y_step(g->x, tr, g->one_over_dy);
	g->y_x_step = gradient_calc_x_step(g->y, tr, g->one_over_dx);
	g->y_y_step = gradient_calc_y_step(g->y, tr, g->one_over_dy);
	g->z_x_step = gradient_calc_x_step(g->z, tr, g->one_over_dx);
	g->z_y_step = gradient_calc_y_step(g->z, tr, g->one_over_dy);
	g->d_x_step = gradient_calc_x_step(g->depth, tr, g->one_over_dx);
	g->d_y_step = gradient_calc_y_step(g->depth, tr, g->one_over_dy);
}

t_gradient	gradient_new(t_v3d min, t_v3d mid, t_v3d max)
{
	t_gradient	g;

	g.z[0] = 1.0 / min.w;
	g.z[1] = 1.0 / mid.w;
	g.z[2] = 1.0 / max.w;
	g.x[0] = min.tex_x * g.z[0];
	g.x[1] = mid.tex_x * g.z[1];
	g.x[2] = max.tex_x * g.z[2];
	g.y[0] = min.tex_y * g.z[0];
	g.y[1] = mid.tex_y * g.z[1];
	g.y[2] = max.tex_y * g.z[2];
	g.depth[0] = min.z;
	g.depth[1] = mid.z;
	g.depth[2] = max.z;
	g.one_over_dx  = 1.0 / (
					(mid.x - max.x) * (min.y - max.y) -
					(min.x - max.x) * (mid.y - max.y));
	g.one_over_dy = -g.one_over_dx;
	gradient_set_steps(&g, min, mid, max);
	return (g);
}