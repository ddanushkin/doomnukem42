#include <doom_nukem.h>


int		event_handling(t_app *app)
{
	SDL_PollEvent(&app->sdl->event);
	if (app->sdl->event.type == SDL_QUIT)
		return(0);
	if (app->inputs->keyboard[SDL_SCANCODE_ESCAPE])
		return(0);
	if (app->inputs->keyboard[SDL_SCANCODE_E])
		app->speed -= 0.01f;
	if (app->inputs->keyboard[SDL_SCANCODE_Q])
		app->speed += 0.01f;
	return (1);
}