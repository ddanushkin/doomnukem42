#include <doom_nukem.h>


int		event_handling(t_app *app)
{
	const uint8_t *key;

	key = app->inputs->keyboard;
	SDL_PollEvent(&app->sdl->event);
	if (app->sdl->event.type == SDL_QUIT)
		return(0);
	if (key[SDL_SCANCODE_ESCAPE])
		return(0);
	if (key[SDL_SCANCODE_E] && app->speed > -0.1)
		app->speed -= 0.001f;
	if (key[SDL_SCANCODE_Q] && app->speed < 0.1)
		app->speed += 0.001f;
	if (key[SDL_SCANCODE_R])
		show_fps(app);
	return (1);
}