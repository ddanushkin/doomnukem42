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

	mat.m[0][0] = r.x;
	mat.m[0][1] = r.y;
	mat.m[0][2] = r.z;
	mat.m[0][3] = 0;
	mat.m[1][0] = u.x;
	mat.m[1][1] = u.y;
	mat.m[1][2] = u.z;
	mat.m[1][3] = 0;
	mat.m[2][0] = f.x;
	mat.m[2][1] = f.y;
	mat.m[2][2] = f.z;
	mat.m[2][3] = 0;
	mat.m[3][0] = 0;
	mat.m[3][1] = 0;
	mat.m[3][2] = 0;
	mat.m[3][3] = 1;
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

void 	update_camera(t_camera *camera)
{
	camera->rotation = qt_to_rot_mat(qt_conjugate(camera->rot));
	camera->translation = matrix_translation(-camera->pos.x, -camera->pos.y, -camera->pos.z);
	camera->view_projection = matrix_multiply(camera->projection, matrix_multiply(camera->rotation, camera->translation));
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
	if (mouse->x)
		rotate(camera, new_vector(0.0, 1.0, 0.0), mouse->x * mouse->sens * delta_time * 0.5);
	forward = get_forward(camera->rot);
	forward.y = 0.0;
	right = get_right(camera->rot);
	if (mouse->y)
		rotate(camera, right, mouse->y * mouse->sens * delta_time * 0.5);
	if (key[SDL_SCANCODE_W])
		move(camera, forward, 5.0 * delta_time);
	if (key[SDL_SCANCODE_S])
		move(camera, forward, -5.0 * delta_time);
	if (key[SDL_SCANCODE_A])
		move(camera, right, -5.0 * delta_time);
	if (key[SDL_SCANCODE_D])
		move(camera, right, 5.0 * delta_time);
}

int		event_handling(t_app *app)
{
	SDL_PollEvent(&app->sdl->event);
	if (app->sdl->event.type == SDL_QUIT)
		return(0);
	if (app->inputs->keyboard[SDL_SCANCODE_ESCAPE])
		return(0);
	process_inputs(app, app->timer->delta);
	update_camera(app->camera);
	return (1);
}