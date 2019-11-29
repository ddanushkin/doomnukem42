#include "doom_nukem.h"

void capFrameRate(t_app *app, long *then, double *remainder)
{
	long wait, frameTime;

	wait = 16.0 + *remainder;
	*remainder -= (int)*remainder;
	frameTime = SDL_GetTicks() - *then;
	wait -= frameTime;
	if (wait < 1)
		wait = 1;
	SDL_Delay(wait);
	*remainder += 0.667;
	*then = SDL_GetTicks();
	app->timer->delta = 0.01;
}

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