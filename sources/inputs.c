#include "doom_nukem.h"

void 	camera_point_mode(t_v3d *pos, t_v3d *rot)
{
	pos->y = 10.0;
	rot->x = 1.57;
	rot->y = 0.0;
	rot->z = 0.0;
}

void 	camera_live_mode(t_v3d *rot)
{
	rot->x = 0.0;
	rot->y = 0.0;
	rot->z = 0.0;
}

int		switch_mode(t_app *app)
{
	app->point_mode = !app->point_mode;
	if (app->point_mode)
		camera_point_mode(&app->camera->pos, &app->camera->rot);
	else
		camera_live_mode(&app->camera->rot);
	return (1);
}

void	point_mode_switch_grid(t_app *app)
{
	app->grid_size = app->grid_size == 2.0 ? 0.5 : 2.0;
}

void	point_mode_mouse(t_app *app)
{
	if (app->mouse[SDL_MOUSE_LEFT])
	{
		app->points[app->points_count] = point_save(app, app->cursor_x, app->cursor_y);
		app->points_count++;
	}
	else if (app->mouse[SDL_MOUSE_RIGHT] && app->points_count > 0)
		app->points_count--;
}

void	point_mode_sector_save(t_app *app)
{
	sector_close(app, &app->sectors[app->sectors_count]);
}

void	live_mode_toggle_fly(t_app *app)
{
	app->camera->fly = !app->camera->fly;
	if (!app->camera->fly)
		app->fall = app->camera->pos.y - app->floor_point.y - app->height;
}

void	live_edit_change_floor_h(t_app *app)
{
	if (app->keys[SDL_SCANCODE_MINUS])
		app->hit_sector->floor_y -= 0.5;
	else if (app->keys[SDL_SCANCODE_EQUALS])
		app->hit_sector->floor_y += 0.5;
	sector_update_height(app->hit_sector);
}

void	live_edit_change_ceil_h(t_app *app)
{
	if (app->keys[SDL_SCANCODE_MINUS])
		app->hit_sector->ceil_y -= 0.5;
	else if (app->keys[SDL_SCANCODE_EQUALS])
		app->hit_sector->ceil_y += 0.5;
	sector_update_height(app->hit_sector);
}

void	live_edit_change_shade(t_app *app)
{
	if (app->keys[SDL_SCANCODE_MINUS] && app->hit_wall->shade > 0)
		app->hit_wall->shade--;
	else if (app->keys[SDL_SCANCODE_EQUALS] && app->hit_wall->shade < 6)
		app->hit_wall->shade++;
}

void	live_edit_add_decore(t_app *app)
{
		decore_add(app->hit_point,
				   app->hit_sector,
				   app->hit_wall,
				   app->camera);
}

void	point_mode_inputs(t_app *app)
{
	if (app->keys[SDL_SCANCODE_G])
		point_mode_switch_grid(app);
	if (app->mouse[SDL_MOUSE_LEFT] || app->mouse[SDL_MOUSE_RIGHT])
		point_mode_mouse(app);
	if (app->keys[SDL_SCANCODE_Q] && app->points_count >= 3)
		point_mode_sector_save(app);
}

void	live_mode_inputs(t_app *app)
{
	if (app->hit_wall && app->hit_sector->ready)
	{
		if (app->inputs->keyboard[SDL_SCANCODE_LSHIFT])
			texture_change(app);
		else if (app->inputs->keyboard[SDL_SCANCODE_LALT])
			texture_scale_y_change(app);
		else if (app->inputs->keyboard[SDL_SCANCODE_LCTRL])
			texture_scale_x_change(app);
		else if (app->keys[SDL_SCANCODE_DELETE])
			app->hit_wall->active = 0;
		else if (app->inputs->keyboard[SDL_SCANCODE_F])
			live_edit_change_floor_h(app);
		else if (app->inputs->keyboard[SDL_SCANCODE_C])
			live_edit_change_ceil_h(app);
		else if (app->inputs->keyboard[SDL_SCANCODE_L])
			live_edit_change_shade(app);
		else if (app->keys[SDL_SCANCODE_T] && app->hit_type == wall)
			live_edit_add_decore(app);
		if (app->keys[SDL_SCANCODE_R])
			live_mode_toggle_fly(app);
	}
}