#include "doom_nukem.h"

void	update_inputs(t_app *app)
{
	if (app->keyboard.up[KB_ESQ])
		app_close(app);
}

void	reset_inputs_states(t_app *app)
{
	ft_bzero(&app->mouse.keys, sizeof(app->mouse.keys));
	ft_bzero(&app->keyboard, sizeof(app->keyboard));
	app->mouse.move = 0;
	app->mouse.hdir = 0;
	app->mouse.vdir = 0;
}