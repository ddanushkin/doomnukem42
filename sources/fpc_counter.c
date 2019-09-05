#include "doom_nukem.h"

void	get_ticks(t_app *app)
{
	app->timer.current_ticks = clock();
	app->timer.fps = 0;
}

void	get_delta_time(t_app *app)
{
	app->timer.delta_ticks = clock() - app->timer.current_ticks;

}

void	show_fps(t_app *app)
{
	if(app->timer.delta_ticks > 0)
		app->timer.fps = CLOCKS_PER_SEC / app->timer.delta_ticks;

	char *fps_text;
	fps_text = ft_itoa((int)app->timer.fps);
	mlx_string_put(app->mlx, app->window.ptr, 10, 10, 0xFFFFFF, "FPS: ");
	mlx_string_put(app->mlx, app->window.ptr, 55, 10, 0xFFFFFF, fps_text);
	ft_strdel(&fps_text);
}