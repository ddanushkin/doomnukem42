#include "doom_nukem.h"

double	tr_area(t_v3d *a, t_v3d *b, t_v3d *c)
{
	return ((b->x - a->x) * (c->y - a->y) -
			(c->x - a->x) * (b->y - a->y));
}

void 	fill_triangle(t_app *app, t_v3d v1, t_v3d v2, t_v3d v3)
{
	t_render	r;
	double		ta;

	v1 = matrix_transform(app->camera->screen_space, v1);
	v2 = matrix_transform(app->camera->screen_space, v2);
	v3 = matrix_transform(app->camera->screen_space, v3);
	vertex_perspective_divide(&v1);
	vertex_perspective_divide(&v2);
	vertex_perspective_divide(&v3);

	ta = tr_area(&v1, &v2, &v3);
	if (!app->is_skybox &&
		(app->render_type == wall || app->render_type == floor_ceil) &&
		(app->cs->inside ? ta < 0.0 : ta >= 0.0))
		return;
	if (v3.y < v2.y)
		SWAP(v2, v3, t_v3d);
	if (v2.y < v1.y)
		SWAP(v1, v2, t_v3d);
	if (v3.y < v2.y)
		SWAP(v2, v3, t_v3d);
	r.t = &app->sprites[app->rw->sprite % MAX_SPRITE].pixels[0];
	r.handedness = tr_area(&v1, &v3, &v2) >= 0.0;
	r.depth = &app->depth_buffer[0];
	r.screen = &app->screen[0];
	r.scale_x = app->rw->sx;
	r.scale_y = app->rw->sy;
	r.shade = app->rw->shade;
	scan_triangle(v1, v2, v3, &r);
}

void	render_triangle_0(t_app *app, t_v3d v0, t_v3d v1, t_v3d v2)
{
	if (app->rw->inside == 16843009 || app->rw->inside == 16843008)
		return (fill_triangle(app, v0, v1, v2));
	clip_fill_triangle(app, v0, v1, v2);
}

void	render_triangle_1(t_app *app, t_v3d v0, t_v3d v3, t_v3d v1)
{
	if (app->rw->inside == 16843009 || app->rw->inside == 16842753)
		return (fill_triangle(app, v0, v3, v1));
	clip_fill_triangle(app, v0, v3, v1);
}
