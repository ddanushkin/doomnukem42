#include "doom_nukem.h"

void 	app_reset_floor_ceil_hit(t_app *app)
{
	app->floor_dist = 10000.0;
	app->ceil_dist = 10000.0;
	app->floor_sector = NULL;
	app->ceil_sector = NULL;
}

void 	app_close(t_app *app)
{
	app->points_count = 0;
	exit(0);
}
