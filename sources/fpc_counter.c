#include "doom_nukem.h"

void	get_ticks(t_timer *timer)
{
	timer->current_ticks = clock();
	timer->fps = 0;
}

void	get_delta_time(t_timer *timer)
{
	timer->delta_ticks = clock() - timer->current_ticks;
}

void	show_fps_sdl(t_timer *timer)
{
	if(timer->delta_ticks > 0)
		timer->fps = CLOCKS_PER_SEC / timer->delta_ticks;

	printf("FPS: %d\n", (int)timer->fps);
}

void	show_fps(t_app *app)
{
	if(app->timer.delta_ticks > 0)
		app->timer.fps = CLOCKS_PER_SEC / app->timer.delta_ticks;

	printf("FPS: %d\n", (int)app->timer.fps);
//	char *fps_text;
//	fps_text = ft_itoa((int)app->timer.fps);
//	mlx_string_put(app->mlx, app->window.ptr, 10, 10, 0xFFFFFF, "FPS: ");
//	mlx_string_put(app->mlx, app->window.ptr, 55, 10, 0xFFFFFF, fps_text);
//	ft_strdel(&fps_text);
}