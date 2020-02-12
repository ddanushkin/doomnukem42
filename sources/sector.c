#include "doom_nukem.h"

double	get_shade(t_light *l, t_v3d	*v0, t_v3d *v1, double value)
{
	double	x;
	double	y;
	double	z;
	double	shade;

	x = l->pos.x - (v0->x + value * (v1->x - v0->x));
	y = l->pos.y - (v0->y + value * (v1->y - v0->y));
	z = l->pos.z - (v0->z + value * (v1->z - v0->z));
	shade = sqrt(x * x + y * y + z * z);
	shade = CLAMP(1.0 - (shade / l->power), 0.0, 1.0);
	return (shade);
}

void 	fill_shade_wall(t_light *light, t_v3d v0, t_v3d v1, double *sh)
{
	double	step_y;
	int 	i;
	int 	j;

	i = 0;
	step_y = fabs(v0.y - v1.y) / 10.0;
	v1.y = v0.y;
	while (i < 10)
	{
		j = 0;
		while (j < 10)
		{
			sh[i * 10 + j] = get_shade(light, &v0, &v1, (double)j / 10.0);
			j++;
		}
		v0.y += step_y;
		v1.y = v0.y;
		i++;
	}
}

void 	fill_shade_floor(t_light *light, t_v3d v0, t_v3d v1, double *sh)
{
	double	step_y;
	int 	i;
	int 	j;

	i = 0;
	step_y = fabs(v0.z - v1.z) / 10.0;
	v1.z = v0.z;
	while (i < 10)
	{
		j = 0;
		while (j < 10)
		{
			sh[i * 10 + j] = get_shade(light, &v0, &v1, (double)j / 10.0);
			j++;
		}
		v0.z += step_y;
		v1.z = v0.z;
		i++;
	}
}

void 	sector_floor_shade(t_sprite *sprites, t_sector *s)
{
	int			i;
	t_triangle	tr;
	t_v3d		v0;
	t_v3d		v1;

	i = 0;
	while (i < s->triangles_count)
	{
		tr = s->triangles[i];
		v0.x = s->x_min;
		v0.y = tr.v[0].y;
		v0.z = s->z_min;
		v1.x = s->x_max;
		v1.y = v0.y;
		v1.z = s->z_max;
		fill_shade_floor(&s->l, v0, v1, &s->floor.sh[0]);
		reshade_sprite(&sprites[s->floor.sprite].pixels[0], &s->floor);
		v0.y = v0.y + s->delta_y;
		v1.y = v0.y;
		fill_shade_floor(&s->l, v0, v1, &s->ceil.sh[0]);
		reshade_sprite(&sprites[s->ceil.sprite].pixels[0], &s->ceil);
		i++;
	}
}

void	sector_update_shade(t_sprite *sprites, t_sector *cs)
{
	int			i;
	t_wall		*w;

	i = 0;
	while (i < cs->walls_count)
	{
		w = &cs->walls[i];
		fill_shade_wall(&cs->l, w->v[0], w->v[1], &w->sh[0]);
		reshade_sprite(&sprites[w->sprite].pixels[0], w);
		i++;
	}
	i = 0;
	while (i < cs->decor_count)
	{
		w = &cs->decor[i];
		fill_shade_wall(&cs->l, w->v[0], w->v[1], &w->sh[0]);
		reshade_sprite(&sprites[w->sprite].pixels[0], w);
		i++;
	}
	sector_floor_shade(sprites, cs);
}

void 	sector_update_height(t_sector *cs)
{
	int		i;

	i = 0;
	while (i < cs->walls_count)
	{
		cs->walls[i].v[0].y = cs->floor_y;
		cs->walls[i].v[2].y = cs->floor_y;
		cs->walls[i].v[3].y = cs->ceil_y;
		cs->walls[i].v[1].y = cs->ceil_y;
		wall_update_scale(&cs->walls[i++]);
	}
	i = 0;
	while (i < cs->objs_count)
		cs->objs[i++].pos.y = cs->floor_y;
	cs->l.pos.y = cs->ceil_y - 0.1;
	i = 0;
	while (i < cs->triangles_count)
	{
		cs->triangles[i].v[0].y = cs->floor_y;
		cs->triangles[i].v[1].y = cs->floor_y;
		cs->triangles[i++].v[2].y = cs->floor_y;
	}
	cs->delta_y = cs->ceil_y - cs->floor_y;
}

void 	sector_copy_points(t_sector *s, t_v3d *p, int len)
{
	int		i;

	i = 0;
	while (i < len)
	{
		s->points[i] = p[i];
		i++;
	}
	s->points_count = len;
}

void 	get_sector_min_max(t_sector *cs)
{
	int		i;
	t_v3d	*v;

	i = 0;
	cs->x_min = cs->points[0].x;
	cs->x_max = cs->x_min;
	cs->z_min = cs->points[0].z;
	cs->z_max = cs->z_min;
	while (i < cs->points_count)
	{
		v = &cs->points[i];
		if (v->x < cs->x_min)
			cs->x_min = v->x;
		if (v->x > cs->x_max)
			cs->x_max = v->x;
		if (v->z < cs->z_min)
			cs->z_min = v->z;
		if (v->z > cs->z_max)
			cs->z_max = v->z;
		i++;
	}
}

void 	sector_update_light(t_sprite *sprites, t_sector *s, t_v3d pos)
{
	s->l.pos = pos;
	s->l.pos.y = s->ceil_y - 0.1;
	sector_update_shade(sprites, s);
	s->l.power = CLAMP(s->l.power, 0.0, 1000.0);
}

void	sector_make_walls(t_sector *s)
{
	int i;
	t_wall *w;

	i = 0;
	while (i < s->points_count)
	{
		w = &s->walls[i];
		*w = wall_new();
		w->v[0] = s->points[i];
		w->v[2] = s->points[i + 1 != s->points_count ? i + 1 : 0];
		w->v[3] = w->v[0];
		w->v[1] = w->v[2];
		w->v[3].y = s->delta_y;
		w->v[1].y = s->delta_y;
		wall_reset_tex(w);
		wall_update_scale(w);
		i++;
	}
	s->walls_count = s->points_count;
}

void 	sector_close(t_app *app, t_sector *s)
{
	if (s->ready)
		return (sector_update_light(&app->sprites[0], s, app->camera->pos));
	sector_copy_points(s, &app->points[0], app->points_count);
	get_sector_min_max(s);
	triangulate(&s->triangles[0], &s->triangles_count, points_to_list(s, app->points, app->points_count));
	s->floor = wall_new();
	s->floor.sx = fabs(s->x_min - s->x_max) * 0.5;
	s->floor.sy = fabs(s->z_min - s->z_max) * 0.5;
	s->floor.sprite = 278;
	s->ceil = s->floor;
	s->ceil.sprite = 399;
	s->floor_y = 0.0;
	s->ceil_y = 2.0;
	s->delta_y = s->ceil_y - s->floor_y;
	s->ready = 1;
	s->walls_count = 0;
	s->objs_count = 0;
	s->l.power = 5.0;
	s->objs_count = 0;
	s->decor_count = 0;
	s->decor_next = 0;
	sector_make_walls(s);
	sector_update_light(&app->sprites[0], s, app->camera->pos);
	app->cs = s;
	app->sectors_count++;
	app->points_count = 0;
	printf("pc -> %d\n", s->points_count);
	printf("tc -> %d\n", s->triangles_count);
}
