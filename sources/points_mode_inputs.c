/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   points_mode_inputs.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lglover <lglover@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/11 15:56:30 by lglover           #+#    #+#             */
/*   Updated: 2020/03/11 15:57:59 by lglover          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"

void	point_mode_switch_grid(t_app *app)
{
	app->grid_size = app->grid_size == 2.0 ? 0.5 : 2.0;
}

int		point_exist(t_v3d *points, int size, t_v3d point)
{
	int		i;
	t_v3d	tmp;

	i = 0;
	while (i < size)
	{
		tmp = points[i];
		if (tmp.x == point.x && tmp.z == point.z)
			return (1);
		i++;
	}
	return (0);
}

void	points_mode_reset_camera_pos(t_v3d *pos)
{
	pos->x = 0.0;
	pos->z = 0.0;
}

void	point_mode_mouse(t_app *app)
{
	t_v3d	new_point;

	if (app->mouse[SDL_MOUSE_LEFT])
	{
		new_point = point_2d_to_3d(app, app->cursor_x, app->cursor_y, 1);
		if (!point_exist(&app->points[0], app->points_count, new_point))
		{
			app->points[app->points_count] = new_point;
			app->points_count++;
		}
	}
	else if (app->mouse[SDL_MOUSE_RIGHT] && app->points_count > 0)
		app->points_count--;
}

void	point_mode_inputs(t_app *app)
{
	if (app->keys[SDL_SCANCODE_G])
		point_mode_switch_grid(app);
	if (app->mouse[SDL_MOUSE_LEFT] || app->mouse[SDL_MOUSE_RIGHT])
		point_mode_mouse(app);
	if (app->keys[SDL_SCANCODE_R])
		points_mode_reset_camera_pos(&app->camera->pos);
	if (app->keys[SDL_SCANCODE_Q] && app->points_count >= 3)
		sector_close(app, &app->sectors[app->sectors_count]);
}
