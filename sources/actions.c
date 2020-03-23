/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lglover <lglover@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/23 17:26:12 by lglover           #+#    #+#             */
/*   Updated: 2020/03/23 17:26:12 by lglover          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"

void	view_rotation(t_mouse_state *mouse, t_camera *c, double ms)
{
	if (mouse->x && mouse->y)
		ms = ms / 1.414;
	if (mouse->x)
		c->rot.y += (double)mouse->x * ms;
	if (mouse->y)
		c->rot.x += (double)mouse->y * ms;
	if (!c->fly)
		c->rot.x = CLAMP(c->rot.x, -1.45, 1.45);
}

void	player_crouch(t_app *app, t_camera *c, const uint8_t *key, double dt)
{
	if (!app->hth)
		app->hth = app->ceil_sector && fabs(c->pos.y - app->ceil_point.y) < 0.1;
	if (app->hth && app->ceil_sector &&
		app->ceil_sector->door && app->ceil_sector->door_anim)
	{
		app->hp -= 50;
		Mix_PlayChannel(3, app->sfx[22], 0);
		Mix_PlayChannel(4, app->sfx[10], 0);
	}
	if (!app->camera->fly && app->y_vel > 0.0 && app->hth)
		app->y_vel *= -1;
	if ((key[SDL_SCANCODE_LCTRL] || app->hth) &&
		app->height > PLAYER_HEIGHT * 0.5)
		app->height -= 2.5 * dt;
	if (!key[SDL_SCANCODE_LCTRL] && app->height < PLAYER_HEIGHT)
		app->height += 2.5 * dt;
	app->height = CLAMP(app->height, PLAYER_HEIGHT * 0.5, PLAYER_HEIGHT);
}

void	player_fly(t_app *app, t_camera *c)
{
	app->y_vel = 0.0;
	app->y_acc = 0.0;
	c->pos = v3d_sum(c->pos, app->ld);
	app_reset_floor_ceil_hit(app);
	update_floor_dist(app, c->pos);
}

void	player_jump(t_app *app)
{
	Mix_PlayChannel(3, app->sfx[38], 0);
	app->y_vel = 6.0;
	app->jumped = 1;
	app->ground = 0;
	app->falling = 0.0;
}

void	player_movement(t_app *app, t_camera *c, const uint8_t *key, double dt)
{
	app->ld = new_vector(0.0, 0.0, 0.0);
	if (key[SDL_SCANCODE_W])
		app->ld = v3d_sum(app->ld, v3d_mul_by(c->forward, app->speed * dt));
	if (key[SDL_SCANCODE_S])
		app->ld = v3d_sum(app->ld, v3d_mul_by(c->forward, -app->speed * dt));
	if (key[SDL_SCANCODE_A])
		app->ld = v3d_sum(app->ld, v3d_mul_by(c->right, -app->speed * dt));
	if (key[SDL_SCANCODE_D])
		app->ld = v3d_sum(app->ld, v3d_mul_by(c->right, app->speed * dt));
}
