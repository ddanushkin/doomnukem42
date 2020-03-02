#include "doom_nukem.h"

void	points_add_check(t_v3d *points, int *size)
{
	int		i;
	int		len;

	if (*size < 3)
		return ;
	i = 0;
	len = *size - 2;
	while (i < len)
	{
		if (line_intersection(
				points[i],
				points[i + 1],
				points[*size - 2],
				points[*size - 1]))
		{
			*size = *size - 1;
			return ;
		}
		i++;
	}
}

void 	point_draw(t_app *app, t_v3d p, Uint32 c)
{
	p = matrix_transform(app->camera->transform, p);
	if (vertex_inside(&p))
	{
		p = matrix_transform(app->camera->screen_space, p);
		vertex_perspective_divide(&p);
		int offset = (int)p.y * SCREEN_W + (int)p.x;
		if (p.z < app->depth_buffer[offset])
			set_pixel_uint32(app->sdl->surface, offset, c);
	}
}

t_v3d	point_2d_to_3d(t_app *app, double x, double z, int grid)
{
	double angle;

	angle = tan(0.5 * app->camera->fov) * app->camera->pos.y;
	x = (2.0 * x / SCREEN_W - 1) * angle / app->camera->asp_ratio;
	z = (2.0 * z / SCREEN_H - 1) * angle;
	if (grid)
	{
		x = round((app->camera->pos.x + x) / app->grid_size) * app->grid_size;
		z = round((app->camera->pos.z - z) / app->grid_size) * app->grid_size;
	}
	else
	{
		x = (app->camera->pos.x + x);
		z = (app->camera->pos.z - z);
	}
	return (new_vector(x, 0.0, z));
}
