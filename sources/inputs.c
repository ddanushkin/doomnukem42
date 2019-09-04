#include "doom_nukem.h"

void	mlx_events(t_app *app)
{
	mlx_hook(app->window.ptr, 17, 5, window_event_close, app);
	mlx_hook(app->window.ptr, 12, 5, window_event_expose, app);
	mlx_hook(app->window.ptr, 2, 5, keyboard_event_down, app);
	mlx_hook(app->window.ptr, 3, 5, keyboard_event_up, app);
	mlx_hook(app->window.ptr, 4, 5, mouse_event_down, app);
	mlx_hook(app->window.ptr, 5, 5, mouse_event_up, app);
	mlx_hook(app->window.ptr, 6, 5, mouse_event_move, app);

}

void	update_inputs(t_app *app)
{
	if (app->keyboard.down[KB_ESQ])
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

void	do_input(t_app *app)
{
	mlx_events(app);
	update_inputs(app);
}

void	reset_inputs_states(t_app *app)
{
	ft_bzero(&app->mouse.keys, sizeof(app->mouse.keys));
	ft_bzero(&app->keyboard, sizeof(app->keyboard));
	app->mouse.move = 0;
	app->mouse.hdir = 0;
	app->mouse.vdir = 0;
}