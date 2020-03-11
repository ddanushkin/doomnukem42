/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lglover <lglover@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/11 15:20:31 by lglover           #+#    #+#             */
/*   Updated: 2020/03/11 15:26:53 by lglover          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"

void	move(t_v3d *v, t_v3d dir, double amount)
{
	*v = v3d_sum(*v, v3d_mul_by(dir, amount));
}

void	get_speed(t_app *app)
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

void	update_points_camera(t_camera *c)
{
	c->view = view_matrix(c->pos, c->rot.x, c->rot.y);
	c->view_projection = matrix_multiply(c->projection, c->view);
	c->transform = get_transform_matrix(c->view_projection);
	c->dir.x = c->view.m[8];
	c->dir.y = c->view.m[9];
	c->dir.z = c->view.m[10];
}

void	floor_is_lava(t_app *app, t_camera *c, double dt)
{
	if (!c->fly && app->floor_sector &&
		app->floor_sector->lava && app->floor_dist <= app->height &&
		app->floor_point.y < app->floor_sector->ceil_y)
	{
		app->lava_timer -= dt;
		if (app->lava_timer <= 0.0)
		{
			app->hp -= 20;
			Mix_PlayChannel(5, app->sfx[36], 0);
			Mix_PlayChannel(6, app->sfx[94], 0);
			app->y_vel = 6.0;
			app->jumped = 1;
			app->ground = 0;
			app->falling = 0.0;
			app->lava_timer = LAVA_TIMER;
		}
	}
	else
		app->lava_timer = 0.0;
}

void	get_quad(t_camera *c)
{
	double	x_acos;

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
}
