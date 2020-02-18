#include <doom_nukem.h>

void 	move_c(t_app *app, t_v3d *pos, t_v3d dir, double amount)
{
	if (!app->camera->fly)
		check_collision(app, pos, v3d_mul_by(dir, amount));
	else
		move(pos, dir, amount);
}

void 	move(t_v3d *v, t_v3d dir, double amount)
{
	*v = v3d_sum(*v, v3d_mul_by(dir, amount));
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
	view.m[3] = -v3d_dot(xaxis, eye);
	view.m[4] = yaxis.x;
	view.m[5] = yaxis.y;
	view.m[6] = yaxis.z;
	view.m[7] = -v3d_dot(yaxis, eye);
	view.m[8] = zaxis.x;
	view.m[9] = zaxis.y;
	view.m[10] = zaxis.z;
	view.m[11] = -v3d_dot(zaxis, eye);
	view.m[12] = 0.0;
	view.m[13] = 0.0;
	view.m[14] = 0.0;
	view.m[15] = 1.0;
	return (view);
}

void	get_head(t_app *app)
{
	if (app->inputs->keyboard[SDL_SCANCODE_LSHIFT] && app->acc < 1.4)
		app->acc += 0.25;
	if (!app->inputs->keyboard[SDL_SCANCODE_LSHIFT] && app->acc > 1.0)
		app->acc -= 0.50;
	app->acc = CLAMP(app->acc, 0.5, 1.4);
	app->speed = 4.54321 * app->acc;
}

void 	update_points_camera(t_camera *c)
{
	c->view = view_matrix(c->pos, c->rot.x, c->rot.y);
	c->view_projection = matrix_multiply(c->projection, c->view);
	c->transform = get_transform_matrix(c->view_projection);
	c->dir.x = c->view.m[8];
	c->dir.y = c->view.m[9];
	c->dir.z = c->view.m[10];
}

void 	update_camera(t_app *app, t_camera *c)
{
	double	x_acos;

	get_head(app);
	c->view = view_matrix(app->camera->pos, c->rot.x, c->rot.y);
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
	c->forward = new_vector(c->view.m[8], c->view.m[9], c->view.m[10]);
	if (!c->fly)
		c->forward.y = 0.0;
}


void 	cursor_clamp(t_app *app)
{
	if (app->cursor_x >= (double)SCREEN_W)
		app->cursor_x = 0.0;
	else if (app->cursor_x <= 0.0)
		app->cursor_x = (double)SCREEN_W;
	if (app->cursor_y >= (double)SCREEN_H)
		app->cursor_y = 0.0;
	else if (app->cursor_y <= 0.0)
		app->cursor_y = (double)SCREEN_H;
}

void	process_points_inputs(t_app *app, double delta_time)
{
	const uint8_t	*key;
	t_camera		*c;
	t_mouse_state	*mouse;

	mouse = &app->inputs->mouse;
	c = app->camera;
	key = app->inputs->keyboard;
	if (key[SDL_SCANCODE_W])
		c->pos.z += app->speed * delta_time;
	if (key[SDL_SCANCODE_S])
		c->pos.z -= app->speed * delta_time;
	if (key[SDL_SCANCODE_A])
		c->pos.x -= app->speed * delta_time;
	if (key[SDL_SCANCODE_D])
		c->pos.x += app->speed * delta_time;
	if (app->mouse[SDL_MOUSE_SCROLL_UP])
		c->pos.y += app->speed * 50.0 * delta_time;
	if (app->mouse[SDL_MOUSE_SCROLL_DOWN])
		c->pos.y -= app->speed * 50.0 * delta_time;
	if (mouse->x != 0)
		app->cursor_x += mouse->x * 2.0;
	if (mouse->y != 0)
		app->cursor_y += mouse->y * 2.0;
	cursor_clamp(app);
}

