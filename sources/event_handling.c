#include <doom_nukem.h>

void 	move(t_camera *camera, t_v3d dir, double amount)
{
	camera->pos = vector_sum(camera->pos, vector_mul_by(dir, amount));
}

t_mat4x4 view_matrix(t_v3d eye, double pitch, double yaw)
{
	double		cosPitch = cos(pitch);
	double		sinPitch = sin(pitch);
	double		cosYaw = cos(yaw);
	double		sinYaw = sin(yaw);
	t_v3d		xaxis = new_vector(cosYaw, 0.0, -sinYaw);
	t_v3d		yaxis = new_vector(sinYaw * sinPitch, cosPitch, cosYaw * sinPitch);
	t_v3d		zaxis = new_vector(sinYaw * cosPitch, -sinPitch, cosPitch * cosYaw);
	t_mat4x4	view;

	view.m[0] = xaxis.x;
	view.m[1] = xaxis.y;
	view.m[2] = xaxis.z;
	view.m[3] = -vector_dot_product(xaxis, eye);
	view.m[4] = yaxis.x;
	view.m[5] = yaxis.y;
	view.m[6] = yaxis.z;
	view.m[7] = -vector_dot_product(yaxis, eye);
	view.m[8] = zaxis.x;
	view.m[9] = zaxis.y;
	view.m[10] = zaxis.z;
	view.m[11] = -vector_dot_product(zaxis, eye);
	view.m[12] = 0.0;
	view.m[13] = 0.0;
	view.m[14] = 0.0;
	view.m[15] = 1.0;
	return (view);
}

void 	update_camera(t_camera *c)
{
	double	x_acos;

	c->view = view_matrix(c->pos, c->rot.x, c->rot.y);
	c->view_projection = matrix_multiply(c->projection, c->view);
	c->transform = get_transform_matrix(c->view_projection);
	c->dir.x = c->view.m[8];
	c->dir.y = c->view.m[9];
	c->dir.z = c->view.m[10];
	x_acos = acos(c->dir.x);
	if (x_acos < 0.392699)
		c->quad = 0;
	else if (x_acos >= 0.392699 && x_acos < 1.178097)
		c->quad = 1;
	else if (x_acos >= 1.178097 && x_acos < 1.963495)
		c->quad = 2;
	else if (x_acos >= 1.963495 && x_acos < 2.748893)
		c->quad = 3;
	else if (x_acos >= 2.748893)
		c->quad = 4;
	if (acos(c->dir.z) > 1.570796 && c->quad != 0 && c->quad != 4)
		c->quad = 8 - c->quad;
	c->right = new_vector(c->view.m[0], c->view.m[1], c->view.m[2]);
	c->forward = new_vector(c->view.m[8], 0.0, c->view.m[10]);
//	printf("[%f, %f, %f]\n", c->rot.x, c->rot.y, c->rot.z);
//	printf("[%f, %f, %f]\n", c->forward.x, c->forward.y, c->forward.z);
}

void	process_inputs(t_app *app, double delta_time)
{
	t_mouse_state	*mouse;
	const uint8_t	*key;
	t_camera		*c;

	double			mouse_speed = 1.2123 * delta_time;

	c = app->camera;
	key = app->inputs->keyboard;
	mouse = &app->inputs->mouse;
	if (mouse->x && mouse->y)
		mouse_speed = mouse_speed / 1.414;
	if (mouse->x)
		c->rot.y += (double)mouse->x * mouse_speed;
	if (mouse->y)
		c->rot.x += (double)mouse->y * mouse_speed;
	c->rot.x = CLAMP(c->rot.x, -1.45, 1.45);
	app->camera->pos_prev = app->camera->pos;
	if (key[SDL_SCANCODE_W])
		move(c, c->forward, 4.54321 * delta_time);
	if (key[SDL_SCANCODE_S])
		move(c, c->forward, -4.54321 * delta_time);
	if (key[SDL_SCANCODE_A])
		move(c, c->right, -4.54321 * delta_time);
	if (key[SDL_SCANCODE_D])
		move(c, c->right, 4.54321 * delta_time);
}

int		event_handling(t_app *app)
{
	SDL_Event	e;
	Uint32		event;

	app->inputs->mouse.x = 0;
	app->inputs->mouse.y = 0;
	while (SDL_PollEvent(&e))
	{
		event = e.type;
		if (event == SDL_QUIT)
			return(0);
		if (event == SDL_MOUSEMOTION)
		{
			app->inputs->mouse.x = e.motion.xrel;
			app->inputs->mouse.y = e.motion.yrel;
		}
		if (event == SDL_MOUSEBUTTONDOWN)
		{
			if (e.button.button == SDL_BUTTON_LEFT)
				app->inputs->mouse.left = 1;
			if (e.button.button == SDL_BUTTON_RIGHT)
				app->inputs->mouse.right = 1;
			if (e.button.button == SDL_BUTTON_MIDDLE)
				app->inputs->mouse.middle = 1;
		}
		if (event == SDL_MOUSEBUTTONUP)
		{
			if (e.button.button == SDL_BUTTON_LEFT)
				app->inputs->mouse.left = 0;
			if (e.button.button == SDL_BUTTON_RIGHT)
				app->inputs->mouse.right = 0;
			if (e.button.button == SDL_BUTTON_MIDDLE)
				app->inputs->mouse.middle = 0;
		}
		if (event == SDL_KEYDOWN)
		{
			if (e.key.keysym.scancode == SDL_SCANCODE_G)
				app->input_g = 1;
			if (e.key.keysym.scancode == SDL_SCANCODE_MINUS)
				app->input_minus = 1;
			if (e.key.keysym.scancode == SDL_SCANCODE_EQUALS)
				app->input_plus = 1;
		}
		if (event == SDL_KEYUP)
		{
			if (e.key.keysym.scancode == SDL_SCANCODE_G)
				app->input_g = 0;
			if (e.key.keysym.scancode == SDL_SCANCODE_MINUS)
				app->input_minus = 0;
			if (e.key.keysym.scancode == SDL_SCANCODE_EQUALS)
				app->input_plus = 0;
		}

	}
	return (1);
}