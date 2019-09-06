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

void	get_color(SDL_Surface *surface, int x, int y, t_color *c)
{
	int 		offset;

	offset = 4 * (y * surface -> w + x);
	unsigned char* pixels = (unsigned char*)surface -> pixels;
	pixels[offset] = c->b;
	pixels[offset + 1] = c->g;
	pixels[offset + 2] = c->r;
}

void	start_the_game(t_app *app)
{
	t_color color;

	color.r = 255;
	color.g = 0;
	color.b = 0;
	while (1)
	{
		get_ticks(app->sdl->timer);
		SDL_PollEvent(&app->sdl->event);
		if (app->sdl->event.type == SDL_QUIT)
			break;
		event_handling(app);
		get_color(app->sdl->surface, 100, 100, &color);
		get_color(app->sdl->surface, 200, 200, &color);

		SDL_UpdateWindowSurface(app->sdl->window);

		get_delta_time(app->sdl->timer);
		show_fps_sdl(app->sdl->timer);
	}
	SDL_Quit();
	SDL_DestroyWindow(app->sdl->window);
}

int		main(int argv, char**argc)
{
	t_app	*app;

	app = (t_app *)malloc(sizeof(t_app));
	init_app(app);
	start_the_game(app);
	quit_properly(app);
	return (0);
}

