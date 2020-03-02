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

void 	sector_pts_h(t_v3d *pts, int size, double amount)
{
	int		i;

	i = 0;
	while (i < size)
		pts[i++].y += amount;
}

void 	sector_update_height(t_sector *cs, t_v3d *fpts, t_v3d *cpts)
{
	int		i;

	i = 0;
	while (i < cs->walls_count)
	{
		cs->walls[i].v[0].y = fpts[cs->walls[i].v[0].i].y;
		cs->walls[i].v[2].y = fpts[cs->walls[i].v[2].i].y;
		cs->walls[i].v[3].y = cpts[cs->walls[i].v[3].i].y;
		cs->walls[i].v[1].y = cpts[cs->walls[i].v[1].i].y;
		wall_update_scale(&cs->walls[i]);
		wall_update_tex(&cs->walls[i]);
		i++;
	}
	i = 0;
	while (i < cs->trs_count)
	{
		cs->ftrs[i].v[0].y = fpts[cs->ftrs[i].v[0].i].y;
		cs->ftrs[i].v[1].y = fpts[cs->ftrs[i].v[1].i].y;
		cs->ftrs[i].v[2].y = fpts[cs->ftrs[i].v[2].i].y;
		cs->ctrs[i].v[0].y = cpts[cs->ctrs[i].v[0].i].y;
		cs->ctrs[i].v[1].y = cpts[cs->ctrs[i].v[1].i].y;
		cs->ctrs[i].v[2].y = cpts[cs->ctrs[i].v[2].i].y;
		i++;
	}
	i = 0;
	while (i < cs->objs_count)
		cs->objs[i++].pos.y = cs->floor_y;
}

void 	sector_copy_v_1(t_sector *s, t_v3d *p, int len)
{
	int		i;

	i = 0;
	while (i < len)
	{
		s->fpts[i] = p[i];
		s->fpts[i].y = s->floor_y;
		s->fpts[i].i = i;
		s->cpts[i] = p[i];
		s->cpts[i].y = s->ceil_y;
		s->cpts[i].i = i;
		i++;
	}
	s->pts_count = len;
}

void 	sector_copy_v_2(t_sector *s, t_v3d *p, int len)
{
	int		i;
	int 	j;

	i = len - 1;
	j = 0;
	while (i >= 0)
	{
		s->fpts[j] = p[i];
		s->fpts[j].y = s->floor_y;
		s->fpts[j].i = i;
		s->cpts[j] = p[i];
		s->cpts[j].y = s->ceil_y;
		s->cpts[j].i = i;
		i--;
		j++;
	}
	s->pts_count = len;
}

void 	sector_copy_points(t_sector *s, t_v3d *p, int len)
{
	if (get_orientation(p, len) < 0)
		sector_copy_v_1(s, p, len);
	else
		sector_copy_v_2(s, p, len);
}

void 	get_sector_min_max(t_sector *s)
{
	int		i;
	t_v3d	*v;

	i = 0;
	s->x_min = s->fpts[0].x;
	s->x_max = s->x_min;
	s->z_min = s->fpts[0].z;
	s->z_max = s->z_min;
	while (i < s->pts_count)
	{
		v = &s->fpts[i];
		if (v->x < s->x_min)
			s->x_min = v->x;
		if (v->x > s->x_max)
			s->x_max = v->x;
		if (v->z < s->z_min)
			s->z_min = v->z;
		if (v->z > s->z_max)
			s->z_max = v->z;
		i++;
	}
}

void	sector_make_walls(t_sector *s)
{
	int i;
	t_wall *w;

	i = 0;
	while (i < s->pts_count)
	{
		w = &s->walls[i];
		*w = wall_new();
		w->v[0] = s->fpts[i];
		w->v[2] = s->fpts[i + 1 != s->pts_count ? i + 1 : 0];
		w->v[3] = s->cpts[i];
		w->v[1] =  s->cpts[i + 1 != s->pts_count ? i + 1 : 0];
		wall_reset_tex(w);
		wall_update_scale(w);
		i++;
	}
	s->walls_count = s->pts_count;
}

int		sector_close_check(t_v3d *points, int size)
{
	int		i;

	if (size < 3)
		return (1);
	i = 1;
	while (i < size - 1)
	{
		if (line_intersection(
				points[i],
				points[i + 1],
				points[size - 1],
				points[0]))
		{
			return (0);
		}
		i++;
	}
	return (1);
}

void 	copy_triangles(t_sector *s)
{
	int			i;
	t_triangle	tr;

	i = 0;
	while (i < s->trs_count)
	{
		SWAP(s->ftrs[i].v[0], s->ftrs[i].v[2], t_v3d);
		tr = s->ftrs[i];
		SWAP(tr.v[0], tr.v[2], t_v3d);
		tr.v[0].y = s->ceil_y;
		tr.v[1].y = s->ceil_y;
		tr.v[2].y = s->ceil_y;
		s->ctrs[i++] = tr;
	}
}

void 	sector_close(t_app *app, t_sector *s)
{
	if (!sector_close_check(&app->points[0], app->points_count))
	{
		app->bad_close = 0.25;
		return ;
	}
	s->floor = wall_new();
	s->floor.sprite = 278;
	s->ceil = s->floor;
	s->floor.flip = 0;
	s->ceil.sprite = 399;
	s->floor_y = 0.0;
	s->ceil_y = 2.0;
	s->delta_y = fabs(s->ceil_y - s->floor_y);
	s->walls_count = 0;
	s->objs_count = 0;
	s->decor_count = 0;
	s->decor_next = 0;
	s->shade = 0;
	s->inside = 0;
	s->door_anim = 0;
	s->door = 0;
	s->door_h = s->delta_y;
	s->door_dir = -1.0;
	sector_copy_points(s, &app->points[0], app->points_count);
	get_sector_min_max(s);
	s->floor.sx = fabs(s->x_min - s->x_max) * 0.5;
	s->floor.sy = fabs(s->z_min - s->z_max) * 0.5;
	s->ceil.sx = fabs(s->x_min - s->x_max) * 0.5;
	s->ceil.sy = fabs(s->z_min - s->z_max) * 0.5;
	triangulate(&s->ftrs[0], &s->trs_count, points_to_list(s));
	copy_triangles(s);
	sector_make_walls(s);
	sector_update_shade(s);
	app->cs = s;
	app->sectors_count++;
	app->points_count = 0;
}
