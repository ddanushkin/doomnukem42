#include "doom_nukem.h"

void	get_ticks(t_timer *timer)
{
	timer->curr = SDL_GetPerformanceCounter();
}

void	get_delta_time(t_timer *timer)
{
	timer->delta = (SDL_GetPerformanceCounter() - timer->curr)
				   * 1000.0 / (double)SDL_GetPerformanceFrequency();
	timer->delta *= 0.001;
	timer->time += timer->delta;
	if(timer->delta > 0.0)
		timer->fps = 1.0 / timer->delta;
}

void	show_fps(t_app *app)
{
	printf("FPS: %d\n", (int)app->timer->fps);
}