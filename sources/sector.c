#include "doom_nukem.h"

void 	sector_close(t_app *app)
{
	t_sector *cs;

	cs = &app->sectors[app->sectors_count - 1];
	if (app->inputs->keyboard[SDL_SCANCODE_Q] && cs->walls_count >= 3 && !cs->ready)
	{
		get_sector_min_max(cs);
		get_floor_poly(cs);
		/*TODO: Set sector floor height*/
		cs->floor_height = 0.0;
		cs->floor = wall_new();
		cs->floor.scale_x = fabs(cs->x_min - cs->x_max) * 0.5;
		cs->floor.scale_y = fabs(cs->z_min - cs->z_max) * 0.5;
		cs->floor.sprite = 278;
		cs->ceil = cs->floor;
		cs->ceil.sprite = 399;
		cs->ready = 1;
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