/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lglover <lglover@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/11 13:03:36 by lglover           #+#    #+#             */
/*   Updated: 2020/03/11 17:07:44 by lglover          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"

void	game_inputs(t_app *app)
{
	if (app->keys[SDL_SCANCODE_E] && app->hit_dist <= USE_DIST)
		live_mode_use_wall(app);
	if (app->keys[SDL_SCANCODE_E])
		live_mode_door_open(app);
}

void	game_start(t_app *app)
{
	state_reset(app);
	prepare_chunks(app);
	SDL_SetRelativeMouseMode(SDL_TRUE);
	Mix_PlayMusic(app->bg[app->md.music_id], -1);
	app->camera->pos = app->md.start_pos;
	while (1)
	{
		get_delta_time(app->timer);
		ft_bzero(&app->keys, sizeof(uint8_t) * 512);
		ft_bzero(&app->mouse, sizeof(uint8_t) * 6);
		if (!event_handling(app))
			break;
		game_inputs(app);
		update_camera(app, app->camera);
		process_inputs(app, app->timer->delta);
		render_map(app);
		draw_hud(app);
		update_fps_text(app);
		draw_action_text(app);
		draw_cross(app, SCREEN_W / 2, SCREEN_H / 2, 8);
		SDL_UpdateWindowSurface(app->sdl->window);
		reset_screen(app);
	}
}