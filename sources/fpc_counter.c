#include "doom_nukem.h"

void	get_delta_time(t_timer *timer)
{
	double new_time;

	timer->frame++;
	new_time = SDL_GetPerformanceCounter();
	timer->delta  = (new_time - timer->curr) / (double)SDL_GetPerformanceFrequency();
	timer->curr = new_time;
	timer->time += timer->delta;
	if(fmod(timer->time, 0.25) >= 0.24)
		timer->fps = 1.0 / timer->delta;
}