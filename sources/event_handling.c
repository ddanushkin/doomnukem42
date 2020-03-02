#include <doom_nukem.h>

void 	move(t_v3d *v, t_v3d dir, double amount)
{
	*v = v3d_sum(*v, v3d_mul_by(dir, amount));
}

void		view_matrix_data_calc(t_vm_data *d, double pitch, double yaw)
{
	d->cosp = cos(pitch);
	d->sinp = sin(pitch);
	d->cosy = cos(yaw);
	d->siny = sin(yaw);
	d->xa = new_vector(d->cosy, 0.0, -d->siny);
	d->ya = new_vector(d->siny * d->sinp, d->cosp, d->cosy * d->sinp);
	d->za = new_vector(d->siny * d->cosp, -d->sinp, d->cosp * d->cosy);
}

t_mat4x4	view_matrix(t_v3d eye, double pitch, double yaw)
{
	t_vm_data	vmd;
	t_mat4x4	view;

	view_matrix_data_calc(&vmd, pitch, yaw);
	view.m[0] = vmd.xa.x;
	view.m[1] = vmd.xa.y;
	view.m[2] = vmd.xa.z;
	view.m[3] = -v3d_dot(vmd.xa, eye);
	view.m[4] = vmd.ya.x;
	view.m[5] = vmd.ya.y;
	view.m[6] = vmd.ya.z;
	view.m[7] = -v3d_dot(vmd.ya, eye);
	view.m[8] = vmd.za.x;
	view.m[9] = vmd.za.y;
	view.m[10] = vmd.za.z;
	view.m[11] = -v3d_dot(vmd.za, eye);
	view.m[12] = 0.0;
	view.m[13] = 0.0;
	view.m[14] = 0.0;
	view.m[15] = 1.0;
	return (view);
}

void	get_head(t_app *app)
{
	if (app->inputs->keyboard[SDL_SCANCODE_LSHIFT] && app->acc < 3.5)
		app->acc += 5.5 * app->timer->delta;
	if (!app->inputs->keyboard[SDL_SCANCODE_LSHIFT] && app->acc > 0)
		app->acc -= 5.5 * app->timer->delta;

	if (app->acc < 0.0)
		app->acc = 0.0;
	if (app->acc > 3.5)
		app->acc = 3.5;

	app->speed = PLAYER_SPEED + app->acc;
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

	if (!c->fly)
		c->forward = new_vector(sin(app->camera->rot.y), 0, cos(app->camera->rot.y));
	else
		c->forward = new_vector(c->view.m[8], c->view.m[9], c->view.m[10]);
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

void	update_floor_dist(t_app *app, t_v3d new_pos)
{
	int			i;
	int			j;
	t_sector	*s;

	i = 0;
	while (i < app->sectors_count)
	{
		s = &app->sectors[i];
		app->cs = s;
		j = 0;
		while (j < s->trs_count)
		{
			if (s->floor.active)
			{
				ray_floor(app, new_pos, s->ftrs[j]);
				ray_ceil(app, new_pos, s->ftrs[j]);
			}
			if (s->ceil.active)
			{
				ray_floor(app, new_pos, s->ctrs[j]);
				ray_ceil(app, new_pos, s->ctrs[j]);
			}
			j++;
		}
		i++;
	}
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
	if (!app->point_mode && !app->camera->fly)
		c->rot.x = CLAMP(c->rot.x, -1.45, 1.45);

	app->last_dir = new_vector(0.0, 0.0, 0.0);
	if (key[SDL_SCANCODE_W])
		app->last_dir = v3d_sum(app->last_dir, v3d_mul_by(c->forward, app->speed * dt));
	if (key[SDL_SCANCODE_S])
		app->last_dir = v3d_sum(app->last_dir, v3d_mul_by(c->forward, -app->speed * dt));
	if (key[SDL_SCANCODE_A])
		app->last_dir = v3d_sum(app->last_dir, v3d_mul_by(c->right, -app->speed * dt));
	if (key[SDL_SCANCODE_D])
		app->last_dir = v3d_sum(app->last_dir, v3d_mul_by(c->right, app->speed * dt));

	if (app->keys[SDL_SCANCODE_SPACE] && !app->jumped && (app->ground || (app->falling > 0.0 && app->falling < 0.25)))
	{
		Mix_VolumeMusic(5);
//		Mix_PlayMusic(app->sfx[app->si], 0);
		app->y_vel = 6.0;
		app->jumped = 1;
		app->ground = 0;
		app->falling = 0.0;
	}

	if (!app->camera->fly)
	{
		c->pos.y += app->y_vel * dt;
		app->floor_dist = 10000.0;
		app->ceil_dist = 10000.0;
		app->floor_sector = NULL;
		app->ceil_sector = NULL;
		update_floor_dist(app, c->pos);
		app->prev_dy = app->floor_dist;
		check_collision(app, &c->pos, app->last_dir);
		app->floor_dist = 10000.0;
		app->ceil_dist = 10000.0;
		app->floor_sector = NULL;
		app->ceil_sector = NULL;
		update_floor_dist(app, c->pos);

		if (app->floor_dist > app->height)
			app->ground = 0;

		double dy = app->height - app->floor_dist;

		if (dy >= 0.0 && dy < 0.85)
		{
			app->ground = 1;
			app->falling = 0.0;
			app->jumped = 0;
			app->y_vel = 0.0;
			app->y_acc = 0.0;
			c->pos.y += app->height - app->floor_dist;
		}

		if (!app->ground)
		{
			if (app->y_acc == 0.0)
				app->y_acc = 15.8;
			app->y_vel -= app->y_acc * dt;
			if (app->y_vel <= 0)
				app->y_acc += 15.8 * dt;
			app->falling += dt;
		}
	}

	if (app->camera->fly)
		c->pos = v3d_sum(c->pos, app->last_dir);

	int head_too_high = app->ceil_sector && fabs(c->pos.y - app->ceil_point.y) < app->height;
	if (!app->camera->fly && app->y_vel > 0.0 && head_too_high)
		app->y_vel *= -1;
	if ((key[SDL_SCANCODE_LCTRL] && app->height > 0.5) || head_too_high)
		app->height -= 2.5 * dt;
	else if(!key[SDL_SCANCODE_LCTRL] && app->height < PLAYER_HEIGHT)
		app->height += 2.5 * dt;
	app->height = CLAMP(app->height, 0.5, PLAYER_HEIGHT);
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
