#include "doom_nukem.h"

void	skybox_front_back(t_app *app, t_skybox *s, t_wall *w)
{
	w->v[0] = s->v[0];
	w->v[1] = s->v[1];
	w->v[2] = s->v[2];
	w->v[3] = s->v[3];
	wall_reset_tex(w);
	w->sprite = 498;
	if (!wall_outside(&w->v[0], &w->v[1], &w->v[2], &w->v[3]))
	{
		w->inside = wall_inside(&w->v[0], &w->v[1], &w->v[2], &w->v[3]);
		render_triangle_0(app, w->v[0], w->v[1], w->v[2]);
		render_triangle_1(app, w->v[0], w->v[3], w->v[1]);
	}
	w->v[0] = s->v[4];
	w->v[1] = s->v[5];
	w->v[2] = s->v[6];
	w->v[3] = s->v[7];
	wall_reset_tex(w);
	w->sprite = 495;
	if (wall_outside(&w->v[0], &w->v[1], &w->v[2], &w->v[3]))
		return;
	w->inside = wall_inside(&w->v[0], &w->v[1], &w->v[2], &w->v[3]);
	render_triangle_0(app, w->v[0], w->v[1], w->v[2]);
	render_triangle_1(app, w->v[0], w->v[3], w->v[1]);
}

void	skybox_left_right(t_app *app, t_skybox *s, t_wall *w)
{
	w->v[0] = s->v[4];
	w->v[1] = s->v[3];
	w->v[2] = s->v[0];
	w->v[3] = s->v[7];
	wall_reset_tex(w);
	w->sprite = 493;
	if (!wall_outside(&w->v[0], &w->v[1], &w->v[2], &w->v[3]))
	{
		w->inside = wall_inside(&w->v[0], &w->v[1], &w->v[2], &w->v[3]);
		render_triangle_0(app, w->v[0], w->v[1], w->v[2]);
		render_triangle_1(app, w->v[0], w->v[3], w->v[1]);
	}
	w->v[0] = s->v[2];
	w->v[1] = s->v[5];
	w->v[2] = s->v[6];
	w->v[3] = s->v[1];
	wall_reset_tex(w);
	w->sprite = 496;
	if (wall_outside(&w->v[0], &w->v[1], &w->v[2], &w->v[3]))
		return;
	w->inside = wall_inside(&w->v[0], &w->v[1], &w->v[2], &w->v[3]);
	render_triangle_0(app, w->v[0], w->v[1], w->v[2]);
	render_triangle_1(app, w->v[0], w->v[3], w->v[1]);
}

void	skybox_top_bottom(t_app *app, t_skybox *s, t_wall *w)
{
	w->v[0] = s->v[3];
	w->v[1] = s->v[5];
	w->v[2] = s->v[1];
	w->v[3] = s->v[7];
	wall_reset_tex(w);
	w->sprite = 497;
	if (!wall_outside(&w->v[0], &w->v[1], &w->v[2], &w->v[3]))
	{
		w->inside = wall_inside(&w->v[0], &w->v[1], &w->v[2], &w->v[3]);
		render_triangle_0(app, w->v[0], w->v[1], w->v[2]);
		render_triangle_1(app, w->v[0], w->v[3], w->v[1]);
	}
	w->v[0] = s->v[4];
	w->v[1] = s->v[2];
	w->v[2] = s->v[6];
	w->v[3] = s->v[0];
	wall_reset_tex(w);
	w->sprite = 494;
	if (wall_outside(&w->v[0], &w->v[1], &w->v[2], &w->v[3]))
		return;
	w->inside = wall_inside(&w->v[0], &w->v[1], &w->v[2], &w->v[3]);
	render_triangle_0(app, w->v[0], w->v[1], w->v[2]);
	render_triangle_1(app, w->v[0], w->v[3], w->v[1]);
}

void 	render_skybox(t_app *app, t_skybox s)
{
	t_wall		w;
	t_mat4x4	tr;

	app->is_skybox = 1;
	tr = app->camera->view;
	tr.m[3] = 0.0;
	tr.m[7] = 0.0;
	tr.m[11] = 0.0;
	tr = get_transform_matrix(matrix_multiply(app->camera->projection, tr));
	w = wall_new();
	w.sprite = 100;
	w.shade = 0;
	app->rw = &w;
	s.v[0] = matrix_transform(tr, s.v[0]);
	s.v[1] = matrix_transform(tr, s.v[1]);
	s.v[2] = matrix_transform(tr, s.v[2]);
	s.v[3] = matrix_transform(tr, s.v[3]);
	s.v[4] = matrix_transform(tr, s.v[4]);
	s.v[5] = matrix_transform(tr, s.v[5]);
	s.v[6] = matrix_transform(tr, s.v[6]);
	s.v[7] = matrix_transform(tr, s.v[7]);
	skybox_front_back(app, &s, &w);
	skybox_left_right(app, &s, &w);
	skybox_top_bottom(app, &s, &w);
	app->is_skybox = 0;
}