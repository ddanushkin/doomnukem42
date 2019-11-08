#include "doom_nukem.h"

t_color 		sprite_get_color_by_offset(t_sprite *sprite, int offset)
{
	return (color_new(
			sprite->pixels[offset],
			sprite->pixels[offset + 1],
			sprite->pixels[offset + 2]));
}

void			sprite_reverse(t_sprite *sprite, t_bmp_header h)
{
	int		row;
	int		si;
	int		ei;
	t_color	sc;
	t_color	ec;

	row = 0;
	while (row < h.height_px)
	{
		si = row * h.width_px * 3;
		ei = si + h.width_px * 3 - 1;
		while (si < ei)
		{
			sc = sprite_get_color_by_offset(sprite, si);
			ec = sprite_get_color_by_offset(sprite, ei - 2);
			sprite->pixels[si] = ec.r;
			sprite->pixels[si + 1] = ec.g;
			sprite->pixels[si + 2] = ec.b;
			sprite->pixels[ei - 2] = sc.r;
			sprite->pixels[ei - 1] = sc.g;
			sprite->pixels[ei] = sc.b;
			si += 3;
			ei -= 3;
		}
		row++;
	}
}

void			bpm_read_pixels(int fd, t_bmp_header h, t_sprite *sprite)
{
	size_t	len;

	len = h.width_px * h.height_px;
	sprite->header = h;
	sprite->pixels = (uint32_t *)malloc(len * sizeof(uint32_t));
	read(fd, sprite->pixels, len * sizeof(uint32_t));
}

t_bmp_header	bmp_read_header(int fd)
{
	t_bmp_header	h;

	ft_memset(&h, 0, sizeof(h));
	read(fd, &(h.type), sizeof(h.type));
	read(fd, &(h.size), sizeof(h.size));
	read(fd, &(h.reserved1), sizeof(h.reserved1));
	read(fd, &(h.reserved2), sizeof(h.reserved2));
	read(fd, &(h.offset), sizeof(h.offset));
	read(fd, &(h.dib_header_size), sizeof(h.dib_header_size));
	read(fd, &(h.width_px), sizeof(h.width_px));
	read(fd, &(h.height_px), sizeof(h.height_px));
	read(fd, &(h.num_planes), sizeof(h.num_planes));
	read(fd, &(h.bits_per_pixel), sizeof(h.bits_per_pixel));
	read(fd, &(h.compression), sizeof(h.compression));
	read(fd, &(h.image_size_bytes), sizeof(h.image_size_bytes));
	read(fd, &(h.x_resolution_ppm), sizeof(h.x_resolution_ppm));
	read(fd, &(h.y_resolution_ppm), sizeof(h.y_resolution_ppm));
	read(fd, &(h.num_colors), sizeof(h.num_colors));
	read(fd, &(h.important_colors), sizeof(h.important_colors));
	return (h);
}

void	bmp_load(char *path, t_sprite *sprite)
{
	int				fd;

	t_bmp_header	header;
	fd = open(path, O_RDONLY);
	if (fd < 3)
		exit_with_status(STATUS_FILE_NOT_FOUND, path);
	header = bmp_read_header(fd);
	bpm_read_pixels(fd, header, sprite);
	close(fd);
}