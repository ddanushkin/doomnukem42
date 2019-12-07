#include <doom_nukem.h>

void 	process_camera(t_app *app)
{
	t_camera		*camera;

	camera = app->camera;
	camera->rotation = matrix_rotation(camera->rot.x, camera->rot.y, 0);
	camera->dir = matrix_multiply_vector(camera->rotation, new_vector(0.0, 0.0, 1.0));
	camera->target = vector_sum(camera->pos, camera->dir);
	camera->view = matrix_look_at(camera->pos, camera->target);
	camera->translation = matrix_translation(-camera->pos.x, -camera->pos.y, -camera->pos.z);
	camera->view_projection = matrix_multiply(camera->projection, matrix_multiply(camera->view, camera->translation));
}

void	process_inputs(t_app *app, double delta_time)
{
	t_mouse_state	*mouse;
	const uint8_t	*key;
	t_v3d			forward;
	t_v3d			right;
	t_camera		*camera;

	camera = app->camera;
	key = app->inputs->keyboard;
	mouse = &app->inputs->mouse;
	forward = vector_mul_by(camera->dir, 2.0 * delta_time);
	right = new_vector(forward.z, 0.0, -forward.x);
	forward.y = 0;
	if (mouse->x)
		camera->rot.y += mouse->x * mouse->sens * delta_time * 0.25;
	if (mouse->y)
		camera->rot.x -= mouse->y * mouse->sens * delta_time * 0.25;
	if (key[SDL_SCANCODE_W])
		camera->pos = vector_sum(camera->pos, forward);
	if (key[SDL_SCANCODE_S])
		camera->pos = vector_sub(camera->pos, forward);
	if (key[SDL_SCANCODE_A])
		camera->pos = vector_sub(camera->pos, right);
	if (key[SDL_SCANCODE_D])
		camera->pos = vector_sum(camera->pos, right);
}

int		event_handling(t_app *app)
{
	SDL_PollEvent(&app->sdl->event);
	if (app->sdl->event.type == SDL_QUIT)
		return(0);
	if (app->inputs->keyboard[SDL_SCANCODE_ESCAPE])
		return(0);
	process_inputs(app, app->timer->delta);
	process_camera(app);
	return (1);
}