#include "doom_nukem.h"

int 	keyboard_event_down(int key_code, t_app *app)
{
	app->keyboard.down[key_code] = 1;
	debug_keyboard("key_down", key_code);
	return (0);
}

int 	keyboard_event_up(int key_code, t_app *app)
{
	app->keyboard.up[key_code] = 1;
	debug_keyboard("key_up", key_code);
	return (0);
}