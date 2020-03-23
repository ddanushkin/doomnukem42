/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   live_mode_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lglover <lglover@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/23 17:27:50 by lglover           #+#    #+#             */
/*   Updated: 2020/03/23 17:27:50 by lglover          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"

void	live_mode_change_shade(t_app *app)
{
	if (app->keys[SDL_SCANCODE_MINUS])
		app->hit_wall->shade++;
	else if (app->keys[SDL_SCANCODE_EQUALS])
		app->hit_wall->shade--;
	if (app->hit_wall->shade < 0)
		app->hit_wall->shade = 255;
	if (app->hit_wall->shade > 255)
		app->hit_wall->shade = 0;
}

void	live_mode_use_wall(t_app *app)
{
	if (app->hit_type == decor &&
		app->hit_wall->is_exit)
		exit(0);
	if (app->hit_wall->is_card)
	{
		app->md.card_picked = 1;
		Mix_PlayChannel(9, app->sfx[89], 0);
	}
	if (app->hit_type == decor && app->hit_wall->healer)
	{
		if (app->hit_wall->healer_cap <= 0)
			Mix_PlayChannel(9, app->sfx[39], 0);
		else if (app->heal_tick > 0)
			app->heal_tick -= app->timer->delta;
		else if (app->hit_wall->healer_cap > 0 && app->heal_tick <= 0.0)
		{
			Mix_PlayChannel(9, app->sfx[9], 0);
			app->hp++;
			app->hit_wall->healer_cap--;
			if (app->hit_wall->healer_cap % 20 == 0)
				app->hit_wall->sprite--;
			app->heal_tick = HEAL_TIMER;
		}
	}
}

void	live_mode_door_open(t_app *app)
{
	if (!app->hit_sector || !app->hit_sector->door || app->hit_dist > USE_DIST)
		return ;
	if (app->hit_sector->need_card && !app->md.card_picked)
	{
		Mix_PlayChannel(9, app->sfx[39], 0);
		return ;
	}
	if (app->hit_sector->need_card)
	{
		Mix_PlayChannel(9, app->sfx[100], 0);
		app->hit_sector->need_card = 0;
		app->md.card_picked = 0;
		app->md.card_set = 0;
	}
	if (app->hit_sector->door_anim)
	{
		app->hit_sector->door_dir *= -1;
		Mix_PlayChannel(1, app->sfx[35], -1);
	}
	app->hit_sector->door_anim = 1;
	Mix_PlayChannel(2, app->sfx[26], 0);
	Mix_PlayChannel(1, app->sfx[35], -1);
}

void	live_mode_sector_io(t_app *app)
{
	app->hit_sector->inside++;
	if (app->hit_sector->inside == 3)
		app->hit_sector->inside = 0;
}

void	live_mode_rotate_npc(t_app *app)
{
	if (app->keys[SDL_SCANCODE_PAGEUP])
		app->hit_wall->ori++;
	if (app->keys[SDL_SCANCODE_PAGEDOWN])
		app->hit_wall->ori--;
	if (app->hit_wall->ori > 7)
		app->hit_wall->ori = 0;
	if (app->hit_wall->ori < 0)
		app->hit_wall->ori = 7;
}
