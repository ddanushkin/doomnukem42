#include "doom_nukem.h"

void	clear_screen(t_app *app)
{
	ft_bzero(app->screen.pixels,app->window.w * app->window.h * 4);
}

int		update(t_app *app)
{
	t_mesh	cube;
	do_input(app);
	make_cube(&cube);

	update_rotation_mat_z(app, app->rot.z);
	update_rotation_mat_x(app, app->rot.x);

	clear_screen(app);

	set_color(&cube.t[0].color, 255, 0, 0);
	draw_triangle(app, cube.t[0]);
	set_color(&cube.t[1].color, 0, 255, 0);
	draw_triangle(app, cube.t[1]);
	set_color(&cube.t[2].color, 0, 0, 255);
	draw_triangle(app, cube.t[2]);
	set_color(&cube.t[3].color, 0, 255, 255);
	draw_triangle(app, cube.t[3]);
	set_color(&cube.t[4].color, 255, 255, 0);
	draw_triangle(app, cube.t[4]);
	set_color(&cube.t[5].color, 255, 0, 255);
	draw_triangle(app, cube.t[5]);

	set_color(&cube.t[6].color, 255, 0, 0);
	draw_triangle(app, cube.t[6]);
	set_color(&cube.t[7].color, 0, 255, 0);
	draw_triangle(app, cube.t[7]);
	set_color(&cube.t[8].color, 0, 0, 255);
	draw_triangle(app, cube.t[8]);
	set_color(&cube.t[9].color, 0, 255, 255);
	draw_triangle(app, cube.t[9]);

	mlx_put_image_to_window(app->mlx, app->window.ptr, app->screen.ptr, 0, 0);
	reset_inputs_states(app);
	return (0);
}

int		main()
{
	t_app	app;

	init_app(&app);
	mlx_loop_hook(app.mlx, update, &app);
	mlx_loop(app.mlx);
}