#include "doom_nukem.h"

void	set_vector(t_vector *v, float x, float y, float z)
{
	v->x = x;
	v->y = y;
	v->z = z;
}

void	set_color(t_color *color, int r, int g, int b)
{
	color->r = r;
	color->g = g;
	color->b = b;
}

void	clear_screen(t_app *app)
{
	ft_bzero(app->screen.pixels,app->window.w * app->window.h * 4);
}

void	make_cube(t_mesh *mesh)
{
	//FRONT PLANE
	//
	set_vector(&mesh->t[0].v[0], 0, 0, 0);
	set_vector(&mesh->t[0].v[1], 1, 0, 0);
	set_vector(&mesh->t[0].v[2], 0, 1, 0);
	//
	set_vector(&mesh->t[1].v[0], 1, 0, 0);
	set_vector(&mesh->t[1].v[1], 1, 1, 0);
	set_vector(&mesh->t[1].v[2], 0, 1, 0);

	//RIGHT PLANE
	//
	set_vector(&mesh->t[2].v[0], 1, 0, 0);
	set_vector(&mesh->t[2].v[1], 1, 0, 1);
	set_vector(&mesh->t[2].v[2], 1, 1, 0);
	//
	set_vector(&mesh->t[3].v[0], 1, 0, 1);
	set_vector(&mesh->t[3].v[1], 1, 1, 1);
	set_vector(&mesh->t[3].v[2], 1, 1, 0);

	//BACK PLANE
	//
	set_vector(&mesh->t[4].v[0], 1, 0, 1);
	set_vector(&mesh->t[4].v[1], 0, 0, 1);
	set_vector(&mesh->t[4].v[2], 1, 1, 1);
	//
	set_vector(&mesh->t[5].v[0], 0, 0, 1);
	set_vector(&mesh->t[5].v[1], 0, 1, 1);
	set_vector(&mesh->t[5].v[2], 1, 1, 1);

	//LEFT PLANE
	//
	set_vector(&mesh->t[6].v[0], 0, 0, 1);
	set_vector(&mesh->t[6].v[1], 0, 0, 0);
	set_vector(&mesh->t[6].v[2], 0, 1, 1);
	//
	set_vector(&mesh->t[7].v[0], 0, 0, 0);
	set_vector(&mesh->t[7].v[1], 0, 1, 0);
	set_vector(&mesh->t[7].v[2], 0, 1, 1);

	//TOP PLANE
	//
	set_vector(&mesh->t[8].v[0], 0, 0, 1);
	set_vector(&mesh->t[8].v[1], 1, 0, 1);
	set_vector(&mesh->t[8].v[2], 0, 0, 0);
	//
	set_vector(&mesh->t[9].v[0], 1, 0, 1);
	set_vector(&mesh->t[9].v[1], 1, 0, 0);
	set_vector(&mesh->t[9].v[2], 0, 0, 0);

	//BOTTOM PLANE
	//
	set_vector(&mesh->t[10].v[0], 0, 0, 0);
	set_vector(&mesh->t[10].v[1], 0, 0, 0);
	set_vector(&mesh->t[10].v[2], 0, 0, 0);
	//
	set_vector(&mesh->t[11].v[0], 0, 0, 0);
	set_vector(&mesh->t[11].v[1], 0, 0, 0);
	set_vector(&mesh->t[11].v[2], 0, 0, 0);
}

int 	update(t_app *app)
{
	t_mesh	cube;
	t_color	color;
	update_inputs(app);
	make_cube(&cube);

	update_rotation_mat_z(app, app->rot.z);
	update_rotation_mat_x(app, app->rot.x);

	clear_screen(app);

	set_color(&color, 255, 0, 0);
	draw_triangle(app, cube.t[0], &color);
	set_color(&color, 0, 255, 0);
	draw_triangle(app, cube.t[1], &color);
	set_color(&color, 0, 0, 255);
	draw_triangle(app, cube.t[2], &color);
	set_color(&color, 255, 0, 0);
	draw_triangle(app, cube.t[3], &color);
	set_color(&color, 0, 255, 0);
	draw_triangle(app, cube.t[4], &color);
	set_color(&color, 0, 0, 255);
	draw_triangle(app, cube.t[5], &color);
	set_color(&color, 255, 0, 0);
	draw_triangle(app, cube.t[6], &color);
	set_color(&color, 0, 255, 0);
	draw_triangle(app, cube.t[7], &color);
	set_color(&color, 0, 0, 255);
	draw_triangle(app, cube.t[8], &color);
	set_color(&color, 255, 0, 0);
	draw_triangle(app, cube.t[9], &color);

	mlx_put_image_to_window(app->mlx, app->window.ptr, app->screen.ptr, 0, 0);
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