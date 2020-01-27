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
	app->hit = 0;
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
	app->hit = 0;
	app->rw = w;
	if (render_triangle_0(app, v0, v1, v2) && !app->edge_selected)
	{
		ray_intersect(app, tr->v[0], tr->v[1], tr->v[2]);
		if (app->hit)
			app->hit_first = 1;
	}
}

void 	render_wall(t_app *app, t_wall *w)
{
	t_v3d	v0;
	t_v3d	v1;
	t_v3d	v2;
	t_v3d	v3;

	t_v3d	pos = app->camera->pos;
	t_v3d	prev = app->camera->pos_prev;

	if (!app->collide_x && app->cs->ready)
		app->collide_x = is_colliding(new_vector(pos.x, 0.0, prev.z), 0.20, w->v[2], w->v[0]);
	if (!app->collide_z && app->cs->ready)
		app->collide_z = is_colliding(new_vector(prev.x, 0.0, pos.z), 0.20, w->v[2], w->v[0]);
	v0 = matrix_transform(app->camera->transform, w->v[0]);
	v1 = matrix_transform(app->camera->transform, w->v[1]);
	v2 = matrix_transform(app->camera->transform, w->v[2]);
	v3 = matrix_transform(app->camera->transform, w->v[3]);
	if (wall_outside(&v0, &v1, &v2, &v3))
		return;
	w->inside = wall_inside(&v0, &v1, &v2, &v3);
	app->rw = w;
	app->hit = 0;
	if (render_triangle_0(app, v0, v1, v2) && !app->edge_selected)
	{
		ray_intersect(app, w->v[0], w->v[1], w->v[2]);
		if (app->hit)
			app->hit_first = 1;
	}
	if (render_triangle_1(app, v0, v3, v1) && !app->edge_selected && !app->hit)
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
		app->is_floor = 1;
		j = 0;
		while (j < s->triangles_count)
		{
			render_floor_ceil(app, &s->triangles[j], &s->floor);
			ceil_triangle = s->triangles[j];
			ceil_triangle.v[0].y += s->delta_y;
			ceil_triangle.v[1].y += s->delta_y;
			ceil_triangle.v[2].y += s->delta_y;
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
	{
		app->is_floor = 0;
		app->cs = &app->sectors[i];
		render_sector(app, &app->sectors[i++]);
		if (app->hit_wall && app->cs->ready)
		{
			if (app->inputs->keyboard[SDL_SCANCODE_L])
			{
				if (app->input_minus)
				{
					app->input_minus = 0;
					app->cs->l.power -= 0.15;
				}
				else if (app->input_plus)
				{
					app->input_plus = 0;
					app->cs->l.power += 0.15;
				}
				sector_update_shade(app->cs);
				app->cs->l.power = CLAMP(app->cs->l.power, 0.0, 1000.0);
			}
			if (app->inputs->keyboard[SDL_SCANCODE_R])
			{
				if (app->input_minus)
				{
					app->input_minus = 0;
					app->cs->floor_y -= 0.5;
//					app->camera->pos.y -= 0.5;
				}
				else if (app->input_plus)
				{
					app->input_plus = 0;
					app->cs->floor_y += 0.5;
//					app->camera->pos.y += 0.5;
				}
				sector_update_height(app->cs);
				sector_update_shade(app->cs);
			}
			if (app->inputs->keyboard[SDL_SCANCODE_C])
			{
				if (app->input_minus)
				{
					app->input_minus = 0;
					app->cs->ceil_y -= 0.5;
				}
				else if (app->input_plus)
				{
					app->input_plus = 0;
					app->cs->ceil_y += 0.5;
				}
				sector_update_height(app->cs);
				sector_update_shade(app->cs);
			}
		}
	}
//	render_skybox(app, app->skybox);
}