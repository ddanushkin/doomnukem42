#include "doom_nukem.h"

uint32_t	wall_inside(t_v3d *v0, t_v3d *v1, t_v3d *v2, t_v3d *v3)
{
	return ((vertex_inside(v0) << 24u) +
			(vertex_inside(v1) << 16u) +
			(vertex_inside(v2) << 8u) +
			vertex_inside(v3));
}

Uint8 	wall_outside_x(t_v3d *v0, t_v3d *v1, t_v3d *v2, t_v3d *v3)
{
	return ((v0->x > v0->w &&
			 v1->x > v1->w &&
			 v2->x > v2->w &&
			 v3->x > v3->w) ||
			(v0->x < -v0->w &&
			 v1->x < -v1->w &&
			 v2->x < -v2->w &&
			 v3->x < -v3->w));
}

Uint8 	wall_outside_y(t_v3d *v0, t_v3d *v1, t_v3d *v2, t_v3d *v3)
{
	return ((v0->y > v0->w &&
			v1->y > v1->w &&
			v2->y > v2->w &&
			v3->y > v3->w) ||
			(v0->y < -v0->w &&
			 v1->y < -v1->w &&
			 v2->y < -v2->w &&
			 v3->y < -v3->w));
}

Uint8 	wall_outside_z(t_v3d *v0, t_v3d *v1, t_v3d *v2, t_v3d *v3)
{
	return ((v0->z > v0->w &&
			 v1->z > v1->w &&
			 v2->z > v2->w &&
			 v3->z > v3->w) ||
			(v0->z < -v0->w &&
			 v1->z < -v1->w &&
			 v2->z < -v2->w &&
			 v3->z < -v3->w));
}

Uint8 wall_outside(t_v3d *v0, t_v3d *v1, t_v3d *v2, t_v3d *v3)
{
	return (wall_outside_x(v0, v1, v2, v3) ||
			wall_outside_y(v0, v1, v2, v3) ||
			wall_outside_z(v0, v1, v2, v3));
}

void 	billboard_face_player(t_app *app, t_wall *w)
{
	t_camera	*c;
	double		half_size;
	t_v3d		r;
	t_v3d		u;
	t_v3d		pos;

	pos = w->pos;
	pos.y += w->size * 0.5;
	c = app->camera;
	half_size = 0.5 * w->size;
	r = vector_mul_by(c->right, half_size);
	u = vector_mul_by(c->up, half_size);
	w->v[0] = vector_sub(vector_sub(pos, r), u);
	w->v[1] = vector_sum(vector_sum(pos, r), u);
	w->v[3] = vector_sum(vector_sub(pos, r), u);
	w->v[2] = vector_sub(vector_sum(pos, r), u);
	wall_reset_tex(w);
}

void 	billboard_switch_sprite(t_app *app, t_wall *w)
{
	int quad;

	quad = app->camera->quad;
	if (quad == 2)
		w->sprite = 499;
	if (quad == 6)
		w->sprite = 503;
	if (quad == 5 || quad == 7)
		w->sprite = 502;
	if (quad == 1 || quad == 3)
		w->sprite = 500;
	if (quad == 4 || quad == 0)
		w->sprite = 501;
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
	if (wall_outside(&v0, &v1, &v2, &v3))
		return;
	w->inside = wall_inside(&v0, &v1, &v2, &v3);
	app->rw = w;
	render_triangle_0(app, v0, v1, v2);
	render_triangle_1(app, v0, v3, v1);
}

void 	render_floor_ceil(t_app *app, t_triangle *tr, t_wall *w)
{
	t_v3d	v0;
	t_v3d	v1;
	t_v3d	v2;

	v0 = matrix_transform(app->camera->transform, tr->v[0]);
	v1 = matrix_transform(app->camera->transform, tr->v[1]);
	v2 = matrix_transform(app->camera->transform, tr->v[2]);
	w->inside = (vertex_inside(&v0) << 24u) +
				(vertex_inside(&v1) << 16u) +
				(vertex_inside(&v2) << 8u) + 1u;
	app->rw = w;
	render_triangle_0(app, v0, v1, v2);
	if (!app->edge_selected && ray_intersect(app, tr->v[0], tr->v[1], tr->v[2]))
		app->hit_first = 1;
	if (!app->camera->fly)
		ray_floor(app, tr->v[0], tr->v[1], tr->v[2]);
}

void 	render_wall(t_app *app, t_wall *w)
{
	t_v3d	v0;
	t_v3d	v1;
	t_v3d	v2;
	t_v3d	v3;

	v0 = matrix_transform(app->camera->transform, w->v[0]);
	v1 = matrix_transform(app->camera->transform, w->v[1]);
	v2 = matrix_transform(app->camera->transform, w->v[2]);
	v3 = matrix_transform(app->camera->transform, w->v[3]);
	if (wall_outside(&v0, &v1, &v2, &v3))
		return;
	w->inside = wall_inside(&v0, &v1, &v2, &v3);
	app->rw = w;
	render_triangle_0(app, v0, v1, v2);
	render_triangle_1(app, v0, v3, v1);
	if (!app->edge_selected)
	{
		if (ray_intersect(app, w->v[0], w->v[1], w->v[2]))
			app->hit_first = 1;
		else if (ray_intersect(app, w->v[0], w->v[3], w->v[1]))
			app->hit_first = 0;
	}
}

void 	render_sector(t_app *app, t_sector *s)
{
	int			j;
	t_triangle	ceil_triangle;

	app->cs = s;
//	j = 0;
//	app->render_type = obj;
//	while (j < s->objs_count)
//		render_billboard(app, &s->objs[j++]);
	j = 0;
	app->render_type = wall;
	while (j < s->walls_count)
		render_wall(app, &s->walls[j++]);
//	j = 0;
//	app->render_type = decor;
//	while (j < s->decor_count)
//		render_wall(app, &s->decor[j++]);
	if (s->ready && s->triangles_count > 0)
	{
		j = 0;
		app->render_type = floor_ceil;
		while (j < s->triangles_count)
		{
			render_floor_ceil(app, &s->triangles[j], &s->floor);
			ceil_triangle = s->triangles[j++];
			ceil_triangle.v[0].y += s->delta_y;
			ceil_triangle.v[1].y += s->delta_y;
			ceil_triangle.v[2].y += s->delta_y;
			render_floor_ceil(app, &ceil_triangle, &s->ceil);
		}
	}
}

void	render_map(t_app *app)
{
	int			i;

	i = 0;
	app->hit_dist = 10000.0;
	app->floor_dist = 10000.0;
	app->hit_wall = NULL;
	app->hit_sector = NULL;
	while (i < app->sectors_count)
		render_sector(app, &app->sectors[i++]);
//	render_skybox(app, app->skybox);
}