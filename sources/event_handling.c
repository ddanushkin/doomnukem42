#include <doom_nukem.h>

t_v3d	qt_axis_angle(t_v3d axis, double angle)
{
	t_v3d	qt;
	double	sinHalfAngle = sin(angle / 2);
	double	cosHalfAngle = cos(angle / 2);

	qt.x = axis.x * sinHalfAngle;
	qt.y = axis.y * sinHalfAngle;
	qt.z = axis.z * sinHalfAngle;
	qt.w = cosHalfAngle;
	return (qt);
}

t_v3d	qt_mul_qt(t_v3d m, t_v3d r)
{
	t_v3d	qt;
	qt.w = m.w * r.w - m.x * r.x - m.y * r.y - m.z * r.z;
	qt.x = m.x * r.w + m.w * r.x + m.y * r.z - m.z * r.y;
	qt.y = m.y * r.w + m.w * r.y + m.z * r.x - m.x * r.z;
	qt.z = m.z * r.w + m.w * r.z + m.x * r.y - m.y * r.x;
	return (qt);
};

t_v3d	qt_mul_v3d(t_v3d m, t_v3d r)
{
	t_v3d	qt;
	qt.w = -m.x * r.x - m.y * r.y - m.z * r.z;
	qt.x =  m.w * r.x + m.y * r.z - m.z * r.y;
	qt.y =  m.w * r.y + m.z * r.x - m.x * r.z;
	qt.z =  m.w * r.z + m.x * r.y - m.y * r.x;
	return (qt);
};

double	qt_length(t_v3d m)
{
	return sqrt(m.x * m.x + m.y * m.y + m.z * m.z + m.w * m.w);
};

t_v3d	qt_normalized(t_v3d m)
{
	t_v3d	qt;
	double length = qt_length(m);

	qt.x = m.x / length;
	qt.y = m.y / length;
	qt.z = m.z / length;
	qt.w = m.w / length;
	return (qt);
};

t_v3d	qt_conjugate(t_v3d m)
{
	t_v3d	qt;

	qt.x = -m.x;
	qt.y = -m.y;
	qt.z = -m.z;
	qt.w = m.w;
	return (qt);
};

t_mat4x4	mat_rotation_fur(t_v3d f, t_v3d u, t_v3d r)
{
	t_mat4x4	mat;

	mat.m[0] = r.x;
	mat.m[1] = r.y;
	mat.m[2] = r.z;
	mat.m[3] = 0;
	mat.m[4] = u.x;
	mat.m[5] = u.y;
	mat.m[6] = u.z;
	mat.m[7] = 0;
	mat.m[8] = f.x;
	mat.m[9] = f.y;
	mat.m[10] = f.z;
	mat.m[11] = 0;
	mat.m[12] = 0;
	mat.m[13] = 0;
	mat.m[14] = 0;
	mat.m[15] = 1;
	return (mat);
}

t_mat4x4	qt_to_rot_mat(t_v3d m)
{
	t_v3d	f;
	t_v3d	u;
	t_v3d	r;

	f =	new_vector(
			2.0f * (m.x * m.z - m.w * m.y),
			2.0f * (m.y * m.z + m.w * m.x),
			1.0f - 2.0f * (m.x * m.x + m.y * m.y));
	u =	new_vector(
			2.0f * (m.x * m.y + m.w * m.z),
			1.0f - 2.0f * (m.x * m.x + m.z * m.z),
			2.0f * (m.y * m.z - m.w * m.x));
	r =	new_vector(
			1.0f - 2.0f * (m.y * m.y + m.z * m.z),
			2.0f * (m.x * m.y - m.w * m.z),
			2.0f * (m.x * m.z + m.w * m.y));
	return (mat_rotation_fur(f, u, r));
};

void 	rotate(t_camera *camera, t_v3d axis, double angle)
{
	camera->rot =
			qt_normalized(
					qt_mul_qt(
							qt_axis_angle(axis, angle), camera->rot));
}

void 	move(t_camera *camera, t_v3d dir, double amount)
{
	camera->pos = vector_sum(camera->pos, vector_mul_by(dir, amount));
}

t_v3d 	vector_rotate_by_qt(t_v3d v, t_v3d qt)
{
	t_v3d conjugate = qt_conjugate(qt);
	t_v3d w = qt_mul_qt(qt_mul_v3d(qt, v), conjugate);
	return new_vector(w.x, w.y, w.z);
}

t_v3d	get_forward(t_v3d qt)
{
	return (vector_rotate_by_qt(new_vector(0.0, 0.0, 1.0), qt));
}

t_v3d	get_right(t_v3d qt)
{
	return (vector_rotate_by_qt(new_vector(1.0, 0.0, 0.0), qt));
}

void 	update_camera(t_camera *c)
{
	double	x_acos;

	c->rotation = qt_to_rot_mat(qt_conjugate(c->rot));
	c->translation = matrix_translation(-c->pos.x, -c->pos.y, -c->pos.z);
	c->view = matrix_multiply(c->rotation, c->translation);
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
}

void	process_inputs(t_app *app, double delta_time)
{
	t_mouse_state	*mouse;
	const uint8_t	*key;
	t_v3d			forward;
	t_v3d			right;
	t_camera		*camera;

	double			mouse_speed = 1.2123 * delta_time;

	camera = app->camera;
	key = app->inputs->keyboard;
	mouse = &app->inputs->mouse;
	if (mouse->x && mouse->y)
		mouse_speed = mouse_speed / 1.414;
	if (mouse->x)
		rotate(camera, new_vector(0.0, 1.0, 0.0), (double)mouse->x * mouse_speed);
	forward = get_forward(camera->rot);
	forward.y = 0.0;
	right = get_right(camera->rot);
	app->camera->pos_prev = app->camera->pos;
//	if (mouse->y)
//		rotate(camera, right, (double)mouse->y * mouse_speed);
	if (key[SDL_SCANCODE_W])
		move(camera, forward, 4.54321 * delta_time);
	if (key[SDL_SCANCODE_S])
		move(camera, forward, -4.54321 * delta_time);
	if (key[SDL_SCANCODE_A])
		move(camera, right, -4.54321 * delta_time);
	if (key[SDL_SCANCODE_D])
		move(camera, right, 4.54321 * delta_time);
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