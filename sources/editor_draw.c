#include "doom_nukem.h"

void 	select_edge(t_app *app,  t_v3d v0, t_v3d v1)
{
	t_wall *w;

	w = app->hit_wall;
	app->edge_selected = 1;
	app->edit_wall.v[0] = v1;
	app->edit_wall.v[1] = v0;
	app->edit_wall.v[2] = v1;
	app->edit_wall.v[3] = v0;
	app->edit_wall.sprite_index = w->sprite_index;
	wall_reset_tex(&app->edit_wall);
	wall_update_scale(&app->edit_wall);
	app->inputs->mouse.left = 0;
}

void 	draw_edge(t_app *app, t_v3d edge)
{
	t_v3d v;

	v = matrix_transform(app->camera->transform, edge);
	if (vertex_inside(&v))
	{
		v = matrix_transform(app->camera->screen_space, v);
		vertex_perspective_divide(&v);
		draw_cross(app, (int)v.x, (int)v.y, app->hit_dist + 0.5, 0xff0000);
	}
}

void 	show_edge(t_app *app)
{
	t_v3d	v0;
	t_v3d	v1;

	v0 = app->hit_wall->v[app->hit_first ? 1 : 3];
	v1 = app->hit_wall->v[app->hit_first ? 2 : 0];
	if (v0.y != v1.y)
	{
		draw_edge(app, v0);
		draw_edge(app, v1);
		if (app->inputs->mouse.left)
			select_edge(app, v0, v1);
	}
}

void	draw_new_wall(t_app *app)
{
	t_v3d tmp;

	tmp = vector_sum(
			app->camera->pos,
			vector_mul_by(get_forward(app->camera->rot), 2));
	tmp.x = round(tmp.x / app->grid_size) * app->grid_size;
	tmp.z = round(tmp.z / app->grid_size) * app->grid_size;
	app->edit_wall.v[app->hit_first ? 1 : 3].x = tmp.x;
	app->edit_wall.v[app->hit_first ? 1 : 3].z = tmp.z;
	app->edit_wall.v[app->hit_first ? 2 : 0].x = tmp.x;
	app->edit_wall.v[app->hit_first ? 2 : 0].z = tmp.z;
	wall_update_scale(&app->edit_wall);
	render_wall(app, &app->edit_wall);
	draw_grid(app);
}