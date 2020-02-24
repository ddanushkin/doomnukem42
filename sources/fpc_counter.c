#include "doom_nukem.h"

void	update_fps_text(t_app *app)
{
	char	*fps_text;

	if(fmod(app->timer->time, 0.25) >= 0.24)
		app->timer->fps = 1.0 / app->timer->delta;
	fps_text = ft_itoa(app->timer->fps);
	print_to_screen(app, 0, 0, fps_text);
	ft_strdel(&fps_text);
}

void	get_delta_time(t_timer *timer)
{
	double current;

	timer->frame++;
	current = SDL_GetPerformanceCounter();
	timer->delta = (current - timer->prev) / (double)SDL_GetPerformanceFrequency();
	timer->prev = current;
	timer->time += timer->delta;
}
