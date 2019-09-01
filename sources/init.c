#include "doom_nukem.h"

void	init_window(t_window *window, void *mlx)
{
	window->w = SCREEN_W;
	window->h = SCREEN_H;
	window->half_w = window->w * 0.5f;
	window->half_h = window->h * 0.5f;
	window->ratio = (float)window->w / (float)window->h;
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
}