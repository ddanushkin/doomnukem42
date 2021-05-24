#include "doom_nukem.h"

void	process_change_weapon_buttons(t_app *app, const uint8_t *key)
{
	if (key[SDL_SCANCODE_1] && app->weapon.current_weapon != AUTO)
		printf("AUTO\n");
	else if (key[SDL_SCANCODE_2] && app->weapon.current_weapon != PISTOL)
		printf("PISTOL\n");
	else if ((key[SDL_SCANCODE_3] && app->weapon.current_weapon != KNIFE))
		printf("KNIFE\n");
}

int 	get_current_weapon_max_ammo(int current_weapon)
{
	if (current_weapon == AUTO)
		return MAX_AUTO_AMMO;
	else if (current_weapon == PISTOL)
		return MAX_PISTOL_AMMO;
	else
		return INFINITY_AMMO;
}

void	process_reload_weapon_button(t_app *app, const uint8_t *key)
{
	if (key[SDL_SCANCODE_R] && app->weapon.current_ammo < get_current_weapon_max_ammo(app))
	{
		app->weapon.is_reload = 1;
		app->weapon.start_reload_time = app->timer->time;
	}
}

void	draw_reloading(t_app *app)
{
	printf("DRAW %d\n", app->weapon.current_ammo);
}

void	process_reload_statement(t_app *app)
{
	if (app->timer->time - app->weapon.start_reload_time >= RELOADING_TIME_SEC)
	{
		printf("RELOAD FINISHED\n");
		app->weapon.is_reload = 0;
		app->weapon.start_reload_time = 0;
		app->weapon.current_ammo = get_current_weapon_max_ammo(app->weapon.current_weapon);
	}
	else
	{
		printf("RELOAD\n");
	}
}

void	process_shooting(t_app *app)
{
	if (!app->weapon.is_shooting && app->mouse[SDL_MOUSE_LEFT])
		printf("SHOOT");
}

void	weapon_handling(t_app *app, const uint8_t *key)
{
	if (!app->weapon.is_reload)
	{
		process_shooting(app);
		process_change_weapon_buttons(app, key);
		process_reload_weapon_button(app, key);
	}
	else
	{
		process_reload_statement(app);
		draw_reloading(app);
	}
}