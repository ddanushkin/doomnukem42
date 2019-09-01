#include "doom_nukem.h"

int 	update(t_app *app)
{
	update_inputs(app);

	t_color color;
	color.r = 128;
	color.g = 75;
	color.b = 200;
	set_pixel(&app->screen, 205, 205, color);

	mlx_put_image_to_window(app->mlx, app->window.ptr, app->screen.ptr, 0, 0);
	reset_inputs_states(app);
	return (0);
}

int		main()
{
	t_app	app;

	init_app(&app);
	mlx_hook(app.window.ptr, 17, 5, window_event_close, &app);
	mlx_hook(app.window.ptr, 12, 5, window_event_expose, &app);
	mlx_hook(app.window.ptr, 2, 5, keyboard_event_down, &app);
	mlx_hook(app.window.ptr, 3, 5, keyboard_event_up, &app);
	mlx_hook(app.window.ptr, 4, 5, mouse_event_down, &app);
	mlx_hook(app.window.ptr, 5, 5, mouse_event_up, &app);
	mlx_hook(app.window.ptr, 6, 5, mouse_event_move, &app);
	mlx_loop_hook(app.mlx, update, &app);
	mlx_loop(app.mlx);
}