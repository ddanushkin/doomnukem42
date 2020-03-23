/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   app.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lglover <lglover@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/23 17:26:16 by lglover           #+#    #+#             */
/*   Updated: 2020/03/23 18:23:50 by lglover          ###   ########.fr       */
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

void	app_load_game_data(t_app *app)
{
	if (!gamedata_load(app))
		exit_with_status(STATUS_NO_GAME_DATA, NULL);
	if (!check_resources())
		exit_with_status(STATUS_BAD_RESOURCES, NULL);
}

void	app_is_editor_mode(t_app *app, int argc, char **argv)
{
	int		i;

	i = 1;
	app->editor = 0;
	while (i < argc)
	{
		if (ft_strequ(argv[i++], "-e\0"))
		{
			app->editor = 1;
			return ;
		}
	}
}

void	app_check_map(t_app *app, int argc, char **argv)
{
	int		i;

	i = 1;
	app->new_map = 1;
	app->map_path = NULL;
	while (i < argc)
	{
		if (argv[i][0] != '-')
		{
			app->new_map = !map_load(app, argv[i]);
			app->map_path = argv[i];
			break ;
		}
		i++;
	}
	if (!app->map_path)
		exit_with_status(STATUS_NO_MAP_PATH, NULL);
	else if (app->editor && app->new_map)
		init_map(app);
	else if (!app->editor && app->map_path && app->new_map)
		exit_with_status(STATUS_MAP_NOT_FOUND, app->map_path);
}

int		main(int argc, char *argv[])
{
	t_app	app;
	int		screen_w;
	int		screen_h;

	screen_w = SCREEN_W;
	screen_h = SCREEN_H;
	app.sdl = (t_sdl *)malloc(sizeof(t_sdl));
	app.inputs = (t_inputs *)malloc(sizeof(t_inputs));
	app.timer = (t_timer *)malloc(sizeof(t_timer));
	app.camera = (t_camera *)malloc(sizeof(t_camera));
	app.depth_buffer = (double *)malloc(sizeof(double) * screen_w * screen_h);
	app_load_game_data(&app);
	app_is_editor_mode(&app, argc, argv);
	app_check_map(&app, argc, argv);
	app_init(&app);
	if (app.editor)
	{
		editor_start(&app);
		map_save(&app, app.map_path);
	}
	else
		game_start(&app);
	app_exit(&app);
	return (0);
}
