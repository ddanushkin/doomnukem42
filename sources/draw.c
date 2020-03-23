/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lglover <lglover@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/23 17:26:30 by lglover           #+#    #+#             */
/*   Updated: 2020/03/23 17:26:30 by lglover          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"

void	draw_line_3d(t_app *app, t_v3d start, t_v3d end, uint32_t c)
{
	t_v3d	tmp1;
	t_v3d	tmp2;

	tmp1 = start;
	tmp2 = end;
	tmp1.y = 0.0;
	tmp2.y = 0.0;
	tmp1 = matrix_transform(app->camera->transform, tmp1);
	tmp2 = matrix_transform(app->camera->transform, tmp2);
	if (vertex_inside(&tmp1) &&
		vertex_inside(&tmp2))
	{
		tmp1 = matrix_transform(app->camera->screen_space, tmp1);
		vertex_perspective_divide(&tmp1);
		tmp2 = matrix_transform(app->camera->screen_space, tmp2);
		vertex_perspective_divide(&tmp2);
		draw_line(app, &tmp1, &tmp2, c);
	}
}

void	draw_points_sector(t_app *app, t_v3d *p, int size)
{
	int		i;

	if (size == 1)
		point_draw(app, p[0], 0xff00ff);
	else
	{
		i = 0;
		while (i < size - 1)
		{
			draw_line_3d(app, p[i], p[i + 1], 0x5c5c5c);
			i++;
		}
		draw_line_3d(app, p[size - 1], p[0], 0x5c5c5c);
	}
}

void	draw_sectors(t_app *app)
{
	int			i;
	t_sector	*s;

	i = 0;
	while (i < app->sectors_count)
	{
		s = &app->sectors[i];
		draw_points_sector(app, &s->fpts[0], s->pts_count);
		i++;
	}
}

void	draw_points(t_app *app, t_v3d *p, int size)
{
	int		i;

	if (size == 1)
		point_draw(app, p[0], 0xff0000);
	else
	{
		i = 0;
		while (i < size - 1)
		{
			draw_line_3d(app, p[i], p[i + 1], 0xff00ff);
			i++;
		}
		if (app->bad_close >= 0.0)
		{
			draw_line_3d(app, p[size - 1], p[0], app->bclr[app->bflag]);
			app->bflag = !app->bflag;
			app->bad_close -= app->timer->delta;
		}
		else
		{
			draw_line_3d(app, p[size - 1], p[0], 0xffff00);
		}
	}
}

void	draw_action_text(t_app *app)
{
	if (app->hit_wall && app->hit_dist <= USE_DIST)
	{
		font_set(app, 0, 0x0000FF);
		if (app->hit_wall->is_card)
			pts(app, SCREEN_W / 2 + 15, SCREEN_H / 2 - 15, "PICK UP");
		else if (app->hit_sector->door && !app->hit_sector->door_anim)
			pts(app, SCREEN_W / 2 + 15, SCREEN_H / 2 - 15, "USE DOOR");
		else if (app->hit_wall->is_exit)
			pts(app, SCREEN_W / 2 + 15, SCREEN_H / 2 - 15, "FINISH LEVEL");
		else if (app->hit_wall->healer && app->hit_wall->healer_cap > 0)
			pts(app, SCREEN_W / 2 + 15, SCREEN_H / 2 - 15, "RESTORE HEALTH");
		else if (app->hit_wall->healer && app->hit_wall->healer_cap <= 0)
			pts(app, SCREEN_W / 2 + 15, SCREEN_H / 2 - 15, "OUT OF POWER");
		else if (app->hit_type == npc)
			pts(app, SCREEN_W / 2 + 15, SCREEN_H / 2 - 15, "SPEAK");
		font_reset(app);
	}
}
