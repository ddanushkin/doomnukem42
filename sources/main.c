#include "doom_nukem.h"

void	clear_screen(t_app *app)
{
	image_clear(app->sdl->surface->pixels, 0, SCREEN_W * SCREEN_H * 4);
}

int 	tr_cmpr(const void *p, const void *q)
{
	t_triangle	t1;
	t_triangle	t2;
	float		z1;
	float		z2;

	t1 = *(t_triangle *)p;
	t2 = *(t_triangle *)q;
	z1 = (t1.v[0].z + t1.v[1].z + t1.v[2].z) / 3.0f;
	z2 = (t2.v[0].z + t2.v[1].z + t2.v[2].z) / 3.0f;
	return (z1 > z2);
}

void	draw_mesh(t_app *app, t_mesh *mesh)
{
	t_triangle	tr;
	int			t_idx;
	int			tr_idx;
	t_triangle	to_render[5000];

	tr_idx = 0;
	t_idx = 0;

	while (t_idx < mesh->t_idx)
	{
		tr = check_triangle(app, mesh->t[t_idx]);
		if (tr.visible)
		{
			to_render[tr_idx] = tr;
			tr_idx++;
		}
		t_idx++;
	}
	qsort((void*)to_render, tr_idx, sizeof(t_triangle), tr_cmpr);
	tr_idx--;
	while (tr_idx >= 0)
	{
		render_triangle(app, to_render[tr_idx]);
		tr_idx--;
	}
}

void	start_the_game(t_app *app)
{
	while (1)
	{
		get_ticks(app->sdl->timer);
		clear_screen(app);
		if (!event_handling(app))
			break;

		app->rot.x += app->speed * app->sdl->timer->delta_ticks * CLOCK_FIX;
		app->rot.z += app->speed * app->sdl->timer->delta_ticks * CLOCK_FIX;

		update_rotation_mat_z(app, app->rot.z);
		update_rotation_mat_x(app, app->rot.x);

		draw_mesh(app, &app->mesh[0]);

		SDL_UpdateWindowSurface(app->sdl->window);

		get_delta_time(app->sdl->timer);
	}
	SDL_Quit();
	SDL_DestroyWindow(app->sdl->window);
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

