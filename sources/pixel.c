#include "doom_nukem.h"

int 	out_of_borders(int x, int y)
{
	if (x < 0 || x > SCREEN_W - 1)
		return (1);
	if (y < 0 || y > SCREEN_H - 1)
		return (1);
	return (0);
}

int 	color_key(t_color *c)
{
	if (c->r == COLOR_KEY_R &&
		c->g == COLOR_KEY_G &&
		c->b == COLOR_KEY_B)
		return (1);
	return (0);
}

void	set_pixel(SDL_Surface *surface, int x, int y, t_color *c, t_app *app)
{
	int		offset;

	if (out_of_borders(x, y) || color_key(c))
		return ;
	int		index_z;

	index_z = y * SCREEN_W + x;
	offset = 4 * (y * surface -> w + x);
//	if (app->z_buf[index_z] > app->depth)
//	{
//		app->z_buf[index_z] = app->depth;
		unsigned char *pixels = (unsigned char *) surface->pixels;
		pixels[offset] = c->b;
		pixels[offset + 1] = c->g;
		pixels[offset + 2] = c->r;
//	}
}