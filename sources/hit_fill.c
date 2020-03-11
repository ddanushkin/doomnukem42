/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hit_fill.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lglover <lglover@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/11 14:33:03 by lglover           #+#    #+#             */
/*   Updated: 2020/03/11 14:34:43 by lglover          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"

int		hit_fill_data(t_app *app, double distance)
{
	app->hit_point = v3d_sum(
			app->camera->pos,
			v3d_mul_by(app->camera->dir, distance));
	app->hit_dist = distance;
	app->hit_wall = app->rw;
	app->hit_sector = app->cs;
	app->hit_type = app->render_type;
	return (1);
}

int		hit_fill_floor(t_app *app, t_v3d pos, t_v3d dir, double distance)
{
	app->floor_point = v3d_sum(pos, v3d_mul_by(dir, distance));
	app->floor_dist = distance;
	app->floor_sector = app->cs;
	return (1);
}

int		hit_fill_ceil(t_app *app, t_v3d pos, t_v3d dir, double distance)
{
	app->ceil_point = v3d_sum(pos, v3d_mul_by(dir, distance));
	app->ceil_dist = distance;
	app->ceil_sector = app->cs;
	return (1);
}
