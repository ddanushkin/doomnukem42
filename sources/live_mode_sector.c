/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   live_mode_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lglover <lglover@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/11 16:06:16 by lglover           #+#    #+#             */
/*   Updated: 2020/03/11 16:07:56 by lglover          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"

void	live_mode_change_floor_h(t_app *app)
{
	t_sector *s;

	s = app->hit_sector;
	if (app->keys[SDL_SCANCODE_MINUS])
	{
		if (fabs(s->ceil_y - (s->floor_y - 0.5)) <= 0.0)
			return ;
		sector_pts_h(&s->fpts[0], s->pts_count, -0.5);
		s->floor_y -= 0.5;
	}
	else if (app->keys[SDL_SCANCODE_EQUALS])
	{
		if (fabs(s->ceil_y - (s->floor_y + 0.5)) <= 0.0)
			return ;
		sector_pts_h(&s->fpts[0], s->pts_count, 0.5);
		s->floor_y += 0.5;
	}
	if (app->keys[SDL_SCANCODE_MINUS] || app->keys[SDL_SCANCODE_EQUALS])
	{
		sector_update_height(s, &s->fpts[0], &s->cpts[0]);
		s->delta_y = fabs(s->ceil_y - s->floor_y);
	}
}

void	live_mode_change_ceil_h(t_app *app)
{
	t_sector *s;

	s = app->hit_sector;
	if (app->keys[SDL_SCANCODE_MINUS])
	{
		if (fabs((s->ceil_y - 0.5) - s->floor_y) <= 0.0)
			return ;
		sector_pts_h(&s->cpts[0], s->pts_count, -0.5);
		s->ceil_y -= 0.5;
	}
	else if (app->keys[SDL_SCANCODE_EQUALS])
	{
		if (fabs((s->ceil_y + 0.5) - s->floor_y) <= 0.0)
			return ;
		sector_pts_h(&s->cpts[0], s->pts_count, 0.5);
		s->ceil_y += 0.5;
	}
	if (app->keys[SDL_SCANCODE_MINUS] || app->keys[SDL_SCANCODE_EQUALS])
	{
		sector_update_height(s, &s->fpts[0], &s->cpts[0]);
		s->delta_y = fabs(s->ceil_y - s->floor_y);
	}
}
