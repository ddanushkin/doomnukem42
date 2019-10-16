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
	app->projection_mat.m[2][3] = 1.0f;
	app->projection_mat.m[3][3] = 0.0f;
}

static	void	init_sdl(t_sdl *sdl)
{
	sdl->width = SCREEN_W;
	sdl->half_width = (float)sdl->width * 0.5f;
	sdl->height = SCREEN_H;
	sdl->half_height = (float)sdl->height * 0.5f;
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
	camera->fov = 78.0f;
	camera->z_far = 1000.0f;
	camera->z_near = 0.1f;
	camera->asp_ratio = (float)SCREEN_H / (float)SCREEN_W;
	camera->for_rad = 1.0f / tanf(camera->fov * 0.5f / 180.0f * 3.14159f);
	set_vector(&camera->pos, 0.0f, 0.0f, 0.0f);
	set_vector(&camera->rot, 0.0f, 0.0f, 0.0f);
}

void	init_app(t_app *app)
{
	init_sdl(app->sdl);
	app->inputs->keyboard = SDL_GetKeyboardState(NULL);
	app->inputs->mouse.sens = 0.5f;
	app->timer->curr = 0;
	app->timer->delta = 0;
	app->timer->fps = 0;
	app->timer->delta = 0.0f;
	app->timer->time = 0.0f;
	ft_memset(app->z_buf, 120, sizeof(double) * SCREEN_H * SCREEN_W);
	init_camera(app->camera);
	init_projection_mat(app);
}