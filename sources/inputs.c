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

void 	points_mode_reset_camera_pos(t_v3d *pos)
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

void	point_mode_sector_save(t_app *app)
{
	sector_close(app, &app->sectors[app->sectors_count]);
}

void	live_mode_toggle_fly(t_app *app)
{
	app->camera->fly = !app->camera->fly;
}

void	live_mode_change_floor_h(t_app *app)
{
	t_sector *s;

	s = app->hit_sector;
	if (app->keys[SDL_SCANCODE_MINUS])
	{
		if (fabs(s->ceil_y - (s->floor_y - 0.5)) <= 0.0)
			return ;
		sector_pts_h(&s->fpts[0], s->pts_count, -0.5);
		s->floor_y -= 0.5;
	}
	else if (app->keys[SDL_SCANCODE_EQUALS])
	{
		if (fabs(s->ceil_y - (s->floor_y + 0.5)) <= 0.0)
			return ;
		sector_pts_h(&s->fpts[0], s->pts_count, 0.5);
		s->floor_y += 0.5;
	}
	if (app->keys[SDL_SCANCODE_MINUS] || app->keys[SDL_SCANCODE_EQUALS])
	{
		sector_update_height(s, &s->fpts[0], &s->cpts[0]);
		s->delta_y = fabs(s->ceil_y - s->floor_y);
	}
}

void	live_mode_change_ceil_h(t_app *app)
{
	t_sector *s;

	s = app->hit_sector;
	if (app->keys[SDL_SCANCODE_MINUS])
	{
		if (fabs((s->ceil_y - 0.5) - s->floor_y) <= 0.0)
			return ;
		sector_pts_h(&s->cpts[0], s->pts_count, -0.5);
		s->ceil_y -= 0.5;
	}
	else if (app->keys[SDL_SCANCODE_EQUALS])
	{
		if (fabs((s->ceil_y + 0.5) - s->floor_y) <= 0.0)
			return ;
		sector_pts_h(&s->cpts[0], s->pts_count, 0.5);
		s->ceil_y += 0.5;
	}
	if (app->keys[SDL_SCANCODE_MINUS] || app->keys[SDL_SCANCODE_EQUALS])
	{
		sector_update_height(s, &s->fpts[0], &s->cpts[0]);
		s->delta_y = fabs(s->ceil_y - s->floor_y);
	}
}

void	live_mode_wall_bot(t_app *app)
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

void	live_mode_wall_top(t_app *app)
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

void	live_mode_change_shade(t_app *app)
{
	if (app->keys[SDL_SCANCODE_MINUS] && app->hit_wall->shade > 0)
		app->hit_wall->shade--;
	else if (app->keys[SDL_SCANCODE_EQUALS] && app->hit_wall->shade < 6)
		app->hit_wall->shade++;
}

void	live_mode_add_decore(t_app *app)
{
		decore_add(app->hit_point,
				   app->hit_sector,
				   app->hit_wall,
				   app->camera);
}

void	live_mode_set_exit(t_app *app)
{
	if (app->hit_type == decor)
		app->hit_wall->is_exit = !app->hit_wall->is_exit;
}

void	live_mode_set_start(t_app *app)
{
	if (!app->camera->fly && app->floor_sector)
	{
		app->md.start_pos = app->camera->pos;
		app->md.start_set = 1;
	}
}

void	live_mode_use_exit(t_app *app)
{
	if (app->hit_type == decor &&
		app->hit_wall->is_exit &&
		app->hit_dist <= USE_DIST)
		exit(0);
}

void	live_mode_toggle_door(t_app *app)
{
	int			i;
	t_sector	*s;

	s = app->hit_sector;
	s->door = !s->door;
	i = 0;
	while (i < s->walls_count)
	{
		s->walls[i].sprite = s->door ? 228 : 103;
		i++;
	}
	s->floor.sprite = s->door ? 226 : 278;
	s->ceil.sprite = s->door ? 226 : 399;
	s->door_anim = 0;
}

void	live_mode_door_open(t_app *app)
{
	if (app->hit_sector->door && app->hit_dist <= USE_DIST)
	{
		app->hit_sector->door_anim = 1;
		Mix_PlayChannel(2, app->sfx[26], 0);
		Mix_PlayChannel(1, app->sfx[35], -1);
	}
}

