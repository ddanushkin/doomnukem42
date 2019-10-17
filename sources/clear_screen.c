#include "doom_nukem.h"

static	void	image_chunk(void *b, int c, size_t len)
{
	unsigned char	*dst_small;

	dst_small = b;
	while (len > 0)
	{
		*dst_small = (unsigned char)c;
		dst_small++;
		len--;
	}
}

void	*image_clear(void *b, int c, size_t len)
{
	unsigned char *dst_small;
	t_image_chunk *dst_big;
	t_image_chunk zero;

	if (len >= 1024)
		image_chunk(&zero, c, 1024);
	dst_big = b;
	while (len >= 1024)
	{
		*dst_big = zero;
		dst_big++;
		len -= 1024;
	}

	dst_small = (void *) dst_big;
	while (len > 0)
	{
		*dst_small = (unsigned char) c;
		dst_small++;
		len--;
	}
	return (b);
}
