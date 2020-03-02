#include "doom_nukem.h"

void	init_projection_mat(t_app *app)
{
	t_camera *camera;

	camera = app->camera;
	ft_bzero(&app->projection_mat, sizeof(t_mat4x4));
	app->projection_mat.m[0] = camera->asp_ratio * camera->for_rad;
	app->projection_mat.m[5] = camera->for_rad;
	app->projection_mat.m[10] = camera->z_far / (camera->z_far - camera->z_near);
	app->projection_mat.m[14] = (-camera->z_far * camera->z_near) / (camera->z_far - camera->z_near);
	app->projection_mat.m[11] = 1.0;
	app->projection_mat.m[15] = 0.0;
}

static	void	init_sdl(t_sdl *sdl)
{
	sdl->width = SCREEN_W;
	sdl->height = SCREEN_H;
	SDL_Init(SDL_INIT_AUDIO | SDL_INIT_VIDEO);
	TTF_Init();
	Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096);
	Mix_AllocateChannels(16);
	Mix_VolumeMusic(MIX_MAX_VOLUME/8);
	sdl->window = SDL_CreateWindow(
			WIN_TITLE,
			SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED,
			sdl->width,
			sdl->height,
			SDL_WINDOW_OPENGL);
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
	camera->fly = 0;
}

void 	init_font(t_app *app)
{
	SDL_RWops	*raw;

	raw = SDL_RWFromMem(&app->fonts.mem[0], app->fonts.size);
	app->font = TTF_OpenFontRW(raw, 1, 16);
}

void 	init_sfx(t_app *app)
{
	SDL_RWops	*raw;
	int 		i;

	i = 0;
	app->sfx = (Mix_Chunk **)malloc(sizeof(Mix_Chunk *) * MAX_SFX);
	while (i < MAX_SFX)
	{
		raw = SDL_RWFromMem(&app->audio[i].mem[0], app->audio[i].size);
		app->sfx[i] = Mix_LoadWAV_RW(raw, 1);
		i++;
	}
}

void 	init_bg(t_app *app)
{
	SDL_RWops	*raw;
	int 		i;

	i = 0;
	app->bg = (Mix_Music **)malloc(sizeof(Mix_Music *) * MAX_BG);
	while (i < MAX_BG)
	{
		raw = SDL_RWFromMem(&app->music[i].mem[0], app->music[i].size);
		app->bg[i] = Mix_LoadMUSType_RW(raw, MUS_OGG, 1);
		i++;
	}
}

void 	init_skybox(t_app *app)
{
	double size;

	size = 100.0;
	app->skybox.v[0] = new_vector(-size, -size, size);
	app->skybox.v[1] = new_vector(size, size, size);
	app->skybox.v[2] = new_vector(size, -size, size);
	app->skybox.v[3] = new_vector(-size, size, size);
	app->skybox.v[4] = new_vector(-size, -size, -size);
	app->skybox.v[5] = new_vector(size, size, -size);
	app->skybox.v[6] = new_vector(size, -size, -size);
	app->skybox.v[7] = new_vector(-size, size, -size);
}

void	init_app(t_app *app)
{
	init_sdl(app->sdl);
	app->screen = app->sdl->surface->pixels;
	app->inputs->keyboard = SDL_GetKeyboardState(NULL);
	app->timer->prev = 0;
	app->timer->delta = 0;
	app->timer->fps = 0;
	app->timer->delta = 0.0;
	app->timer->time = 0.0;
	app->timer->frame = 0;
	app->grid_size = 0.5;
	app->triangles_counter = 0;
	init_camera(app->camera);
	init_projection_mat(app);
	init_font(app);
	init_sfx(app);
	init_bg(app);
	init_skybox(app);
}
