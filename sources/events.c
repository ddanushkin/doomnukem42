/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   events.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lglover <lglover@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/23 17:26:58 by lglover           #+#    #+#             */
/*   Updated: 2020/03/23 17:26:58 by lglover          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"

void	event_mouse(t_app *app, SDL_Event e, uint32_t event)
{
	if (event == SDL_MOUSEMOTION)
	{
		app->inputs->mouse.x = e.motion.xrel;
		app->inputs->mouse.y = e.motion.yrel;
	}
	if (event == SDL_MOUSEWHEEL)
	{
		if (e.wheel.y > 0)
			app->mouse[SDL_MOUSE_SCROLL_UP] = 1;
		else if (e.wheel.y < 0)
			app->mouse[SDL_MOUSE_SCROLL_DOWN] = 1;
	}
	if (event == SDL_MOUSEBUTTONDOWN)
		app->mouse[e.button.button] = 1;
}

int		event_handling(t_app *app)
{
	SDL_Event	e;
	uint32_t	event;

	app->inputs->mouse.x = 0;
	app->inputs->mouse.y = 0;
	while (SDL_PollEvent(&e))
	{
		event = e.type;
		if (event == SDL_QUIT || app->inputs->keyboard[SDL_SCANCODE_ESCAPE])
			return (0);
		event_mouse(app, e, event);
		if (event == SDL_KEYDOWN)
			app->keys[e.key.keysym.scancode] = 1;
	}
	if (app->hp <= 0)
	{
		state_reset(app);
		if (app->md.start_set)
			app->camera->pos = app->md.start_pos;
		else
			app->camera->fly = 1;
	}
	return (1);
}
