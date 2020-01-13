#include "doom_nukem.h"

int		inside_view(t_app *app, t_v3d v1, t_v3d v2, t_v3d v3)
{
	int		counter;
	double 	w1;
	double 	w2;
	double 	w3;

	w1 = fabs(v1.w);
	w2 = fabs(v2.w);
	w3 = fabs(v3.w);
	counter = 0;
	counter += fabs(v1.x) <= w1;
	counter += fabs(v1.y) <= w1;
	counter += v1.z <= app->camera->z_far;
	counter += v1.z >= app->camera->z_near;
	counter += fabs(v2.x) <= w2;
	counter += fabs(v2.y) <= w2;
	counter += v2.z <= app->camera->z_far;
	counter += v2.z >= app->camera->z_near;
	counter += fabs(v3.x) <= w3;
	counter += fabs(v3.y) <= w3;
	counter += v3.z <= app->camera->z_far;
	counter += v3.z >= app->camera->z_near;
	return (counter);
}

double	triangle_area(t_v3d *a, t_v3d *b, t_v3d *c)
{
	return ((b->x - a->x) * (c->y - a->y) - (c->x - a->x) * (b->y - a->y));
}

void 	fill_triangle(t_app *app, t_v3d v1, t_v3d v2, t_v3d v3)
{
	v1 = matrix_transform(app->camera->screen_space, v1);
	v2 = matrix_transform(app->camera->screen_space, v2);
	v3 = matrix_transform(app->camera->screen_space, v3);
	vertex_perspective_divide(&v1);
	vertex_perspective_divide(&v2);
	vertex_perspective_divide(&v3);
//	if (triangle_area(&v1, &v3, &v2) >= 0.0)
//		return;
	if (v3.y < v2.y)
		SWAP(v2, v3, t_v3d);
	if (v2.y < v1.y)
		SWAP(v1, v2, t_v3d);
	if (v3.y < v2.y)
		SWAP(v2, v3, t_v3d);
	scan_triangle(app, v1, v2, v3, triangle_area(&v1, &v3, &v2) >= 0.0);
	app->triangles_counter++;
}

int		render_triangle(t_app *app, t_v3d v1, t_v3d v2, t_v3d v3)
{
	int		inside_counter;

	inside_counter = inside_view(app, v1, v2, v3);
	if (!inside_counter)
		return 0;
	if (inside_counter < 12)
		clip_fill_triangle(app, v1, v2, v3);
	else
		fill_triangle(app, v1, v2, v3);
	return 1;
}