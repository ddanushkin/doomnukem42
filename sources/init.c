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

static	void	init_sdl(t_sdl *sdl)
{
	float	fov;

	fov = M_PI / 6.0;
	sdl->width = 1280;
	sdl->half_width = (int)(sdl->width * 0.5);
	sdl->height = 720;
	sdl->half_height = (int)(sdl->height * 0.5);
	sdl->dist_to_pp = (int)((float)sdl->half_width / tanf(fov));
	sdl->draw_dist = 840;
	sdl->pixels = (Uint32 *)malloc(sizeof(Uint32) * sdl->width * sdl->height);
	sdl->dist_per_x = (float *)malloc(sizeof(float) * sdl->width);
}

static	void	create_stuff(t_sdl *sdl)
{
	SDL_Init(SDL_INIT_VIDEO);

	sdl->window = SDL_CreateWindow(WIN_TITLE, SDL_WINDOWPOS_CENTERED,
								   SDL_WINDOWPOS_CENTERED, SCREEN_W, SCREEN_H, 0);
	sdl->surface = SDL_GetWindowSurface(sdl->window);
	sdl->pixels = (Uint32 *)malloc(sizeof(Uint32) * SCREEN_W * SCREEN_H);

}

void	init_app(t_app *app)
{

	ft_bzero(app, sizeof(t_app));
	app->sdl = (t_sdl *)malloc(sizeof(t_sdl));
	init_sdl(app->sdl);
	create_stuff(app->sdl);
	app->inputs = (t_inputs *)malloc(sizeof(t_inputs));
	app->camera.fov = 90.0f;
	app->camera.z_far = 1000.0f;
	app->camera.z_near = 0.1f;
	app->camera.asp_ratio = (float)app->window.h / (float)app->window.w;
	app->camera.for_rad = 1.0f / tanf(app->camera.fov * 0.5f / 180.0f * 3.14159f);
	set_vertex(&app->camera.pos, 0.0f, 0.0f, 0.0f);
	app->speed = 0.01f;
	init_projection_mat(app);
}