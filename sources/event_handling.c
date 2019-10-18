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

	if (key[SDL_SCANCODE_R])
		show_fps(app);

	/* CHANGE HEIGHT */
	if (key[SDL_SCANCODE_LSHIFT])
		app->camera->pos.y -= 5.0 * app->timer->delta;
	if (key[SDL_SCANCODE_SPACE])
		app->camera->pos.y += 5.0 * app->timer->delta;

	t_v3d	forward = vector_mul_by(app->camera->dir, 15.0 * app->timer->delta);
	t_v3d	right = new_vector(forward.z, 0.0, -forward.x);

	/* MOVE FORWARD/BACKWARD */
	if (key[SDL_SCANCODE_W])
		app->camera->pos = vector_sum(app->camera->pos, forward);
	if (key[SDL_SCANCODE_S])
		app->camera->pos = vector_sub(app->camera->pos, forward);
	/* MOVE LEFT/RIGHT */
	if (key[SDL_SCANCODE_A])
		app->camera->pos = vector_sum(app->camera->pos, right);
	if (key[SDL_SCANCODE_D])
		app->camera->pos = vector_sub(app->camera->pos, right);
	/* ROTATE CAMERA */
	if (app->inputs->mouse.x)
		app->camera->rot.y += app->inputs->mouse.x * app->inputs->mouse.sens * app->timer->delta;
	if (app->inputs->mouse.y)
		app->camera->rot.x -= app->inputs->mouse.y * app->inputs->mouse.sens * app->timer->delta;
	app->camera->rot.x = CLAMP(app->camera->rot.x, -1.5, 1.5);
	return (1);
}