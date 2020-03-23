/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   live_mode_add.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lglover <lglover@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/23 17:27:39 by lglover           #+#    #+#             */
/*   Updated: 2020/03/23 17:27:39 by lglover          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"

void	live_mode_add_decor(t_app *app, int healer)
{
	t_wall	*h;

	h = decor_add(app->hit_point,
	app->hit_sector,
	app->hit_wall,
	app->camera);
	if (app->hit_sector->decor_count != 25)
		app->hit_sector->decor_count++;
	app->hit_sector->decor_next = (app->hit_sector->decor_next + 1) % 25;
	h->healer = healer;
	if (healer)
	{
		h->sprite = HEALER_SPRITE;
		h->healer_cap = 100;
	}
}

void	live_mode_add_obj(t_app *app)
{
	t_wall	obj;

	obj = wall_new();
	obj.pos = app->floor_point;
	obj.size = 1.0;
	obj.sprite = OBJ_SPRITE;
	obj.rotate = 0;
	obj.shade = 0;
	app->floor_sector->obj[app->floor_sector->objs_count] = obj;
	app->floor_sector->objs_count++;
	app->floor_sector->objs_count %= MAX_OBJ;
}

void	live_mode_add_card(t_app *app)
{
	t_wall	obj;

	if (app->md.card_set && app->hit_sector->door)
	{
		app->hit_sector->need_card = 1;
		return ;
	}
	obj = wall_new();
	obj.pos = app->floor_point;
	obj.size = 0.5;
	obj.sprite = HUD_CARD_SPRITE_P;
	obj.is_card = 1;
	obj.collect = 1;
	obj.rotate = 0;
	obj.shade = 0;
	app->card_w = obj;
	app->md.card_set = 1;
	app->md.card_sector = app->hit_sector->id;
	app->md.card_pos = obj.pos;
}

void	live_mode_add_npc(t_app *app)
{
	t_wall	obj;

	obj = wall_new();
	obj.pos = app->floor_point;
	obj.size = 1.0;
	obj.sprite = 482;
	obj.rotate = 1;
	obj.shade = 0;
	app->floor_sector->npc[app->floor_sector->npcs_count] = obj;
	app->floor_sector->npcs_count++;
	app->floor_sector->npcs_count %= MAX_NPC;
}
