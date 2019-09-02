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

void	set_pixel(t_image *image, int x, int y, t_color *c)
{
	int		index;

	if (out_of_borders(x, y) || color_key(c))
		return ;
	index = x * 4 + image->s_l * y;
	image->pixels[index] = c->b;
	image->pixels[index + 1] = c->g;
	image->pixels[index + 2] = c->r;
}