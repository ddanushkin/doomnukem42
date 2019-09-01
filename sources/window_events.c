#include "doom_nukem.h"

int		window_event_close(t_app *app)
{
	if (PRINT_DEBUG)
		printf("window event -> close\n");
	app_close(app);
	return (0);
}

int		window_event_expose(t_app *app)
{
	if (PRINT_DEBUG)
		printf("window event -> expose\n");
	(void)app;
	return (0);
}

