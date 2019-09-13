#include "doom_nukem.h"

void	clear_screen(t_app *app)
{
	image_clear(app->sdl->surface->pixels, 0, SCREEN_W * SCREEN_H * 4);
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

void	draw_mesh(t_app *app, t_mesh *mesh)
{
	int t;
	t_triangle triangle;

	t = 0;

	while (t < mesh->t_idx)
	{
		triangle = mesh->t[t];
		set_color(&triangle.color, 128, 128, 128);
		draw_triangle(app, triangle);
		t++;
	}
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
		clear_screen(app);
		if (!event_handling(app))
			break;

		app->rot.x += app->speed * app->sdl->timer->delta_ticks * 0.00001f;
		app->rot.z += app->speed * app->sdl->timer->delta_ticks * 0.00001f;

		//printf("speed: %f", app->speed);

		update_rotation_mat_z(app, app->rot.z);
		update_rotation_mat_x(app, app->rot.x);

		int repeat = 0;

//		make_cube(&app->cube, fabsf(sinf(app->sdl->timer->current_ticks * 0.000001f)) + 1);
//		while (repeat >= 0)
//		{
//			draw_cube(app, &app->cube);
//			repeat--;
//		}
//		free(app->cube.v);

		draw_mesh(app, &app->mesh[0]);

		SDL_UpdateWindowSurface(app->sdl->window);

		get_delta_time(app->sdl->timer);
	}
	SDL_Quit();
	SDL_DestroyWindow(app->sdl->window);
}

int 	cmpr(const void *p, const void *q)
{
	t_triangle t1 = *(t_triangle *)p;
	t_triangle  t2 = *(t_triangle *)q;

	float z1 = (t1.v[0]->z + t1.v[1]->z + t1.v[2]->z) / 3.0f;
	float z2 = (t2.v[0]->z + t2.v[1]->z + t2.v[2]->z) / 3.0f;
	return z1 > z2;
}

int		main(int argv, char**argc)
{
	t_app	*app;

	app = (t_app *)malloc(sizeof(t_app));
	app->mesh = (t_mesh *)malloc(sizeof(t_mesh) * 1);

	read_obj("../Wolf.obj", &app->mesh[0]);

	init_app(app);
	start_the_game(app);
	quit_properly(app);
	return (0);
}

