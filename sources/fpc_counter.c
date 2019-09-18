#include "doom_nukem.h"

void	get_ticks(t_timer *timer)
{
	timer->current_ticks = clock();
}

void	get_delta_time(t_timer *timer)
{
	if(!PRINT_DEBUG)
	{
		timer->delta_ticks = clock() - timer->current_ticks;
		timer->delta = (float)timer->delta_ticks / (float)CLOCKS_PER_SEC;
		timer->time += timer->delta;
		if(timer->delta > 0)
			timer->fps = 1.0f / timer->delta;
	}
	while (PRINT_DEBUG)
	{
		timer->delta_ticks = clock() - timer->current_ticks;
		timer->delta = (float)timer->delta_ticks / (float)CLOCKS_PER_SEC;
		timer->time += timer->delta;
		if(timer->delta > 0)
		{
			timer->fps = 1.0f / timer->delta;
			if ((int)timer->fps <= 30)
				break ;
			if ((int)timer->fps > 30)
				continue ;
		}
		else
			break;
	}
}

void	show_fps(t_app *app)
{
	printf("FPS: %d\n", (int)app->timer.fps);
}