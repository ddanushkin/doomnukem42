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

	/* ROTATE CAMERA */
	if (app->inputs->mouse.x)
		app->camera->rot.y += app->inputs->mouse.x * app->inputs->mouse.sens * app->timer->delta * 0.5  * 0.5;
	if (app->inputs->mouse.y)
		app->camera->rot.x -= app->inputs->mouse.y * app->inputs->mouse.sens * app->timer->delta * 0.5 * 0.5;

	/* Create camera view matrix */
	app->camera->rot_mat = init_rotation_matrix(app->camera->rot);
	app->camera->dir = matrix_multiply_vector(app->camera->rot_mat, new_vector(0.0, 0.0, 1.0));
	app->camera->target = vector_sum(app->camera->pos, app->camera->dir);
	app->camera->view_mat = matrix_inverse(matrix_look_at(app->camera->pos, app->camera->target));

	t_v3d	forward;
	t_v3d	right;

	forward = vector_mul_by(app->camera->dir, 2.0 * app->timer->delta);
	forward.y = 0;
	right = new_vector(forward.z, 0.0, -forward.x);

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
	return (1);
}