void	live_mode_set_bg(t_app *app)
{
	int		prev;

	prev = app->md.music_id;
	if (app->keys[SDL_SCANCODE_0])
		app->md.music_id = 0;
	else if (app->keys[SDL_SCANCODE_1])
		app->md.music_id = 1;
	else if (app->keys[SDL_SCANCODE_2])
		app->md.music_id = 2;
	else if (app->keys[SDL_SCANCODE_3])
		app->md.music_id = 3;
	else if (app->keys[SDL_SCANCODE_4])
		app->md.music_id = 4;
	else if (app->keys[SDL_SCANCODE_5])
		app->md.music_id = 5;
	else if (app->keys[SDL_SCANCODE_6])
		app->md.music_id = 6;
	else if (app->keys[SDL_SCANCODE_7])
		app->md.music_id = 7;
	else if (app->keys[SDL_SCANCODE_8])
		app->md.music_id = 8;
	else if (app->keys[SDL_SCANCODE_9])
		app->md.music_id = 9;
	if (prev != app->md.music_id)
		Mix_PlayMusic(app->bg[app->md.music_id], -1);
}


void	live_mode_sector_io(t_app *app)
{
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
	if (app->keys[SDL_SCANCODE_R])
		points_mode_reset_camera_pos(&app->camera->pos);
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

void 	live_mode_add_obj(t_app *app)
{
	t_wall	obj;

	if (app->floor_sector == NULL && app->camera->fly)
		return ;
	obj = wall_new();
	obj.pos = app->floor_point;
	obj.size = 1.0;
	obj.sprite = 145;
	obj.rotate = 0;
	app->floor_sector->obj[app->floor_sector->objs_count] = obj;
	app->floor_sector->objs_count++;
	app->floor_sector->objs_count %= MAX_OBJ;
}

void 	live_mode_add_npc(t_app *app)
{
	t_wall	obj;

	if (app->floor_sector == NULL && app->camera->fly)
		return ;
	obj = wall_new();
	obj.pos = app->floor_point;
	obj.size = 1.0;
	obj.sprite = 499;
	obj.rotate = 1;
	app->floor_sector->npc[app->floor_sector->npcs_count] = obj;
	app->floor_sector->npcs_count++;
	app->floor_sector->npcs_count %= MAX_NPC;
}

void	live_mode_rotate_npc(t_app *app)
{
	if (app->keys[SDL_SCANCODE_PAGEUP])
		app->hit_wall->ori++;
	if (app->keys[SDL_SCANCODE_PAGEDOWN])
		app->hit_wall->ori--;
	if (app->hit_wall->ori > 7)
		app->hit_wall->ori = 0;
	if (app->hit_wall->ori < 0)
		app->hit_wall->ori = 7;
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
			live_mode_change_floor_h(app);
		else if (app->inputs->keyboard[SDL_SCANCODE_C])
			live_mode_change_ceil_h(app);
		else if (app->inputs->keyboard[SDL_SCANCODE_L])
			live_mode_change_shade(app);
		else if (app->inputs->keyboard[SDL_SCANCODE_T] && app->hit_type == wall)
			live_mode_add_decore(app);
		else if (app->inputs->keyboard[SDL_SCANCODE_Q] && app->hit_type == wall)
			live_mode_wall_top(app);
		else if (app->inputs->keyboard[SDL_SCANCODE_E] && app->hit_type == wall)
			live_mode_wall_bot(app);
		if (app->keys[SDL_SCANCODE_I])
			live_mode_sector_io(app);
		if (app->inputs->keyboard[SDL_SCANCODE_O])
			live_mode_wall_offset(app);
		if (app->hit_wall && app->keys[SDL_SCANCODE_F1])
			live_mode_set_exit(app);
		if (app->hit_sector && app->keys[SDL_SCANCODE_F2])
			live_mode_toggle_door(app);
		if (app->hit_wall && app->keys[SDL_SCANCODE_E])
			live_mode_use_exit(app);
		if (app->keys[SDL_SCANCODE_V])
			live_mode_door_open(app);
		if (app->inputs->keyboard[SDL_SCANCODE_M])
			live_mode_set_bg(app);
		if (app->hit_type == npc && app->inputs->keyboard[SDL_SCANCODE_RSHIFT])
			live_mode_rotate_npc(app);
	}
	if (app->keys[SDL_SCANCODE_R])
		live_mode_toggle_fly(app);
	if (app->keys[SDL_SCANCODE_B])
		live_mode_add_npc(app);
	if (app->keys[SDL_SCANCODE_O])
		live_mode_add_obj(app);
	if (app->keys[SDL_SCANCODE_F5])
		live_mode_set_start(app);
}
