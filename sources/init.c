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
	sdl->height = SCREEN_H;
	SDL_Init(SDL_INIT_VIDEO);
	TTF_Init();
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
	camera->fov = TO_RAD(60.0);
	camera->z_far = 1000.0;
	camera->z_near = 0.1;
	camera->asp_ratio = (double)SCREEN_H / (double)SCREEN_W;
	camera->for_rad = 1.0 / tan(camera->fov * 0.5 / 180.0 * 3.14159);
	camera->pos = new_vector(1.0, 1.0, -10);
	camera->rot = new_vector(0.0, 0.0, 0.0);
	camera->projection = matrix_perspective(
			camera->fov,
			(double)SCREEN_W / (double)SCREEN_H,
			0.05,
			1000.0);
	camera->screen_space = matrix_screen_space();
}

void	init_app(t_app *app)
{
	init_sdl(app->sdl);
	app->inputs->keyboard = SDL_GetKeyboardState(NULL);
	app->inputs->mouse.sens = 0.5;
	app->timer->prev = 0;
	app->timer->delta = 0;
	app->timer->fps = 0;
	app->timer->delta = 0.0;
	app->timer->time = 0.0;
	app->timer->frame = 0;
	app->edge_selected = 0;
	app->input_g = 0;
	app->input_minus = 0;
	app->input_plus = 0;
	app->inputs->mouse.left = 0;
	app->inputs->mouse.right = 0;
	app->inputs->mouse.middle = 0;
	app->grid_size = 0.5;
	app->triangles_counter = 0;
	init_camera(app->camera);
	init_projection_mat(app);
	//prepare_chunks(app);
	app->font = TTF_OpenFont("resources/calibrib.ttf", 14);
}