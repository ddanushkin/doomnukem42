/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   floor_collision.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lglover <lglover@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/11 15:05:33 by lglover           #+#    #+#             */
/*   Updated: 2020/03/11 15:14:46 by lglover          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"

void	floor_ground(t_app *app)
{
	app->temp = 0;
	app->y_acc = 0.0;
	app->y_vel = 0.0;
	app->ground = 1;
	app->falling = 0.0;
	app->jumped = 0;
}

void	floor_under(t_app *app, t_camera *c, double dt)
{
	double	dy;

	dy = app->floor_point.y - app->prev_dy;
	if (dy != 0.0)
		app->temp += -dy;
	if (dy > 0.25 && dy <= 0.55 && app->y_vel == 0.0)
	{
		app->y_vel = dy * 8;
		app->y_acc = 12.0;
	}
	c->pos.y = app->floor_point.y + app->height + app->temp;
	if (app->temp != 0.0)
	{
		if (app->y_acc == 0.0)
			app->y_acc = 15.8;
		app->y_vel -= app->y_acc * dt;
		if (app->y_vel <= 0)
			app->y_acc += 15.8 * dt;
		if (app->y_vel < 0.0)
			app->falling += dt;
	}
}

void	floor_collision(t_app *app, t_camera *c, double dt)
{
	app_reset_floor_ceil_hit(app);
	update_floor_dist(app, c->pos);
	app->prev_dy = app->floor_point.y;
	if (app->y_vel != 0.0)
		app->temp += app->y_vel * dt;
	if (app->temp <= 0.0 && app->y_vel < 0.0)
	{
		if (app->falling > 0.25)
			Mix_PlayChannel(3, app->sfx[70], 0);
		if (app->falling > 0.55)
		{
			Mix_PlayChannel(4, app->sfx[51], 0);
			app->hp -= (int)(app->falling * 50);
		}
		floor_ground(app);
	}
	check_collision(app, &c->pos, app->ld);
	app_reset_floor_ceil_hit(app);
	update_floor_dist(app, c->pos);
	floor_under(app, c, dt);
}
