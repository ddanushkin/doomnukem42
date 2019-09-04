#include "doom_nukem.h"

void	*fast_bzero(void *b, int c, size_t len)
{
	unsigned char *dst;
	t_zero_char *dst_64;
	t_zero_char zero;

	ft_bzero(&zero, 1024);
	dst_64 = b;
	while (len >= 1024)
	{
		*dst_64 = zero;
		dst_64++;
		len -= 1024;
	}

	dst = (void *)dst_64;
	while (len > 0)
	{
		*dst = (unsigned char)c;
		dst++;
		len--;
	}
	return (b);
}

void	clear_screen(t_app *app)
{
	fast_bzero(app->screen.pixels, 0,app->window.w * app->window.h * 4);
	//bzero(app->screen.pixels, app->window.w * app->window.h * 4);
	//ft_bzero(app->screen.pixels,app->window.w * app->window.h * 4);
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

int		update(t_app *app)
{
	clock_t current_ticks, delta_ticks;
	clock_t fps = 0;

	current_ticks = clock();
	clear_screen(app);

	update_inputs(app);

	app->rot.x += app->speed > 0.0f ? app->speed : 0.0f;
	app->rot.z += app->speed > 0.0f ? app->speed : 0.0f;

	update_rotation_mat_z(app, app->rot.z);
	update_rotation_mat_x(app, app->rot.x);

	int repeat = 0;

	make_cube(&app->cube, 1);

	while (repeat >= 0)
	{
		draw_cube(app, &app->cube);
		repeat--;
	}

	free(app->cube.v);

	mlx_put_image_to_window(app->mlx, app->window.ptr, app->screen.ptr, 0, 0);

	delta_ticks = clock() - current_ticks;

	if(delta_ticks > 0)
		fps = CLOCKS_PER_SEC / delta_ticks;

	char *fps_text;
	fps_text = ft_itoa((int)fps);
	mlx_string_put(app->mlx, app->window.ptr, 10, 10, 0xFFFFFF, "FPS: ");
	mlx_string_put(app->mlx, app->window.ptr, 55, 10, 0xFFFFFF, fps_text);
	ft_strdel(&fps_text);
	reset_inputs_states(app);
	return (0);
}

int		main()
{
	t_app	app;

	init_app(&app);
	mlx_hook(app.window.ptr, 17, 5, window_event_close, &app);
	mlx_hook(app.window.ptr, 12, 5, window_event_expose, &app);
	mlx_hook(app.window.ptr, 2, 5, keyboard_event_down, &app);
	mlx_hook(app.window.ptr, 3, 5, keyboard_event_up, &app);
	mlx_hook(app.window.ptr, 4, 5, mouse_event_down, &app);
	mlx_hook(app.window.ptr, 5, 5, mouse_event_up, &app);
	mlx_hook(app.window.ptr, 6, 5, mouse_event_move, &app);
	mlx_loop_hook(app.mlx, update, &app);
	mlx_loop(app.mlx);
}