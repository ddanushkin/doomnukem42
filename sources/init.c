#include "doom_nukem.h"

void	init_projection_mat(t_app *app)
{
	t_camera *camera;

	camera = &app->camera;
	ft_bzero(&app->projection_mat, sizeof(t_mat4x4));
	app->projection_mat.m[0][0] = camera->asp_ratio * camera->for_rad;
	app->projection_mat.m[1][1] = camera->for_rad;
	app->projection_mat.m[2][2] = camera->z_far / (camera->z_far - camera->z_near);
	app->projection_mat.m[3][2] = (-camera->z_far * camera->z_near) / (camera->z_far - camera->z_near);
	app->projection_mat.m[2][3] = 1.0f;
	app->projection_mat.m[3][3] = 0.0f;
}

void	init_window(t_window *window, void *mlx)
{
	window->w = SCREEN_W;
	window->h = SCREEN_H;
	window->half_w = window->w * 0.5f;
	window->half_h = window->h * 0.5f;
	window->title = ft_strdup(WIN_TITLE);
	window->ptr = mlx_new_window(
			mlx,
			window->w,
			window->h,
			window->title);
}

void	init_image(t_image *image, t_window *window, void *mlx)
{
	image->ptr = mlx_new_image(
			mlx,
			window->w,
			window->h);
	image->pixels = mlx_get_data_addr(
			image->ptr,
			&(image->bpp),
			&(image->s_l),
			&(image->endian));
}

void	init_app(t_app *app)
{
	ft_bzero(app, sizeof(t_app));
	app->mlx = mlx_init();
	init_window(&app->window, app->mlx);
	init_image(&app->screen, &app->window, app->mlx);
	app->camera.fov = 90.0f;
	app->camera.z_far = 1000.0f;
	app->camera.z_near = 0.1f;
	app->camera.asp_ratio = (float)app->window.h / (float)app->window.w;
	app->camera.for_rad = 1.0f / tanf(app->camera.fov * 0.5f / 180.0f * 3.14159f);
	set_vertex(&app->camera.pos, 0.0f, 0.0f, 0.0f);
	app->speed = 0.01f;
	init_projection_mat(app);
}