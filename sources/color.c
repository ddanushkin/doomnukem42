#include "doom_nukem.h"

t_color	color_new(int r, int g, int b)
{
	t_color c;

	c.r = r;
	c.g = g;
	c.b = b;
	return (c);
}

t_color	color_sub(t_color color, int k)
{
	color.r -= k;
	color.g -= k;
	color.b -= k;
	return (color);
}