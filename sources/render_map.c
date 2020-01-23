#include "doom_nukem.h"

void 	billboard_face_player(t_app *app, t_wall *w)
{
	t_camera	*c;
	double		half_size;
	t_v3d		r;
	t_v3d		u;

	c = app->camera;
	half_size = 0.5 * w->size;
	r = vector_mul_by(c->right, half_size);
	u = vector_mul_by(c->up, half_size);
	w->v[0] = vector_sub(vector_sub(w->pos, r), u);
	w->v[1] = vector_sum(vector_sum(w->pos, r), u);
	w->v[3] = vector_sum(vector_sub(w->pos, r), u);
	w->v[2] = vector_sub(vector_sum(w->pos, r), u);
	wall_reset_tex(w);
}

void 	billboard_switch_sprite(t_app *app, t_wall *w)
{
	int quad;

	quad = app->camera->quad;
	if (quad == 2)
		w->sprite_index = 499;
	if (quad == 6)
		w->sprite_index = 503;
	if (quad == 5 || quad == 7)
		w->sprite_index = 502;
	if (quad == 1 || quad == 3)
		w->sprite_index = 500;
	if (quad == 4 || quad == 0)
		w->sprite_index = 501;
	if (quad == 1 || quad == 0 || quad == 7)
	{
		SWAP(w->v[3].tex_x, w->v[1].tex_x, double);
		SWAP(w->v[3].tex_y, w->v[1].tex_y, double);
		SWAP(w->v[0].tex_x, w->v[2].tex_x, double);
		SWAP(w->v[0].tex_y, w->v[2].tex_y, double);
	}
}

void 	render_billboard(t_app *app, t_wall *w)
{
	t_v3d v0;
	t_v3d v1;
	t_v3d v2;
	t_v3d v3;

	billboard_face_player(app, w);
	billboard_switch_sprite(app, w);
	v0 = matrix_transform(app->camera->transform, w->v[0]);
	v1 = matrix_transform(app->camera->transform, w->v[1]);
	v2 = matrix_transform(app->camera->transform, w->v[2]);
	v3 = matrix_transform(app->camera->transform, w->v[3]);
	app->hit = 0;
	app->render_wall = w;
	render_triangle(app, v0, v1, v2);
	render_triangle(app, v0, v3, v1);
}

void 	render_floor_ceil(t_app *app, t_triangle *tr, t_wall *w)
{
	t_v3d	v0;
	t_v3d	v1;
	t_v3d	v2;

	v0 = matrix_transform(app->camera->transform, tr->v[0]);
	v1 = matrix_transform(app->camera->transform, tr->v[1]);
	v2 = matrix_transform(app->camera->transform, tr->v[2]);
	app->hit = 0;
	app->render_wall = w;
	if (render_triangle(app, v0, v1, v2) && !app->edge_selected)
	{
		ray_intersect(app, tr->v[0], tr->v[1], tr->v[2]);
		if (app->hit)
			app->hit_first = 1;
	}
}

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

void 	render_wall(t_app *app, t_wall *w)
{
	t_v3d	v0;
	t_v3d	v1;
	t_v3d	v2;
	t_v3d	v3;

	t_v3d	pos = app->camera->pos;
	t_v3d	prev = app->camera->pos_prev;

	if (!app->collide_x)
		app->collide_x = is_colliding(new_vector(pos.x, 0.0, prev.z), 0.20, w->v[2], w->v[0]);
	if (!app->collide_z)
		app->collide_z = is_colliding(new_vector(prev.x, 0.0, pos.z), 0.20, w->v[2], w->v[0]);
	v0 = matrix_transform(app->camera->transform, w->v[0]);
	v1 = matrix_transform(app->camera->transform, w->v[1]);
	v2 = matrix_transform(app->camera->transform, w->v[2]);
	v3 = matrix_transform(app->camera->transform, w->v[3]);

	app->render_wall = w;
	app->hit = 0;
	if (render_triangle(app, v0, v1, v2) && !app->edge_selected)
	{
		ray_intersect(app, w->v[0], w->v[1], w->v[2]);
		if (app->hit)
			app->hit_first = 1;
	}
	if (render_triangle(app, v0, v3, v1) && !app->edge_selected && !app->hit)
	{
		ray_intersect(app, w->v[0], w->v[3], w->v[1]);
		if (app->hit)
			app->hit_first = 0;
	}
}

void 	render_sector(t_app *app, t_sector *s)
{
	int			j;
	t_triangle	ceil_triangle;

	j = 0;
	while (j < s->objs_count)
		render_billboard(app, &s->objs[j++]);
	j = 0;
	while (j < s->walls_count)
		render_wall(app, &s->walls[j++]);
	if (s->ready && s->triangles_count > 0)
	{
		j = 0;
		while (j < s->triangles_count)
		{
			render_floor_ceil(app, &s->triangles[j], &s->floor);
			ceil_triangle = s->triangles[j];
			ceil_triangle.v[0].y += 2.0;
			ceil_triangle.v[1].y += 2.0;
			ceil_triangle.v[2].y += 2.0;
			render_floor_ceil(app, &ceil_triangle, &s->ceil);
			j++;
		}
	}
}

void	render_map(t_app *app)
{
	int			i;

	i = 0;
	app->hit_dist = 10000.0;
	app->hit = 0;
	app->hit_wall = NULL;
	while (i < app->sectors_count)
		render_sector(app, &app->sectors[i++]);
//	render_skybox(app, app->skybox);
}