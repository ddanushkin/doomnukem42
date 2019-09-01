#include "doom_nukem.h"

void	update_inputs(t_app *app)
{
	if (app->keyboard.up[KB_ESQ])
		app_close(app);
}

void	reset_inputs_states(t_app *app)
{
	ft_bzero(&app->keyboard.up, sizeof(app->keyboard.up));
	ft_bzero(&app->keyboard.down, sizeof(app->keyboard.down));
	ft_bzero(&app->mouse.keys.up, sizeof(app->mouse.keys.up));
	ft_bzero(&app->mouse.keys.down, sizeof(app->mouse.keys.down));
	app->mouse.move = 0;
	app->mouse.x_prev = app->mouse.x;
	app->mouse.y_prev = app->mouse.y;
	app->mouse.hdir = 0;
	app->mouse.vdir = 0;
}