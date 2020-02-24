#include "doom_nukem.h"

void			bpm_read_pixels(int fd, t_bmp_header h, t_sprite *sprite)
{
	size_t	len;

	len = h.width_px * h.height_px;
	sprite->header = h;
	read(fd, &sprite->pixels[0], len * sizeof(uint32_t));
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

void	bmp_load(t_app *app, char *path)
{
	int				fd;

	t_bmp_header	header;
	fd = open(path, O_RDONLY);
	if (fd < 3)
		exit_with_status(STATUS_FILE_NOT_FOUND, path);
	header = bmp_read_header(fd);
	bpm_read_pixels(fd, header, &app->sprites[app->sprites_count]);
	close(fd);
	app->sprites_count++;
}
