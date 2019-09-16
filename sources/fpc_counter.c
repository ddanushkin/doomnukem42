#include "doom_nukem.h"

void	get_ticks(t_timer *timer)
{
	timer->current_ticks = clock();
}

void	get_delta_time(t_timer *timer)
{
	timer->delta_ticks = clock() - timer->current_ticks;
}

void	show_fps(t_app *app)
{
	app->timer.fps = 0;
	if(app->timer.delta_ticks > 0)
		app->timer.fps = CLOCKS_PER_SEC / app->timer.delta_ticks;
	printf("FPS: %d\n", (int)app->timer.fps);
}