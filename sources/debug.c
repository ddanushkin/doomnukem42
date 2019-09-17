#include "doom_nukem.h"

void	debug_mouse(t_app *app, char *event, int key_code)
{
	if (PRINT_DEBUG)
	{
		printf("\nDEBUG MOUSE:\n");
		printf("EVENT: \t\t %s\n", event);
		printf("KEY_CODE: \t %d\n", key_code);
		printf("X: \t\t\t %d\n", app->mouse.x);
		printf("Y: \t\t\t %d\n", app->mouse.y);
		printf("\n");
	}
}

void	debug_keyboard(char *event, int key_code)
{
	if (PRINT_DEBUG)
	{
		printf("\nDEBUG KEYBOARD:\n");
		printf("EVENT: \t\t %s\n", event);
		printf("KEY_CODE: \t %d\n", key_code);
		printf("\n");
	}
}