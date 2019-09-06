#include "doom_nukem.h"

void	reset_inputs_states(t_app *app)
{
	ft_bzero(&app->mouse.keys, sizeof(app->mouse.keys));
	ft_bzero(&app->keyboard, sizeof(app->keyboard));
	app->mouse.move = 0;
	app->mouse.hdir = 0;
	app->mouse.vdir = 0;
}