/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ui_draw.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lglover <lglover@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/11 13:03:49 by lglover           #+#    #+#             */
/*   Updated: 2020/03/11 17:17:14 by lglover          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"

void	pts(t_app *app, int x, int y, char *text)
{
	SDL_Color	*color;
	SDL_Surface	*surface;
	SDL_Rect	rect;

	color = (SDL_Color *)(&app->font_color);
	surface = TTF_RenderText_Solid(app->font, text, *color);
	rect.x = x;
	rect.y = y;
	rect.w = surface->w;
	rect.h = surface->h;
	SDL_BlitSurface(surface, &surface->clip_rect, app->sdl->surface, &rect);
	SDL_FreeSurface(surface);
}

void	draw_cross(t_app *app, int x, int y, double size)
{
	int	len;
	int	i;
	int start_x;
	int start_y;

	if (size <= 0)
		return ;
	i = 0;
	len = (int)(64.0 / size);
	len = CLAMP(len, 2, 32);
	start_x = x - (int)(len * 0.5);
	start_y = y - (int)(len * 0.5);
	while (i <= len)
	{
		pixel_set(app->sdl->surface, (start_y + i) * SCREEN_W + x, 0xffffff);
		pixel_set(app->sdl->surface, y * SCREEN_W + (start_x + i), 0xffffff);
		i++;
	}
}