void	process_inputs(t_app *app, double dt)
{
	t_mouse_state	*mouse;
	const uint8_t	*key;
	t_camera		*c;
	double			mouse_speed = 1.2123 * dt;

	c = app->camera;
	key = app->inputs->keyboard;
	mouse = &app->inputs->mouse;
	if (mouse->x && mouse->y)
		mouse_speed = mouse_speed / 1.414;
	if (mouse->x)
		c->rot.y += (double)mouse->x * mouse_speed;
	if (mouse->y)
		c->rot.x += (double)mouse->y * mouse_speed;
	if (!app->point_mode)
		c->rot.x = CLAMP(c->rot.x, -1.45, 1.45);

	if (key[SDL_SCANCODE_W])
		move_c(app, &c->pos, c->forward, app->speed * dt);
	if (key[SDL_SCANCODE_S])
		move_c(app, &c->pos, c->forward, -app->speed * dt);
	if (key[SDL_SCANCODE_A])
		move_c(app, &c->pos, c->right, -app->speed * dt);
	if (key[SDL_SCANCODE_D])
		move_c(app, &c->pos, c->right, app->speed * dt);

	if (key[SDL_SCANCODE_LCTRL] && app->height > 0.5)
	{
		app->height -= 2.5 * dt;
		if (app->height < 0.5)
			app->height = 0.5;
	}
	else if(!key[SDL_SCANCODE_LCTRL] && app->height < PLAYER_HEIGHT)
	{
		app->height += 2.5 * dt;
		if (app->height > PLAYER_HEIGHT)
			app->height = PLAYER_HEIGHT;
	}

	double	dy;
	dy = (app->height - fabs(app->floor_point.y - c->pos.y)) * -1.0;
	//printf("dy -> %f\n", dy);

	if (dy > 0.0 && app->ground == 1)
		app->ground = 0;

	if (app->ground == 0 && !app->jumped)
		app->falling += dt;

	//printf("[%d, %d, %f]\n", app->ground, app->jumped, app->falling);

	if (app->keys[SDL_SCANCODE_SPACE] && !app->jumped && (app->ground || (app->falling > 0.0 && app->falling < 0.25)))
	{
		app->y_vel = 12.0;
		app->jumped = 1;
		app->ground = 0;
		app->falling = 0.0;
		printf("[%llu] jump\n", app->timer->frame);
	}

	if (!app->ground && !app->camera->fly)
	{
		app->y_acc += 2.5 * dt;
		app->y_vel -= app->y_acc;
	}
	if (app->ground)
	{
		app->y_acc = 0.0;
		app->y_vel = 0.0;
	}

	if (app->y_vel != 0.0 && app->y_vel < -12.0)
		app->y_vel = -12.0;

	if (!app->camera->fly)
		c->pos.y += app->y_vel * dt;

	dy = (app->height - fabs(app->floor_point.y - c->pos.y)) * -1.0;

	if (dy < 0.0 && app->y_vel < 0.0)
	{
		printf("[out][%llu, %f]\n\n", app->timer->frame, dy);
		c->pos.y = app->floor_point.y + app->height;
		app->falling = 0.0;
		app->jumped = 0;
		app->ground = 1;
	}

	if (dy < 0.0 && app->y_vel == 0.0)
	{
		if (fabs(dy) < 0.7 && fabs(dy) >= 0.25)
		{
			app->y_vel = 5.0;
			printf("[big][%llu, %f, %f]\n\n", app->timer->frame, dy, app->y_vel);
			app->ground = 0;
		}
		else if (fabs(dy) < 0.25 && !app->camera->fly)
		{
			printf("[small][%llu, %f]\n\n", app->timer->frame, dy);
			c->pos.y = app->floor_point.y + app->height;
			app->falling = 0;
			app->jumped = 0;
			app->ground = 1;
		}
	}
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
		if (event == SDL_MOUSEWHEEL)
		{
			if (e.wheel.y > 0)
				app->mouse[SDL_MOUSE_SCROLL_UP] = 1;
			else if (e.wheel.y < 0)
				app->mouse[SDL_MOUSE_SCROLL_DOWN] = 1;
		}
		if (event == SDL_MOUSEBUTTONDOWN)
			app->mouse[e.button.button] = 1;
		if (event == SDL_KEYDOWN)
			app->keys[e.key.keysym.scancode] = 1;
	}
	return (1);
}