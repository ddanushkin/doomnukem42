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

void	editor_init(t_app *app)
{
	app->cursor_x = SCREEN_W * 0.5;
	app->cursor_y = SCREEN_H * 0.5;
	app->camera->pos.x = 0.0;
	app->camera->pos.z = 0.0;
	app->points_count = 0;
	app->point_mode = 0;
	app->bad_close = 0.0;
	app->bflag = 0;
	app->bclr[0] = 0xff0000;
	app->bclr[1] = 0x00ff00;
	switch_mode(app);
}

void	editor_point_mode(t_app *app)
{
	point_mode_inputs(app);
	update_points_camera(app->camera);
	process_points_inputs(app, app->timer->delta);
	draw_sectors(app);
	if (app->points_count > 3)
		points_add_check(&app->points[0], &app->points_count);
	if (app->points_count > 0)
		draw_points(app, &app->points[0], app->points_count);
	draw_point_mode(app);
	font_set(app, 0, 0x0000FF);
	draw_exit(app);
	draw_start(app);
	font_reset(app);
	draw_cross(app, (int)app->cursor_x, (int)app->cursor_y, 8);
}

void	editor_live_mode(t_app *app)
{
	live_mode_inputs(app);
	update_camera(app, app->camera);
	process_inputs(app, app->timer->delta);
	render_map(app);
	draw_hud(app);
	update_fps_text(app);
	draw_action_text(app);
	draw_cross(app, SCREEN_W / 2, SCREEN_H / 2, 8);
}

void	editor_start(t_app *app)
{
	state_reset(app);
	editor_init(app);
	prepare_chunks(app);
	SDL_SetRelativeMouseMode(SDL_TRUE);
	while (1)
	{
		get_delta_time(app->timer);
		ft_bzero(&app->keys, sizeof(uint8_t) * 512);
		ft_bzero(&app->mouse, sizeof(uint8_t) * 6);
		if (!event_handling(app))
			break;
		if (app->keys[SDL_SCANCODE_P])
			switch_mode(app);
		if (app->point_mode)
			editor_point_mode(app);
		else
			editor_live_mode(app);
		SDL_UpdateWindowSurface(app->sdl->window);
		reset_screen(app);
	}
}
