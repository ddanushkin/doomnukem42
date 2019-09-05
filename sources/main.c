#include "doom_nukem.h"

void	clear_screen(t_app *app)
{
	image_clear(app->screen.pixels, 0, app->window.w * app->window.h * 4);
	//bzero(app->screen.pixels,app->window.w * app->window.h * 4);
}

void	draw_cube(t_app *app, t_mesh *m)
{
	set_color(&m->t[0].color, 255, 0, 0);
	draw_triangle(app, m->t[0]);
	set_color(&m->t[1].color, 0, 255, 0);
	draw_triangle(app, m->t[1]);
	set_color(&m->t[2].color, 0, 0, 255);
	draw_triangle(app, m->t[2]);
	set_color(&m->t[3].color, 0, 255, 255);
	draw_triangle(app, m->t[3]);
	set_color(&m->t[4].color, 255, 255, 0);
	draw_triangle(app, m->t[4]);
	set_color(&m->t[5].color, 255, 0, 255);
	draw_triangle(app, m->t[5]);
	set_color(&m->t[6].color, 255, 0, 0);
	draw_triangle(app, m->t[6]);
	set_color(&m->t[7].color, 0, 255, 0);
	draw_triangle(app, m->t[7]);
	set_color(&m->t[8].color, 0, 0, 255);
	draw_triangle(app, m->t[8]);
	set_color(&m->t[9].color, 0, 255, 255);
	draw_triangle(app, m->t[9]);
	set_color(&m->t[10].color, 255, 255, 0);
	draw_triangle(app, m->t[10]);
	set_color(&m->t[11].color, 255, 0, 255);
	draw_triangle(app, m->t[11]);
}

void	mlx_events(t_app *app)
{
	mlx_hook(app->window.ptr, 17, 5, window_event_close, app);
	mlx_hook(app->window.ptr, 12, 5, window_event_expose, app);
	mlx_hook(app->window.ptr, 2, 5, keyboard_event_down, app);
	mlx_hook(app->window.ptr, 3, 5, keyboard_event_up, app);
	mlx_hook(app->window.ptr, 4, 5, mouse_event_down, app);
	mlx_hook(app->window.ptr, 5, 5, mouse_event_up, app);
	mlx_hook(app->window.ptr, 6, 5, mouse_event_move, app);
}

int		update(t_app *app)
{
	get_ticks(app);
	clear_screen(app);
	update_inputs(app);

//	app->rot.x += app->speed > 0.0f ? app->speed : 0.0f;
//	app->rot.z += app->speed > 0.0f ? app->speed : 0.0f;

	update_rotation_mat_z(app, app->rot.z);
	update_rotation_mat_x(app, app->rot.x);

	int repeat = 0;

	make_cube(&app->cube, 2.6f);
	while (repeat >= 0)
	{
		draw_cube(app, &app->cube);
		repeat--;
	}
	free(app->cube.v);

	mlx_put_image_to_window(app->mlx, app->window.ptr, app->screen.ptr, 0, 0);
	get_delta_time(app);
	show_fps(app);
	reset_inputs_states(app);
	return (0);
}

int		main()
{
	t_app	app;

	init_app(&app);
	mlx_events(&app);
	mlx_loop_hook(app.mlx, update, &app);
	mlx_loop(app.mlx);
}