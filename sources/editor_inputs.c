/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor_inputs.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lglover <lglover@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/23 17:26:49 by lglover           #+#    #+#             */
/*   Updated: 2020/03/23 17:26:49 by lglover          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"

void	texture_change(t_app *app)
{
	if (app->keys[SDL_SCANCODE_MINUS])
		app->hit_wall->sprite--;
	else if (app->keys[SDL_SCANCODE_EQUALS])
		app->hit_wall->sprite++;
	app->hit_wall->sprite =
			(app->hit_wall->sprite % LAST_TEX + LAST_TEX) % LAST_TEX;
	if (app->hit_wall->sprite < FIRST_TEX)
		app->hit_wall->sprite = FIRST_TEX;
}

void	texture_scale_y_change(t_app *app)
{
	if (app->keys[SDL_SCANCODE_MINUS] && app->hit_wall->sy < 10.0)
		app->hit_wall->sy *= 2;
	else if (app->keys[SDL_SCANCODE_EQUALS] && app->hit_wall->sy > 0.25)
		app->hit_wall->sy /= 2;
}

void	texture_scale_x_change(t_app *app)
{
	if (app->keys[SDL_SCANCODE_MINUS] && app->hit_wall->sx < 10.0)
		app->hit_wall->sx *= 2;
	else if (app->keys[SDL_SCANCODE_EQUALS] && app->hit_wall->sx > 0.25)
		app->hit_wall->sx /= 2;
}
