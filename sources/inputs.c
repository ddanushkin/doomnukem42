#include "doom_nukem.h"

void	update_inputs(t_app *app)
{
	if (app->keyboard.up[KB_ESQ])
		app_close(app);
}

void	reset_inputs_states(t_app *app)
{
	int	x;
	int y;

	x = app->mouse.x;
	y = app->mouse.y;
	ft_bzero(&app->mouse, sizeof(app->mouse));
	app->mouse.x_prev = x;
	app->mouse.y_prev = y;
	ft_bzero(&app->keyboard, sizeof(app->keyboard));
}