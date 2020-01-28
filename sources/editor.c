#include "doom_nukem.h"

void	save_new_wall(t_app *app)
{
	t_sector *cs;

	cs = &app->sectors[app->sectors_count - 1];
	app->edit_wall.v[0].w = 1.0;
	app->edit_wall.v[1].w = 1.0;
	app->edit_wall.v[2].w = 1.0;
	app->edit_wall.v[3].w = 1.0;
	cs->walls[cs->walls_count] = app->edit_wall;
	cs->walls_count++;
	app->hit_wall = NULL;
	app->hit_dist = 10000.0;
	app->edge_selected = 0;
}