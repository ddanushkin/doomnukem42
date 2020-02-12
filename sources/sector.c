#include "doom_nukem.h"

void	sector_update_shade(t_sector *s)
{
	int			i;

	i = 0;
	while (i < s->walls_count)
		s->walls[i++].shade = s->shade;
	i = 0;
	while (i < s->decor_count)
		s->decor[i++].shade = s->shade;
	s->floor.shade = s->shade;
	s->ceil.shade = s->shade;
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
	s->objs_count = 0;
	s->decor_count = 0;
	s->decor_next = 0;
	s->shade = 1.0;
	sector_make_walls(s);
	sector_update_shade(s);
	app->cs = s;
	app->sectors_count++;
	app->points_count = 0;
}
