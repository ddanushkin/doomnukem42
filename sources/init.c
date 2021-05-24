/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lglover <lglover@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/23 17:27:29 by lglover           #+#    #+#             */
/*   Updated: 2020/03/23 17:29:20 by lglover          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"

void	init_map(t_app *app)
{
	app->sectors_count = 0;
	app->md.start_set = 0;
	app->md.music_id = 8;
	app->md.card_set = 0;
	app->md.card_picked = 0;
	app->sectors = (t_sector *)malloc(sizeof(t_sector) * MAX_SECTOR);
}

void	init_projection_mat(t_app *app)
{
	t_camera *c;

	c = app->camera;
	ft_bzero(&app->proj_mat, sizeof(t_mat4x4));
	app->proj_mat.m[0] = c->asp_ratio * c->for_rad;
	app->proj_mat.m[5] = c->for_rad;
	app->proj_mat.m[10] = c->z_far / (c->z_far - c->z_near);
	app->proj_mat.m[14] = (-c->z_far * c->z_near) / (c->z_far - c->z_near);
	app->proj_mat.m[11] = 1.0;
	app->proj_mat.m[15] = 0.0;
}

void	init_camera(t_camera *camera)
{
	camera->fov = TO_RAD(60.0);
	camera->z_far = 1000.0;
	camera->z_near = 0.1;
	camera->asp_ratio = (double)SCREEN_H / (double)SCREEN_W;
	camera->for_rad = 1.0 / tan(camera->fov * 0.5 / 180.0 * 3.14159);
	camera->pos = new_vector(1.0, 1.0, -10);
	camera->up = new_vector(0.0, 1.0, 0.0);
	camera->rot = new_vector(0.0, 0.0, 0.0);
	camera->projection = matrix_persp(
			camera->fov,
			(double)SCREEN_W / (double)SCREEN_H,
			0.05,
			1000.0);
	camera->screen_space = matrix_screen_space();
	camera->fly = 0;
}

void	init_skybox(t_app *app)
{
	double size;

	size = 100.0;
	app->skybox.v[0] = new_vector(-size, -size, size);
	app->skybox.v[1] = new_vector(size, size, size);
	app->skybox.v[2] = new_vector(size, -size, size);
	app->skybox.v[3] = new_vector(-size, size, size);
	app->skybox.v[4] = new_vector(-size, -size, -size);
	app->skybox.v[5] = new_vector(size, size, -size);
	app->skybox.v[6] = new_vector(size, -size, -size);
	app->skybox.v[7] = new_vector(-size, size, -size);
}

void	init_weapon(t_app *app)
{
	app->weapon.current_weapon = KNIFE;
	app->weapon.is_reload = 0;
	app->weapon.current_ammo = INFINITY_AMMO;
	app->weapon.start_reload_time = 0;
	app->weapon.start_change_weapon_time = 0;
}

void	app_init(t_app *app)
{
	init_sdl(app->sdl);
	app->screen = app->sdl->surface->pixels;
	app->inputs->keyboard = SDL_GetKeyboardState(NULL);
	app->timer->prev = 0;
	app->timer->delta = 0;
	app->timer->fps = 0;
	app->timer->fps_count = 0;
	app->timer->fps_sum = 0;
	app->timer->delta = 0.0;
	app->timer->time = 0.0;
	app->timer->frame = 0;
	app->grid_size = 0.5;
	init_camera(app->camera);
	init_projection_mat(app);
	init_font(app);
	init_sfx(app);
	init_bg(app);
	init_skybox(app);
	app->card_hud = hud_image_new(HUD_CARD_POS_X, HUD_CARD_POS_Y, 64, 64);
	app->hp_hud = hud_image_new(HUD_HP_POS_X, HUD_HP_POS_Y, 64, 64);
	app->time_hud = hud_image_new(HUD_TIME_POS_X, HUD_TIME_POS_Y, 64, 64);
	init_weapon(app);
}
