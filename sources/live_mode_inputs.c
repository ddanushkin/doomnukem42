/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   live_mode_inputs.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lglover <lglover@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/23 17:27:41 by lglover           #+#    #+#             */
/*   Updated: 2020/03/23 17:27:41 by lglover          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"

void	live_mode_inputs_0(t_app *app)
{
	if (app->inputs->keyboard[SDL_SCANCODE_LSHIFT])
		texture_change(app);
	else if (app->inputs->keyboard[SDL_SCANCODE_LALT])
		texture_scale_y_change(app);
	else if (app->inputs->keyboard[SDL_SCANCODE_LCTRL])
		texture_scale_x_change(app);
	else if (app->keys[SDL_SCANCODE_DELETE])
		app->hit_wall->active = 0;
	else if (app->keys[SDL_SCANCODE_TAB] &&
	(app->hit_type == wall || app->hit_type == floor_ceil))
		app->hit_wall->flip = !app->hit_wall->flip;
	else if (app->inputs->keyboard[SDL_SCANCODE_F])
		live_mode_change_floor_h(app);
	else if (app->inputs->keyboard[SDL_SCANCODE_C])
		live_mode_change_ceil_h(app);
	else if (app->inputs->keyboard[SDL_SCANCODE_L])
		live_mode_change_shade(app);
	else if (app->inputs->keyboard[SDL_SCANCODE_T] && app->hit_type == wall)
		live_mode_add_decor(app, 0);
	else if (app->inputs->keyboard[SDL_SCANCODE_H] && app->hit_type == wall)
		live_mode_add_decor(app, 1);
}

void	live_mode_inputs_1(t_app *app)
{
	if (app->inputs->keyboard[SDL_SCANCODE_Q] && app->hit_type == wall)
		live_mode_wall_top(app);
	if (app->inputs->keyboard[SDL_SCANCODE_E] && app->hit_type == wall)
		live_mode_wall_bot(app);
	if (app->keys[SDL_SCANCODE_I])
		live_mode_sector_io(app);
	if (app->inputs->keyboard[SDL_SCANCODE_O])
		live_mode_wall_offset(app);
	if (app->hit_wall && app->keys[SDL_SCANCODE_F1])
		live_mode_set_exit(app);
	if (app->hit_sector && app->keys[SDL_SCANCODE_F2])
		live_mode_toggle_door(app);
	if (app->hit_sector && app->keys[SDL_SCANCODE_F3])
		live_mode_toggle_lava(app);
	if (app->keys[SDL_SCANCODE_F7] && app->hit_type == decor)
		live_mode_toggle_healer(app);
	if (app->keys[SDL_SCANCODE_E] && app->hit_dist <= USE_DIST)
		live_mode_use_wall(app);
	if (app->hit_type == npc && app->inputs->keyboard[SDL_SCANCODE_E])
	    live_mode_npc_speak(app);
	else if (app->keys[SDL_SCANCODE_E])
		live_mode_door_open(app);
	if (app->inputs->keyboard[SDL_SCANCODE_M])
		live_mode_set_bg(app);
	if (app->hit_type == npc && app->inputs->keyboard[SDL_SCANCODE_RSHIFT])
		live_mode_rotate_npc(app);
}

void	live_mode_inputs_2(t_app *app)
{
	if (app->keys[SDL_SCANCODE_F])
		app->camera->fly = !app->camera->fly;
	if (app->hit_sector && app->floor_sector)
	{
		if (app->keys[SDL_SCANCODE_N])
			live_mode_add_npc(app);
		if (app->keys[SDL_SCANCODE_O])
			live_mode_add_obj(app);
		if (app->keys[SDL_SCANCODE_F5])
			live_mode_set_start(app);
		if (app->keys[SDL_SCANCODE_F6])
			live_mode_add_card(app);
	}
}

void	live_mode_inputs(t_app *app)
{
	if (app->hit_wall)
	{
		live_mode_inputs_0(app);
		live_mode_inputs_1(app);
	}
	live_mode_inputs_2(app);
}
