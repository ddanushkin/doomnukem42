#include "doom_nukem.h"

void		mouse_update(t_app *app)
{
	Uint32	mouse_state;

	mouse_state = SDL_GetRelativeMouseState(&app->mouse.x, &app->mouse.y);
	if (mouse_state & SDL_BUTTON(SDL_BUTTON_LEFT))
		app->mouse.left = 1;
	else
		app->mouse.left = 0;
	if (mouse_state & SDL_BUTTON(SDL_BUTTON_RIGHT))
		app->mouse.right = 1;
	else
		app->mouse.right = 0;
	if (mouse_state & SDL_BUTTON(SDL_BUTTON_MIDDLE))
		app->mouse.middle = 1;
	else
		app->mouse.middle = 0;
}