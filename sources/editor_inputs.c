#include "doom_nukem.h"

void	update_wall_shade(t_app *app, t_wall *w)
{
	uint32_t *s;

	s = &app->sprites[w->sprite].pixels[0];
	reset_shade(s, &w->t[0], w->sx, w->sy);
	fill_shade_wall(&app->hit_sector->l, w->v[0], w->v[1], &w->t[0]);
}

void	texture_change(t_app *app)
{
	if (app->keys[SDL_SCANCODE_MINUS])
		app->hit_wall->sprite--;
	else if (app->keys[SDL_SCANCODE_EQUALS])
		app->hit_wall->sprite++;
	app->hit_wall->sprite =
			(app->hit_wall->sprite % 493 + 493) % 493;
	if (app->keys[SDL_SCANCODE_MINUS] || app->keys[SDL_SCANCODE_EQUALS])
		update_wall_shade(app, app->hit_wall);
}

void	texture_scale_y_change(t_app *app)
{
	if (app->keys[SDL_SCANCODE_MINUS] && app->hit_wall->sy < 10.0)
		app->hit_wall->sy += 0.25;
	else if (app->keys[SDL_SCANCODE_EQUALS] && app->hit_wall->sy > 0.25)
		app->hit_wall->sy -= 0.25;
	if (app->keys[SDL_SCANCODE_MINUS] || app->keys[SDL_SCANCODE_EQUALS])
		update_wall_shade(app, app->hit_wall);
}

void	texture_scale_x_change(t_app *app)
{
	if (app->keys[SDL_SCANCODE_MINUS] && app->hit_wall->sx < 10.0)
		app->hit_wall->sx += 0.25;
	else if (app->keys[SDL_SCANCODE_EQUALS] && app->hit_wall->sx > 0.25)
		app->hit_wall->sx -= 0.25;
	if (app->keys[SDL_SCANCODE_MINUS] || app->keys[SDL_SCANCODE_EQUALS])
		update_wall_shade(app, app->hit_wall);
}
