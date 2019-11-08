#include "doom_nukem.h"

int 	out_of_borders(int x, int y)
{
	if (x < 0 || x > REAL_W - 1)
		return (1);
	if (y < 0 || y > REAL_H - 1)
		return (1);
	return (0);
}

int 	color_key(t_color c)
{
	if (c.r == COLOR_KEY_R &&
		c.g == COLOR_KEY_G &&
		c.b == COLOR_KEY_B)
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
	((Uint32 *)surface->pixels)[offset] = c;
}

uint32_t	sprite_get_color(t_sprite *s, int x, int y)
{
	return (s->pixels[y * s->header.width_px + x]);
}

uint32_t	sprite_get_color_by_uv(t_sprite *s, double u, double v)
{
	t_bmp_header	*header;
	int 			h;
	int 			w;

	header = &s->header;
	h = header->height_px - 1;
	w = header->width_px - 1;
	return (sprite_get_color(s, w * u, h * v));
}

//void	sprite_draw(SDL_Surface *screen, t_sprite *sprite, int x, int y, int size_x, int size_y)
//{
//	int		i;
//	int		j;
//	t_color	c;
//
//	i = 0;
//	size_x = abs(size_x);
//	size_y = abs(size_y);
//	while (i < size_x)
//	{
//		j = 0;
//		while (j < size_y)
//		{
//			double u = (double)i / (double)size_x;
//			double v = (double)j / (double)size_y;
//			c = sprite_get_color_by_uv(sprite, u, v);
//			set_pixel(screen, i + x, j + y, c);
//			j++;
//		}
//		i++;
//	}
//}