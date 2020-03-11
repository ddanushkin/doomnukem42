/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   live_mode_set.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lglover <lglover@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/11 16:03:02 by lglover           #+#    #+#             */
/*   Updated: 2020/03/11 16:04:31 by lglover          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"

void	live_mode_set_exit(t_app *app)
{
	if (app->hit_type == decor)
		app->hit_wall->is_exit = !app->hit_wall->is_exit;
}

void	live_mode_set_start(t_app *app)
{
	if (!app->camera->fly && app->floor_sector)
	{
		app->md.start_pos = app->camera->pos;
		app->md.start_set = 1;
	}
}

void	live_mode_set_bg(t_app *app)
{
	int		prev;

	prev = app->md.music_id;
	if (app->keys[SDL_SCANCODE_0])
		app->md.music_id = 0;
	else if (app->keys[SDL_SCANCODE_1])
		app->md.music_id = 1;
	else if (app->keys[SDL_SCANCODE_2])
		app->md.music_id = 2;
	else if (app->keys[SDL_SCANCODE_3])
		app->md.music_id = 3;
	else if (app->keys[SDL_SCANCODE_4])
		app->md.music_id = 4;
	else if (app->keys[SDL_SCANCODE_5])
		app->md.music_id = 5;
	else if (app->keys[SDL_SCANCODE_6])
		app->md.music_id = 6;
	else if (app->keys[SDL_SCANCODE_7])
		app->md.music_id = 7;
	else if (app->keys[SDL_SCANCODE_8])
		app->md.music_id = 8;
	else if (app->keys[SDL_SCANCODE_9])
		app->md.music_id = 9;
	if (prev != app->md.music_id)
		Mix_PlayMusic(app->bg[app->md.music_id], -1);
}
