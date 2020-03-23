/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hud_image.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lglover <lglover@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/23 17:27:26 by lglover           #+#    #+#             */
/*   Updated: 2020/03/23 17:27:26 by lglover          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"

t_hud_data	hud_image_new(int x, int y, int w, int h)
{
	t_hud_data	d;

	d.x = x;
	d.y = y;
	d.w = w;
	d.h = h;
	d.y_rat = 256 / (double)d.h;
	d.x_rat = 256 / (double)d.w;
	return (d);
}

void		draw_hud_image(uint32_t *screen, t_hud_data d, uint32_t *img)
{
	int		img_y;
	int		img_x;
	int		x;
	int		y;

	x = 0;
	while (x < d.w)
	{
		y = 0;
		while (y < d.h)
		{
			img_y = (int)(y * d.y_rat);
			img_x = (int)(x * d.x_rat);
			screen[(y + d.y) * SCREEN_W + x + d.x] =
					img[img_y * 256 + img_x];
			y++;
		}
		x++;
	}
}

void		draw_hud_text(t_app *app, int data, int x, int y)
{
	char	text[5];

	text[0] = '\0';
	if (data > 9999)
		data = 9999;
	ft_itoa2(data, &text[0]);
	font_set(app, 0, 0xFFFFFF);
	pts(app, x, y, &text[0]);
	font_reset(app);
}

void		draw_hud(t_app *app)
{
	t_sprite	*s;
	uint32_t	*p;

	p = (uint32_t *)&app->sdl->surface->pixels;
	s = &app->sprites[0];
	if (app->md.card_picked)
		draw_hud_image(p, app->card_hud, &s[HUD_CARD_SPRITE_P].pixels[0]);
	else
		draw_hud_image(p, app->card_hud, &s[HUD_CARD_SPRITE_E].pixels[0]);
	draw_hud_image(p, app->hp_hud, &s[HUD_HP_SPRITE].pixels[0]);
	draw_hud_text(app, app->hp, 120, SCREEN_H - 155);
	draw_hud_image(p, app->time_hud, &s[HUD_TIME_SPRITE].pixels[0]);
	draw_hud_text(app,
			(int)app->timer->time, SCREEN_W / 2 - 10, SCREEN_H - 130);
}
