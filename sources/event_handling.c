#include <doom_nukem.h>

int		event_handling(t_app *app, double delta_time)
{
	t_mouse_state	*mouse;
	const uint8_t	*key;
	t_v3d			forward;
	t_v3d			right;
	t_camera		*camera;

	key = app->inputs->keyboard;
	mouse = &app->inputs->mouse;
	camera = app->camera;
	SDL_PollEvent(&app->sdl->event);
	if (app->sdl->event.type == SDL_QUIT)
		return(0);
	if (key[SDL_SCANCODE_ESCAPE])
		return(0);

	/* ROTATE CAMERA */
	if (mouse->x)
		camera->rot.y += mouse->x * mouse->sens * delta_time * 0.25;
	if (mouse->y)
		camera->rot.x -= mouse->y * mouse->sens * delta_time * 0.25;

	/* Create camera view matrix */
	camera->rotation = matrix_rotation(camera->rot.x, camera->rot.y, 0);
	camera->dir = matrix_multiply_vector(camera->rotation, new_vector(0.0, 0.0, 1.0));
	camera->target = vector_sum(camera->pos, camera->dir);
	camera->view = matrix_look_at(camera->pos, camera->target);

	forward = vector_mul_by(camera->dir, 2.0 * delta_time);
	forward.y = 0;
	right = new_vector(forward.z, 0.0, -forward.x);

	/* MOVE FORWARD/BACKWARD */
	if (key[SDL_SCANCODE_W])
		camera->pos = vector_sum(camera->pos, forward);
	if (key[SDL_SCANCODE_S])
		camera->pos = vector_sub(camera->pos, forward);
	/* MOVE LEFT/RIGHT */
	if (key[SDL_SCANCODE_A])
		camera->pos = vector_sub(camera->pos, right);
	if (key[SDL_SCANCODE_D])
		camera->pos = vector_sum(camera->pos, right);
	camera->translation = matrix_translation(-camera->pos.x, -camera->pos.y, -camera->pos.z);
	camera->view_projection = matrix_multiply(camera->projection, matrix_multiply(camera->view, camera->translation));
	return (1);
}