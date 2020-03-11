/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fpc_counter.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lglover <lglover@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/11 13:03:30 by lglover           #+#    #+#             */
/*   Updated: 2020/03/11 13:04:02 by lglover          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"

void	update_fps_text(t_app *app)
{
	char	fps_text[10];

	fps_text[0] = '\0';
	app->timer->fps_sum += (uint64_t)(1.0 / app->timer->delta);
	app->timer->fps_count++;
	app->timer->fps = app->timer->fps_sum / app->timer->fps_count;
	if (app->timer->fps_count >= 120)
	{
		app->timer->fps_count = 0;
		app->timer->fps_sum = 0;
	}
	ft_itoa2(app->timer->fps, fps_text);
	font_set(app, 0, 0xffffff);
	print_to_screen(app, 0, 0, fps_text);
	font_reset(app);
}

void	get_delta_time(t_timer *timer)
{
	uint32_t current;

	timer->frame++;
	current = SDL_GetTicks();
	timer->delta = (double)(current - timer->prev) * 0.001;
	timer->prev = current;
	timer->time += timer->delta;
}
