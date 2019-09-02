#include "doom_nukem.h"

void 	app_close(t_app *app)
{
	if (PRINT_DEBUG)
		printf("app -> close");
	mlx_destroy_window(app->mlx, app->window.ptr);
	mlx_destroy_image(app->mlx, app->screen.ptr);
	ft_strdel(&app->window.title);
	exit(0);
}