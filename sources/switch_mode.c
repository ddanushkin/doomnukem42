/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   switch_mode.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lglover <lglover@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/11 15:55:28 by lglover           #+#    #+#             */
/*   Updated: 2020/03/11 16:53:17 by lglover          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"

void	camera_point_mode(t_v3d *pos, t_v3d *rot)
{
	pos->y = 10.0;
	rot->x = 1.57;
	rot->y = 0.0;
	rot->z = 0.0;
}

void	camera_live_mode(t_v3d *rot)
{
	rot->x = 0.0;
	rot->y = 0.0;
	rot->z = 0.0;
}

void	set_start_pos(t_app *app)
{
	if (app->md.start_set)
	{
		app->camera->pos = app->md.start_pos;
		app->camera->fly = 0;
	}
}

int		switch_mode(t_app *app)
{
	double	temp_y;

	Mix_HaltMusic();
	app->point_mode = !app->point_mode;
	if (app->point_mode)
		camera_point_mode(&app->camera->pos, &app->camera->rot);
	else
	{
		Mix_PlayMusic(app->bg[app->md.music_id], -1);
		temp_y = app->camera->pos.y;
		app->camera->pos = point_2d_to_3d(app, app->cursor_x, app->cursor_y, 0);
		app->camera->pos.y = temp_y;
		app_reset_floor_ceil_hit(app);
		update_floor_dist(app, app->camera->pos);
		if (app->floor_sector != NULL)
		{
			app->camera->pos.y = app->floor_point.y + PLAYER_HEIGHT;
			camera_live_mode(&app->camera->rot);
		}
		else
			app->camera->fly = 1;
		set_start_pos(app);
	}
	return (1);
}
