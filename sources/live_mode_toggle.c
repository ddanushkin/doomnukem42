/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   live_mode_toggle.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lglover <lglover@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/11 16:00:29 by lglover           #+#    #+#             */
/*   Updated: 2020/03/11 16:04:31 by lglover          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"

void	live_mode_toggle_lava(t_app *app)
{
	int r;

	app->hit_sector->lava = !app->hit_sector->lava;
	r = app->hit_sector->lava;
	app->hit_sector->floor.sprite = r ? LAVA_SPRITE : FLOOR_SPRITE;
}

void	live_mode_toggle_door(t_app *app)
{
	int			i;
	t_sector	*s;

	s = app->hit_sector;
	s->door = !s->door;
	s->need_card = 0;
	i = 0;
	while (i < s->walls_count)
	{
		s->walls[i].sprite = s->door ? DOOR_SIDE : WALL_SPRITE;
		i++;
	}
	s->floor.sprite = s->door ? DOOR_TOP_BOT : FLOOR_SPRITE;
	s->ceil.sprite = s->door ? DOOR_TOP_BOT : CEIL_SPRITE;
	s->door_anim = 0;
}

void	live_mode_toggle_healer(t_app *app)
{
	if (app->hit_type == decor)
		app->hit_wall->healer = !app->hit_wall->healer;
	if (app->hit_wall->healer)
	{
		app->hit_wall->sprite = HEALER_SPRITE;
		app->hit_wall->healer_cap = 100;
	}
	else
		app->hit_wall->sprite = DECOR_SPRITE;
}
