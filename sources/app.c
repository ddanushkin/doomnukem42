/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   app.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lglover <lglover@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/11 13:02:45 by lglover           #+#    #+#             */
/*   Updated: 2020/03/11 13:02:54 by lglover          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"

void	app_reset_floor_ceil_hit(t_app *app)
{
	app->floor_dist = 10000.0;
	app->ceil_dist = 10000.0;
	app->floor_sector = NULL;
	app->ceil_sector = NULL;
}

void	app_close(t_app *app)
{
	app->points_count = 0;
	exit(0);
}
