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
		app->camera.pos.y += 0.5f;
	if (key[SDL_SCANCODE_SPACE])
		app->camera.pos.y -= 0.5f;

	t_vector	forward = vector_mul_by(app->camera.dir, 0.01f);
	t_vector	right = vector_new(forward.z, forward.y, -forward.x, 0.0f);

	/* MOVE FORWARD/BACKWARD */
	if (key[SDL_SCANCODE_W])
		app->camera.pos = vector_sum(app->camera.pos, forward);
	if (key[SDL_SCANCODE_S])
		app->camera.pos = vector_sub(app->camera.pos, forward);

	/* MOVE LEFT/RIGHT */
	if (key[SDL_SCANCODE_A])
		app->camera.pos = vector_sum(app->camera.pos, right);
	if (key[SDL_SCANCODE_D])
		app->camera.pos = vector_sub(app->camera.pos, right);

	/* ROTATE CAMERA */
	if (key[SDL_SCANCODE_LEFT])
		app->camera.rot.y += 0.01f;
	if (key[SDL_SCANCODE_RIGHT])
		app->camera.rot.y -= 0.01f;
	if (key[SDL_SCANCODE_UP])
		app->camera.rot.x += 0.01f;
	if (key[SDL_SCANCODE_DOWN])
		app->camera.rot.x -= 0.01f;

	return (1);
}