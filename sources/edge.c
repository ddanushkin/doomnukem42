#include "doom_nukem.h"

t_edge	edge_new(t_gradient	g, t_v3d min, t_v3d max, int index)
{
	t_edge		e;
	double		y_dist;
	double		x_dist;
	double		y_step;
	double		x_step;

	e.y_start = (int)ceil(min.y);
	e.y_end = (int)ceil(max.y);
	y_dist = max.y - min.y;
	x_dist = max.x - min.x;
	y_step = e.y_start - min.y;
	e.x_step = x_dist / y_dist;
	e.x = min.x + y_step * e.x_step;
	x_step = e.x - min.x;
	e.tex_x = g.x[index] + g.x_x_step * x_step + g.x_y_step * y_step;
	e.tex_x_step = g.x_y_step + g.x_x_step * e.x_step;
	e.tex_y = g.y[index] + g.y_x_step * x_step + g.y_y_step * y_step;
	e.tex_y_step = g.y_y_step + g.y_x_step * e.x_step;
	e.tex_z = g.z[index] + g.z_x_step * x_step + g.z_y_step * y_step;
	e.tex_z_step = g.z_y_step + g.z_x_step * e.x_step;
	e.depth = g.depth[index] + g.d_x_step * x_step + g.d_y_step * y_step;
	e.depth_step = g.d_y_step + g.d_x_step * e.x_step;
	return (e);
}

void	edge_step(t_edge *edge)
{
	edge->x += edge->x_step;
	edge->tex_x += edge->tex_x_step;
	edge->tex_y += edge->tex_y_step;
	edge->tex_z += edge->tex_z_step;
	edge->depth += edge->depth_step;
}