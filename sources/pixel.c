#include "doom_nukem.h"

void	set_pixel_uint32(SDL_Surface *surface, int offset, Uint32 c)
{
	if (offset < 0 || offset >= SCREEN_W * SCREEN_H)
		return ;
	((Uint32 *)surface->pixels)[offset] = c;
}
