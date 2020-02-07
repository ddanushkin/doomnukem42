#include "doom_nukem.h"

void 	app_close(t_app *app)
{
	app->points_count = 0;
	if (PRINT_DEBUG)
		printf("app -> close");

	exit(0);
}