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
	int	id;

	app->point_mode = !app->point_mode;
	if (app->point_mode)
		camera_point_mode(&app->camera->pos, &app->camera->rot);
	else
	{
		id = app->sectors_count - 1;
		camera_live_mode(&app->camera->rot);
		app->camera->pos = point_save(app, app->cursor_x, app->cursor_y, 0);
		app->camera->pos.y = app->sectors[id].floor_y + 1.0;
	}
	return (1);
}

void	point_mode_switch_grid(t_app *app)
{
	app->grid_size = app->grid_size == 2.0 ? 0.5 : 2.0;
}

int 	point_exist(t_v3d *points, int size, t_v3d point)
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

void	point_mode_mouse(t_app *app)
{
	t_v3d	new_point;

	if (app->mouse[SDL_MOUSE_LEFT])
	{
		new_point = point_save(app, app->cursor_x, app->cursor_y, 1);
		if (!point_exist(&app->points[0], app->points_count, new_point))
		{
			app->points[app->points_count] = new_point;
			app->points_count++;
		}
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
	t_sector *s;

	s = app->hit_sector;
	if (app->keys[SDL_SCANCODE_MINUS])
	{
		sector_pts_h(&s->fpts[0], s->pts_count, -0.5);
		s->floor_y -= 0.5;
	}
	else if (app->keys[SDL_SCANCODE_EQUALS])
	{
		sector_pts_h(&s->fpts[0], s->pts_count, 0.5);
		s->floor_y += 0.5;
	}
	if (app->keys[SDL_SCANCODE_MINUS] || app->keys[SDL_SCANCODE_EQUALS])
		sector_update_height(s, &s->fpts[0], &s->cpts[0]);
}

void	live_edit_change_ceil_h(t_app *app)
{
	t_sector *s;

	s = app->hit_sector;
	if (app->keys[SDL_SCANCODE_MINUS])
	{
		sector_pts_h(&s->cpts[0], s->pts_count, -0.5);
		s->ceil_y -= 0.5;
	}
	else if (app->keys[SDL_SCANCODE_EQUALS])
	{
		sector_pts_h(&s->cpts[0], s->pts_count, 0.5);
		s->ceil_y += 0.5;
	}
	if (app->keys[SDL_SCANCODE_MINUS] || app->keys[SDL_SCANCODE_EQUALS])
		sector_update_height(s, &s->fpts[0], &s->cpts[0]);
}

void	live_edit_wall_bot(t_app *app)
{
	t_sector	*s;
	t_wall		*w;

	s = app->hit_sector;
	w = app->hit_wall;
	if (app->keys[SDL_SCANCODE_MINUS])
	{
		s->fpts[w->v[0].i].y -= 0.5;
		s->fpts[w->v[2].i].y -= 0.5;
	}
	else if (app->keys[SDL_SCANCODE_EQUALS])
	{
		s->fpts[w->v[0].i].y += 0.5;
		s->fpts[w->v[2].i].y += 0.5;
	}
	if (app->keys[SDL_SCANCODE_MINUS] || app->keys[SDL_SCANCODE_EQUALS])
		sector_update_height(s, &s->fpts[0], &s->cpts[0]);
}

void	live_edit_wall_top(t_app *app)
{
	t_sector	*s;
	t_wall		*w;

	s = app->hit_sector;
	w = app->hit_wall;
	if (app->keys[SDL_SCANCODE_MINUS])
	{
		s->cpts[w->v[3].i].y -= 0.5;
		s->cpts[w->v[1].i].y -= 0.5;
	}
	else if (app->keys[SDL_SCANCODE_EQUALS])
	{
		s->cpts[w->v[3].i].y += 0.5;
		s->cpts[w->v[1].i].y += 0.5;
	}
	if (app->keys[SDL_SCANCODE_MINUS] || app->keys[SDL_SCANCODE_EQUALS])
		sector_update_height(s, &s->fpts[0], &s->cpts[0]);
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

void 	sector_flip_walls(t_sector *s)
{
	int i;

	i = 0;
	while (i < s->walls_count)
	{
		s->walls[i].flip = !s->walls[i].flip;
		i++;
	}
}

void	live_edit_sector_io(t_app *app)
{
	sector_flip_walls(app->hit_sector);
	app->hit_sector->floor.flip = !app->hit_sector->floor.flip;
	app->hit_sector->ceil.flip = !app->hit_sector->ceil.flip;
	app->hit_sector->inside = !app->hit_sector->inside;
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

void 	wall_offset_x(t_app *app, t_wall *w)
{
	if (app->inputs->keyboard[SDL_SCANCODE_RIGHT])
	{
		w->v[0].tex_x -= 0.001;
		w->v[1].tex_x -= 0.001;
		w->v[2].tex_x -= 0.001;
		w->v[3].tex_x -= 0.001;
	}
	if (app->inputs->keyboard[SDL_SCANCODE_LEFT])
	{
		w->v[0].tex_x += 0.001;
		w->v[1].tex_x += 0.001;
		w->v[2].tex_x += 0.001;
		w->v[3].tex_x += 0.001;
	}
}

void 	wall_offset_y(t_app *app, t_wall *w)
{
	if (app->inputs->keyboard[SDL_SCANCODE_UP])
	{
		w->v[0].tex_y -= 0.001;
		w->v[1].tex_y -= 0.001;
		w->v[2].tex_y -= 0.001;
		w->v[3].tex_y -= 0.001;
	}
	if (app->inputs->keyboard[SDL_SCANCODE_DOWN])
	{
		w->v[0].tex_y += 0.001;
		w->v[1].tex_y += 0.001;
		w->v[2].tex_y += 0.001;
		w->v[3].tex_y += 0.001;
	}
}

void 	live_mode_wall_offset(t_app *app)
{
	wall_offset_x(app, app->hit_wall);
	wall_offset_y(app, app->hit_wall);
}

void	live_mode_inputs(t_app *app)
{
	if (app->hit_wall)
	{
		if (app->inputs->keyboard[SDL_SCANCODE_LSHIFT])
			texture_change(app);
		else if (app->inputs->keyboard[SDL_SCANCODE_LALT])
			texture_scale_y_change(app);
		else if (app->inputs->keyboard[SDL_SCANCODE_LCTRL])
			texture_scale_x_change(app);
		else if (app->keys[SDL_SCANCODE_DELETE])
			app->hit_wall->active = 0;
		else if (app->keys[SDL_SCANCODE_TAB] && (app->hit_type == wall || app->hit_type == floor_ceil))
			app->hit_wall->flip = !app->hit_wall->flip;
		else if (app->inputs->keyboard[SDL_SCANCODE_F])
			live_edit_change_floor_h(app);
		else if (app->inputs->keyboard[SDL_SCANCODE_C])
			live_edit_change_ceil_h(app);
		else if (app->inputs->keyboard[SDL_SCANCODE_L])
			live_edit_change_shade(app);
		else if (app->inputs->keyboard[SDL_SCANCODE_T] && app->hit_type == wall)
			live_edit_add_decore(app);
		else if (app->inputs->keyboard[SDL_SCANCODE_Q] && app->hit_type == wall)
			live_edit_wall_top(app);
		else if (app->inputs->keyboard[SDL_SCANCODE_E] && app->hit_type == wall)
			live_edit_wall_bot(app);
		if (app->keys[SDL_SCANCODE_R])
			live_mode_toggle_fly(app);
		if (app->keys[SDL_SCANCODE_I])
			live_edit_sector_io(app);
		if (app->inputs->keyboard[SDL_SCANCODE_O])
			live_mode_wall_offset(app);
	}
}