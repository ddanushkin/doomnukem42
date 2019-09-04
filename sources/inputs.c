#include "doom_nukem.h"

void	update_inputs(t_app *app)
{
	if (app->keyboard.up[KB_ESQ])
		app_close(app);
	if (app->keyboard.down[KB_Q])
		app->rot.x -= 0.2f;
	if (app->keyboard.down[KB_E])
		app->rot.x += 0.2f;
	if (app->keyboard.down[KB_A])
		app->rot.z -= 0.2f;
	if (app->keyboard.down[KB_D])
		app->rot.z += 0.2f;
}

void	reset_inputs_states(t_app *app)
{
	ft_bzero(&app->mouse.keys, sizeof(app->mouse.keys));
	ft_bzero(&app->keyboard, sizeof(app->keyboard));
	app->mouse.move = 0;
	app->mouse.hdir = 0;
	app->mouse.vdir = 0;
}