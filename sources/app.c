#include "doom_nukem.h"

void 	app_close(t_app *app)
{
	if (PRINT_DEBUG)
		printf("app -> close");

	exit(0);
}