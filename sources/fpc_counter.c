#include "doom_nukem.h"

void	get_ticks(t_timer *timer)
{
	timer->current_ticks = clock();
	timer->fps = 0;
}

void	get_delta_time(t_timer *timer)
{
	timer->delta_ticks = clock() - timer->current_ticks;
	timer->delta = timer->delta_ticks;
}

void	show_fps(t_app *app)
{
	if(app->sdl->timer->delta_ticks > 0)
		app->sdl->timer->fps = CLOCKS_PER_SEC / app->sdl->timer->delta_ticks;
	printf("FPS: %d\n", (int)app->sdl->timer->fps);
}