#include "doom_nukem.h"

void	save_new_wall(t_app *app)
{
	t_sector *cs;

	cs = &app->sectors[app->sectors_count - 1];
	cs->walls[cs->walls_count] = app->edit_wall;
	cs->walls_count++;
	app->hit_wall = NULL;
	app->hit = 0;
	app->hit_dist = 10000.0;
	app->edge_selected = 0;
	app->inputs->mouse.left = 0;
}