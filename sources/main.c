/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lglover <lglover@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/11 13:03:36 by lglover           #+#    #+#             */
/*   Updated: 2020/03/11 17:07:44 by lglover          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"

void	main(int argc, char **argv)
{
	t_app	*app;

	app = (t_app *)malloc(sizeof(t_app));
	app->sdl = (t_sdl *)malloc(sizeof(t_sdl));
	app->inputs = (t_inputs *)malloc(sizeof(t_inputs));
	app->timer = (t_timer *)malloc(sizeof(t_timer));
	app->camera = (t_camera *)malloc(sizeof(t_camera));
	app->depth_buffer = (double *)malloc(sizeof(double) * SCREEN_W * SCREEN_H);
	app_load_game_data(app);
	app_is_editor_mode(app, argc, argv);
	app_check_map(app, argc, argv);
	app_init(app);
	if (app->editor)
	{
		editor_start(app);
		map_save(app, app->map_path);
	}
	else
		game_start(app);
	app_exit(app);
}
