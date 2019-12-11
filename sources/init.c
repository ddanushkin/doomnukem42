#include "doom_nukem.h"

void	init_projection_mat(t_app *app)
{
	t_camera *camera;

	camera = app->camera;
	ft_bzero(&app->projection_mat, sizeof(t_mat4x4));
	app->projection_mat.m[0][0] = camera->asp_ratio * camera->for_rad;
	app->projection_mat.m[1][1] = camera->for_rad;
	app->projection_mat.m[2][2] = camera->z_far / (camera->z_far - camera->z_near);
	app->projection_mat.m[3][2] = (-camera->z_far * camera->z_near) / (camera->z_far - camera->z_near);
	app->projection_mat.m[2][3] = 1.0;
	app->projection_mat.m[3][3] = 0.0;
}

static	void	init_sdl(t_sdl *sdl)
{
	sdl->width = SCREEN_W;
	sdl->half_width = (double)sdl->width * 0.5;
	sdl->height = SCREEN_H;
	sdl->half_height = (double)sdl->height * 0.5;
	SDL_Init(SDL_INIT_VIDEO);
	sdl->window = SDL_CreateWindow(
			WIN_TITLE,
			SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED,
			sdl->width,
			sdl->height,
			0);
	sdl->surface = SDL_GetWindowSurface(sdl->window);
}

void	init_camera(t_camera *camera)
{
	camera->fov = 30.0;
	camera->z_far = 1000.0;
	camera->z_near = 0.1;
	camera->asp_ratio = (double)SCREEN_H / (double)SCREEN_W;
	camera->for_rad = 1.0 / tanf(camera->fov * 0.5 / 180.0 * 3.14159);
	set_vector(&camera->pos, 0.0, 0.0, 0.0);
	set_vector(&camera->rot, 0.0, 0.0, 0.0);
}

void	prepare_chunks(t_app *app)
{
	int		i;
	double	*depth_chunk;
	Uint32	*screen_chunk;

	i = 0;
	depth_chunk = (double *)&app->depth_chunk;
	screen_chunk = (Uint32 *)&app->screen_chunk;
	while (i++ < SCREEN_W)
	{
		*depth_chunk++ = 100000.0;
		*screen_chunk++ = 0;
	}
	app->depth_chunk_array = (t_depth_chunk *)app->depth_buffer;
	app->screen_chunk_array = (t_screen_chunk *)app->sdl->surface->pixels;
}

void	init_app(t_app *app)
{
	init_sdl(app->sdl);
	app->inputs->keyboard = SDL_GetKeyboardState(NULL);
	app->inputs->mouse.sens = 0.5;
	app->timer->curr = 0;
	app->timer->delta = 0;
	app->timer->fps = 0;
	app->timer->delta = 0.0;
	app->timer->time = 0.0;
	init_camera(app->camera);
	init_projection_mat(app);
	prepare_chunks(app);
}