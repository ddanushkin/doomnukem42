#include "doom_nukem.h"

void		mouse_update(t_app *app)
{
	Uint32	mouse_state;
	int		x;
	int		y;

	mouse_state = SDL_GetRelativeMouseState(&x, &y);
	if (mouse_state & SDL_BUTTON(SDL_BUTTON_LEFT))
		app->inputs->mouse.left = 1;
	else
		app->inputs->mouse.left = 0;
	if (mouse_state & SDL_BUTTON(SDL_BUTTON_RIGHT))
		app->inputs->mouse.right = 1;
	else
		app->inputs->mouse.right = 0;
	if (mouse_state & SDL_BUTTON(SDL_BUTTON_MIDDLE))
		app->inputs->mouse.middle = 1;
	else
		app->inputs->mouse.middle = 0;
	app->inputs->mouse.x = (double)x / (double)SCREEN_W * 1000.0;
	app->inputs->mouse.y = (double)y / (double)SCREEN_H * 1000.0;
}