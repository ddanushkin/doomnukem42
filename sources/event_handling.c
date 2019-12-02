#include <doom_nukem.h>

int		event_handling(t_app *app)
{
	const uint8_t *key;
	t_v3d		forward;
	t_v3d		right;
	t_camera	*camera;

	key = app->inputs->keyboard;
	SDL_PollEvent(&app->sdl->event);
	if (app->sdl->event.type == SDL_QUIT)
		return(0);
	if (key[SDL_SCANCODE_ESCAPE])
		return(0);

	/* ROTATE CAMERA */
	if (app->inputs->mouse.x)
		app->camera->rot.y -= app->inputs->mouse.x * app->inputs->mouse.sens * app->timer->delta;
	if (app->inputs->mouse.y)
		app->camera->rot.x -= app->inputs->mouse.y * app->inputs->mouse.sens * app->timer->delta;

	camera = app->camera;
	camera->view = FPSViewRH(app->camera);
	right = new_vector(camera->view.m[0][0], camera->view.m[0][1], camera->view.m[0][2]);
	forward = new_vector(camera->view.m[2][0], camera->view.m[2][1], camera->view.m[2][2]);

	right.y = 0.0;
	forward.y = 0.0;

	/* MOVE LEFT/RIGHT */
	if (key[SDL_SCANCODE_A])
		camera->pos = vector_sum(app->camera->pos, vector_mul_by(right, 10.0 * app->timer->delta));
	if (key[SDL_SCANCODE_D])
		camera->pos = vector_sum(app->camera->pos, vector_mul_by(right, -10.0 * app->timer->delta));
	/* MOVE FORWARD/BACKWARD */
	if (key[SDL_SCANCODE_W])
		camera->pos = vector_sum(app->camera->pos, vector_mul_by(forward, -10.0 * app->timer->delta));
	if (key[SDL_SCANCODE_S])
		camera->pos = vector_sum(app->camera->pos, vector_mul_by(forward, 10.0 * app->timer->delta));

//	/* CHANGE HEIGHT */
//	if (key[SDL_SCANCODE_LSHIFT])
//		app->camera->pos.y += 1.0 * app->timer->delta;
//	if (key[SDL_SCANCODE_SPACE])
//		app->camera->pos.y -= 1.0 * app->timer->delta;

	app->camera->translation = matrix_translation(app->camera->pos.x, app->camera->pos.y, app->camera->pos.z);

	camera->view_projection = matrix_multiply(camera->projection, matrix_multiply(camera->view, camera->translation));
	return (1);
}