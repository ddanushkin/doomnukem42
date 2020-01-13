#include "doom_nukem.h"

void	texture_change(t_app *app)
{
	if (app->input_minus)
	{
		app->input_minus = 0;
		app->hit_wall->sprite_index--;
	}
	else if (app->input_plus)
	{
		app->input_plus = 0;
		app->hit_wall->sprite_index++;
	}
}

void	texture_scale_y_change(t_app *app)
{
	if (app->input_minus && app->hit_wall->scale_y < 10.0)
	{
		app->hit_wall->scale_y += 0.25;
		app->input_minus = 0;
	}
	else if (app->input_plus && app->hit_wall->scale_y > 0.25)
	{
		app->hit_wall->scale_y -= 0.25;
		app->input_plus = 0;
	}
	app->hit_wall->sprite_index =
			(app->hit_wall->sprite_index % 493 + 493) % 493;
}

void	texture_scale_x_change(t_app *app)
{
	if (app->input_minus && app->hit_wall->scale_x < 10.0)
	{
		app->hit_wall->scale_x += 0.25;
		app->input_minus = 0;
	}
	else if (app->input_plus && app->hit_wall->scale_x > 0.25)
	{
		app->hit_wall->scale_x -= 0.25;
		app->input_plus = 0;
	}
	app->hit_wall->sprite_index =
			(app->hit_wall->sprite_index % 493 + 493) % 493;
}