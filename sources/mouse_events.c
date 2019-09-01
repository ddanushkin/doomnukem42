#include "doom_nukem.h"

void	mouse_update(t_app *app, int x, int y)
{
	app->mouse.x_prev = app->mouse.x;
	app->mouse.y_prev = app->mouse.y;
	app->mouse.x = x;
	app->mouse.y = y;
	if (app->mouse.y_prev < app->mouse.y)
		app->mouse.vdir = -1;
	else if (app->mouse.y_prev > app->mouse.y)
		app->mouse.vdir = 1;
	if (app->mouse.x_prev > app->mouse.x)
		app->mouse.hdir = -1;
	else if (app->mouse.x_prev < app->mouse.x)
		app->mouse.hdir = 1;
}

int 	mouse_event_down(int key_code, int x, int y, t_app *app)
{
	app->mouse.keys.down[key_code] = 1;
	mouse_update(app, x, y);
	debug_mouse(app, "key_down", key_code);
	return (0);
}
int 	mouse_event_up(int key_code, int x, int y, t_app *app)
{
	app->mouse.keys.up[key_code] = 1;
	mouse_update(app, x, y);
	debug_mouse(app, "key_up", key_code);
	return (0);
}

int 	mouse_event_move(int x, int y, t_app *app)
{
	app->mouse.move = 1;
	mouse_update(app, x, y);
	debug_mouse(app, "move", -1);
	return (0);
}