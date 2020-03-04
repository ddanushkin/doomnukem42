#include "doom_nukem.h"

void 	print_to_screen(t_app *app, int x, int y, char *text)
{
	SDL_Color	*color;
	SDL_Surface	*surface;
	SDL_Rect	rect;

	color = (SDL_Color *)(&app->font_color);
	surface = TTF_RenderText_Solid(app->font, text, *color);
	rect.x = x;
	rect.y = y;
	rect.w = surface->w;
	rect.h = surface->h;
	SDL_BlitSurface(surface, &surface->clip_rect, app->sdl->surface, &rect);
	SDL_FreeSurface(surface);
}

void 	draw_cross(t_app *app, int x, int y, double size, Uint32 color)
{
	int	len;
	int	i;
	int start_x;
	int start_y;

	if (size <= 0)
		return ;
	i = 0;
	len = (int)(64.0 / size);
	len = CLAMP(len, 2, 32);
	start_x = x - (int)(len * 0.5);
	start_y = y - (int)(len * 0.5);
	while (i <= len)
	{
		set_pixel_uint32(app->sdl->surface, (start_y + i) * SCREEN_W + x, color);
		set_pixel_uint32(app->sdl->surface, y * SCREEN_W + (start_x + i), color);
		i++;
	}
}
