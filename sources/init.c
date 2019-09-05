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

static	void	init_sdl(t_sdl *sdl)
{
	sdl->width = SCREEN_W;
	sdl->half_width = (int)(sdl->width * 0.5);
	sdl->height = SCREEN_H;
	sdl->half_height = (int)(sdl->height * 0.5);
}

static	void	create_stuff(t_sdl *sdl)
{
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

void	init_app(t_app *app)
{
	app = (t_app *)malloc(sizeof(t_app));
	ft_bzero(app, sizeof(t_app));
	app->sdl = (t_sdl *)malloc(sizeof(t_sdl));
	app->sdl->timer = (t_timer *)malloc(sizeof(t_timer));
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