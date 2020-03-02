#include "doom_nukem.h"

int 	point_in_screen(t_v3d p)
{
	return ((int)p.x >= 0 && (int)p.x < SCREEN_W &&
			(int)p.y >= 0 && (int)p.y < SCREEN_H);
}

void	set_pixel_uint32(SDL_Surface *surface, int offset, Uint32 c)
{
	if (offset < 0 || offset >= SCREEN_W * SCREEN_H)
		return ;
	((Uint32 *)surface->pixels)[offset] = c;
}
