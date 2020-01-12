#include "doom_nukem.h"

void 	draw_grid_point(t_app *app, t_v3d *gp)
{
	t_v3d	tmp;

	tmp = matrix_transform(app->camera->transform, *gp);
	if (vertex_inside(&tmp))
	{
		tmp = matrix_transform(app->camera->screen_space, tmp);
		vertex_perspective_divide(&tmp);
		int offset = (int)tmp.y * SCREEN_W + (int)tmp.x;
		if (tmp.z < app->depth_buffer[offset])
			set_pixel_uint32(app->sdl->surface, offset, 0xffffff);
	}
	gp->x += app->grid_size;
}

void 	draw_grid(t_app *app)
{
	int		x;
	int		z;
	t_v3d	grid_point;

	x = 100;
	z = 100;
	grid_point.y = app->edit_wall.v[2].y;
	grid_point.x = app->edit_wall.v[2].x - 50.0 * app->grid_size;
	grid_point.z = app->edit_wall.v[2].z - 50.0 * app->grid_size;
	grid_point.w = app->edit_wall.v[2].w;
	while (z > 0)
	{
		while (x > 0)
		{
			draw_grid_point(app, &grid_point);
			x--;
		}
		x = 100;
		grid_point.x = app->edit_wall.v[2].x - 50.0 * app->grid_size;
		grid_point.z += app->grid_size;
		z--;
	}
}