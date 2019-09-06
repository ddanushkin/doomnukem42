#include <doom_nukem.h>


void	event_handling(t_app *app)
{
	if (app->inputs->keyboard[SDL_SCANCODE_ESCAPE])
		exit(0);
	if (app->inputs->keyboard[SDL_SCANCODE_E])
		app->speed -= 0.01f;
	if (app->inputs->keyboard[SDL_SCANCODE_Q])
		app->speed += 0.01f;
}