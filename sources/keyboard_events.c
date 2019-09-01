#include "doom_nukem.h"

int 	keyboard_event_down(int key_code, t_app *app)
{
	if (PRINT_DEBUG)
		printf("keyboard event -> down\n");
	app->keyboard.down[key_code] = 1;
	return (0);
}

int 	keyboard_event_up(int key_code, t_app *app)
{
	if (PRINT_DEBUG)
		printf("keyboard event -> up\n");
	app->keyboard.up[key_code] = 1;
	return (0);
}