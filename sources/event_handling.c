/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   event_handling.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lglover <lglover@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/11 13:03:29 by lglover           #+#    #+#             */
/*   Updated: 2020/03/11 15:30:08 by lglover          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"

void	update_camera(t_app *app, t_camera *c)
{
	get_speed(app);
	c->view = view_matrix(c->pos, c->rot.x, c->rot.y);
	c->view_projection = matrix_multiply(c->projection, c->view);
	c->transform = get_transform_matrix(c->view_projection);
	c->dir.x = c->view.m[8];
	c->dir.y = c->view.m[9];
	c->dir.z = c->view.m[10];
	get_quad(c);
	if (acos(c->dir.z) > 1.570796 && c->quad != 0 && c->quad != 4)
		c->quad = 8 - c->quad;
	c->right = new_vector(c->view.m[0], c->view.m[1], c->view.m[2]);
	if (!c->fly)
		c->forward = new_vector(sin(c->rot.y), 0, cos(c->rot.y));
	else
		c->forward = new_vector(c->view.m[8], c->view.m[9], c->view.m[10]);
}

void	cursor_clamp(t_app *app)
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
				ray_floor_ceil(app, new_pos, s->ftrs[j]);
			if (s->ceil.active)
				ray_floor_ceil(app, new_pos, s->ctrs[j]);
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
	double			mouse_speed;

	mouse_speed = MOUSE_SPEED * dt;
	c = app->camera;
	key = app->inputs->keyboard;
	mouse = &app->inputs->mouse;
	view_rotation(mouse, c, mouse_speed);
	player_movement(app, c, key, dt);
	if (app->keys[SDL_SCANCODE_SPACE] && !app->jumped &&
		(app->ground || (app->falling > 0.0 && app->falling < 0.25)))
		player_jump(app);
	if (!c->fly)
		floor_collision(app, c, dt);
	floor_is_lava(app, c, dt);
	if (app->camera->fly)
		player_fly(app, c);
	player_crouch(app, c, key, dt);
}
