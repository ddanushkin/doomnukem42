#include "doom_nukem.h"

void	texture_change(t_app *app)
{
	if (app->keys[SDL_SCANCODE_MINUS])
		app->hit_wall->sprite--;
	else if (app->keys[SDL_SCANCODE_EQUALS])
		app->hit_wall->sprite++;
	app->hit_wall->sprite =
			(app->hit_wall->sprite % 493 + 493) % 493;
	reset_shade(&app->sprites[app->hit_wall->sprite].pixels[0],
			&app->hit_wall->t[0],
			app->hit_wall->sx,
			app->hit_wall->sy);
	fill_shade_wall(&app->hit_sector->l,
			app->hit_wall->v[0],
			app->hit_wall->v[1],
			&app->hit_wall->t[0]);
}

void	texture_scale_y_change(t_app *app)
{
	if (app->keys[SDL_SCANCODE_MINUS] && app->hit_wall->sy < 10.0)
		app->hit_wall->sy += 0.25;
	else if (app->keys[SDL_SCANCODE_EQUALS] && app->hit_wall->sy > 0.25)
		app->hit_wall->sy -= 0.25;
	reset_shade(&app->sprites[app->hit_wall->sprite].pixels[0],
				&app->hit_wall->t[0],
				app->hit_wall->sx,
				app->hit_wall->sy);
	fill_shade_wall(&app->hit_sector->l,
			app->hit_wall->v[0],
			app->hit_wall->v[1],
			&app->hit_wall->t[0]);
}

void	texture_scale_x_change(t_app *app)
{
	if (app->keys[SDL_SCANCODE_MINUS] && app->hit_wall->sx < 10.0)
		app->hit_wall->sx += 0.25;
	else if (app->keys[SDL_SCANCODE_EQUALS] && app->hit_wall->sx > 0.25)
		app->hit_wall->sx -= 0.25;
	reset_shade(&app->sprites[app->hit_wall->sprite].pixels[0],
				&app->hit_wall->t[0],
				app->hit_wall->sx,
				app->hit_wall->sy);
	fill_shade_wall(
			&app->hit_sector->l,
			app->hit_wall->v[0],
			app->hit_wall->v[1],
			&app->hit_wall->t[0]);
}
