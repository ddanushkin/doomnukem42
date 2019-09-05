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
	t_color c;

	c.b = 255;
	c.r = 255;
	c.g = 255;

	get_ticks(&app->timer);
	clear_screen(app);
	update_inputs(app);

	set_pixel(&app->screen, 100, 100, &c);
	set_pixel(&app->screen, 200, 200, &c);
//	app->rot.x += app->speed > 0.0f ? app->speed : 0.0f;
//	app->rot.z += app->speed > 0.0f ? app->speed : 0.0f;
//
//	update_rotation_mat_z(app, app->rot.z);
//	update_rotation_mat_x(app, app->rot.x);
//
//	int repeat = 0;
//
//	make_cube(&app->cube, 2.6f);
//	while (repeat >= 0)
//	{
//		draw_cube(app, &app->cube);
//		repeat--;
//	}
//	free(app->cube.v);

	mlx_put_image_to_window(app->mlx, app->window.ptr, app->screen.ptr, 0, 0);
	get_delta_time(&app->timer);
	show_fps(app);
	reset_inputs_states(app);
	return (0);
}

void	get_color(SDL_Surface *surface, int x, int y, t_color *c)
{
	int 		offset;

	offset = 4 * (y * surface -> w + x);
	unsigned char* pixels = (unsigned char*)surface -> pixels;
	pixels[offset] = c->b;
	pixels[offset + 1] = c->g;
	pixels[offset + 2] = c->r;
}

void	start_the_game(t_sdl *sdl)
{

	while (1)
	{
		get_ticks(&sdl->timer);
		SDL_PollEvent(&sdl->event);
		if (sdl->event.type == SDL_QUIT)
			break;

		//get_color(sdl->surface, 100, 100, &clr);
		//get_color(sdl->surface, 200, 200, &clr);

		SDL_UpdateWindowSurface(sdl->window);

		get_delta_time(&sdl->timer);
		show_fps_sdl(&sdl->timer);
	}
	SDL_Quit();
	SDL_DestroyWindow(sdl->window);
}

int		main()
{
	t_app	*app;
	app = (t_app *)malloc(sizeof(t_app));
	init_app(app);
	start_the_game(app->sdl);
	quit_properly(app);
	//mlx_events(&app);
	//mlx_loop_hook(app.mlx, update, &app);
	//mlx_loop(app.mlx);
}

