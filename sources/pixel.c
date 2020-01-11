#include "doom_nukem.h"

int 	out_of_borders(int x, int y)
{
	if (x < 0 || x > SCREEN_H - 1)
		return (1);
	if (y < 0 || y > SCREEN_H - 1)
		return (1);
	return (0);
}

void	set_pixel(SDL_Surface *surface, int x, int y, t_color c)
{
	int				offset;
	unsigned char	*pixels;
	if (out_of_borders(x, y))
		return ;
	offset = 4 * (y * surface->w + x);
	pixels = (unsigned char *)surface->pixels;
	pixels[offset] = c.b;
	pixels[offset + 1] = c.g;
	pixels[offset + 2] = c.r;
}

void	set_pixel_uint32(SDL_Surface *surface, int offset, Uint32 c)
{
	if (offset < 0 || offset >= SCREEN_W * SCREEN_H)
		return ;
	((Uint32 *)surface->pixels)[offset] = c;
}

uint32_t	sprite_get_color(t_sprite *s, int offset)
{
	return (s->pixels[offset]);
}
