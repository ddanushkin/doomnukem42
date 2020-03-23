/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   live_mode_wall.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lglover <lglover@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/23 17:27:52 by lglover           #+#    #+#             */
/*   Updated: 2020/03/23 17:27:52 by lglover          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"

void	wall_offset_x(t_app *app, t_wall *w)
{
	if (app->inputs->keyboard[SDL_SCANCODE_RIGHT])
	{
		w->v[0].tex_x -= 0.001;
		w->v[1].tex_x -= 0.001;
		w->v[2].tex_x -= 0.001;
		w->v[3].tex_x -= 0.001;
	}
	if (app->inputs->keyboard[SDL_SCANCODE_LEFT])
	{
		w->v[0].tex_x += 0.001;
		w->v[1].tex_x += 0.001;
		w->v[2].tex_x += 0.001;
		w->v[3].tex_x += 0.001;
	}
}

void	wall_offset_y(t_app *app, t_wall *w)
{
	if (app->inputs->keyboard[SDL_SCANCODE_UP])
	{
		w->v[0].tex_y -= 0.001;
		w->v[1].tex_y -= 0.001;
		w->v[2].tex_y -= 0.001;
		w->v[3].tex_y -= 0.001;
	}
	if (app->inputs->keyboard[SDL_SCANCODE_DOWN])
	{
		w->v[0].tex_y += 0.001;
		w->v[1].tex_y += 0.001;
		w->v[2].tex_y += 0.001;
		w->v[3].tex_y += 0.001;
	}
}

void	live_mode_wall_offset(t_app *app)
{
	wall_offset_x(app, app->hit_wall);
	wall_offset_y(app, app->hit_wall);
}

void	live_mode_wall_bot(t_app *app)
{
	t_sector	*s;
	t_wall		*w;

	s = app->hit_sector;
	w = app->hit_wall;
	if (app->keys[SDL_SCANCODE_MINUS])
	{
		s->fpts[w->v[0].i].y -= 0.5;
		s->fpts[w->v[2].i].y -= 0.5;
	}
	else if (app->keys[SDL_SCANCODE_EQUALS])
	{
		s->fpts[w->v[0].i].y += 0.5;
		s->fpts[w->v[2].i].y += 0.5;
	}
	if (app->keys[SDL_SCANCODE_MINUS] || app->keys[SDL_SCANCODE_EQUALS])
		sector_update_height(s, &s->fpts[0], &s->cpts[0]);
}

void	live_mode_wall_top(t_app *app)
{
	t_sector	*s;
	t_wall		*w;

	s = app->hit_sector;
	w = app->hit_wall;
	if (app->keys[SDL_SCANCODE_MINUS])
	{
		s->cpts[w->v[3].i].y -= 0.5;
		s->cpts[w->v[1].i].y -= 0.5;
	}
	else if (app->keys[SDL_SCANCODE_EQUALS])
	{
		s->cpts[w->v[3].i].y += 0.5;
		s->cpts[w->v[1].i].y += 0.5;
	}
	if (app->keys[SDL_SCANCODE_MINUS] || app->keys[SDL_SCANCODE_EQUALS])
		sector_update_height(s, &s->fpts[0], &s->cpts[0]);
}
