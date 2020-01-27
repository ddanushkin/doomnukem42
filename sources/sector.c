#include "doom_nukem.h"

double	get_shade(t_light *l, t_v3d	*v0, t_v3d *v2, double value)
{
	double	x;
	double	y;
	double	z;
	double	shade;

	x = l->pos.x - (v0->x + value * (v2->x - v0->x));
	y = l->pos.y - (v0->y + value * (v2->y - v0->y));
	z = l->pos.z - (v0->z + value * (v2->z - v0->z));
	shade = sqrt(x * x + y * y + z * z);
	shade = CLAMP(1.0 - (shade / l->power), 0.0, 1.0);
	return (shade);
}

void 	fill_shade_y(t_light *light, t_v3d v0, t_v3d v1, double *shade)
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
			shade[i * 10 + j] = get_shade(light, &v0, &v1,
										  (double)j / 10.0);
			j++;
		}
		v0.y += step_y;
		v1.y = v0.y;
		i++;
	}
}

void 	fill_shade_z(t_light *light, t_v3d v0, t_v3d v1, double *shade)
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
			shade[i * 10 + j] = get_shade(light, &v0, &v1,
										  (double)j / 10.0);
			j++;
		}
		v0.z += step_y;
		v1.z = v0.z;
		i++;
	}
}

void 	sector_floor_shade(t_sector *cs)
{
	int			i;
	t_triangle	tr;
	t_v3d		v0;
	t_v3d		v1;

	i = 0;
	while (i < cs->triangles_count)
	{
		tr = cs->triangles[i];
		v0.x = cs->x_min;
		v0.y = tr.v[0].y;
		v0.z = cs->z_min;
		v1.x = cs->x_max;
		v1.y = v0.y;
		v1.z = cs->z_max;
		fill_shade_z(&cs->l, v0, v1, &cs->floor.shade[0]);
		v0.y = v0.y + 2.0;
		v1.y = v0.y + 2.0;
		fill_shade_z(&cs->l, v0, v1, &cs->ceil.shade[0]);
		i++;
	}
}

void	sector_update_shade(t_sector *cs)
{
	int			i;
	t_wall		*w;

	i = 0;
	while (i < cs->objs_count)
	{
		w = &cs->objs[i];
		fill_shade_y(&cs->l, w->v[0], w->v[1], &w->shade[0]);
		i++;
	}
	i = 0;
	while (i < cs->walls_count)
	{
		w = &cs->walls[i];
		fill_shade_y(&cs->l, w->v[0], w->v[1], &w->shade[0]);
		i++;
	}
	i = 0;
	while (i < cs->decor_count)
	{
		w = &cs->decor[i];
		fill_shade_y(&cs->l, w->v[0], w->v[1], &w->shade[0]);
		i++;
	}
	sector_floor_shade(cs);
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

void 	sector_close(t_app *app)
{
	t_sector *cs;

	cs = &app->sectors[app->sectors_count - 1];
	if (app->inputs->keyboard[SDL_SCANCODE_Q] && cs->walls_count >= 3 && !cs->ready)
	{
		get_sector_min_max(cs);
		get_floor_poly(cs);
		/*TODO: Set sector floor height*/
		cs->floor = wall_new();
		cs->floor.scale_x = fabs(cs->x_min - cs->x_max) * 0.5;
		cs->floor.scale_y = fabs(cs->z_min - cs->z_max) * 0.5;
		cs->floor.sprite = 278;
		cs->ceil = cs->floor;
		cs->ceil.sprite = 399;
		cs->ready = 1;
	}
	if (app->inputs->keyboard[SDL_SCANCODE_Q] && cs->ready)
	{
		cs->l.pos = app->camera->pos;
		cs->l.pos.y = cs->ceil_y - 0.1;
		cs->l.power = 5;
		sector_update_shade(cs);
	}

}

void 	compare_size(t_sector *cs, t_wall *w, int index)
{
	if (w->v[index].x < cs->x_min)
		cs->x_min = w->v[index].x;
	if (w->v[index].x > cs->x_max)
		cs->x_max = w->v[index].x;
	if (w->v[index].z < cs->z_min)
		cs->z_min = w->v[index].z;
	if (w->v[index].z > cs->z_max)
		cs->z_max = w->v[index].z;
}

void 	get_sector_min_max(t_sector *cs)
{
	int		i;

	i = 0;
	cs->x_min = cs->walls[0].v[0].x;
	cs->x_max = cs->x_min;
	cs->z_min = cs->walls[0].v[0].z;
	cs->z_max = cs->z_min;
	while (i < cs->walls_count)
	{
		compare_size(cs, &cs->walls[i], 0);
		compare_size(cs, &cs->walls[i], 2);
		i++;
	}
}