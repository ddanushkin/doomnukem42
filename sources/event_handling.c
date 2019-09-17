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
		app->camera.pos.y -= 0.01f;
	if (key[SDL_SCANCODE_SPACE])
		app->camera.pos.y += 0.01f;

	t_vector	forward = vector_mul_by(app->camera.dir, 0.01f);
	t_vector	right = vector_new(forward.z, 0.0f, -forward.x);

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
	if (app->mouse.x)
		app->camera.rot.y += app->mouse.x < 0 ? -0.01f : 0.01f;
	if (app->mouse.y)
		app->camera.rot.x += app->mouse.y < 0 ? 0.01f : -0.01f;

	return (1);
}