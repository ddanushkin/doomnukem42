#include "doom_nukem.h"

double 	calc_tex(double min, double cur, double max)
{
	return ((cur - min)/(max - min));
}

void	push_vertex(t_sector *cs, t_v3d *v)
{
	v->tex_x = calc_tex(cs->x_min, v->x, cs->x_max);
	v->tex_y = calc_tex(cs->z_min, v->z, cs->z_max);
	polygon_add(&cs->polygon, *v);
}

void 	polygon_to_list(t_sector *cs, t_v3d *polygon, int size)
{
	double	or;
	int		i;

	or = get_orientation(&polygon[0], size);
	if (or > 0)
	{
		i = 0;
		cs->polygon = NULL;
		while (i < size)
			push_vertex(cs , &polygon[i++]);
	}
	else if (or < 0)
	{
		i = size - 1;
		cs->polygon = NULL;
		while (i >= 0)
			push_vertex(cs , &polygon[i--]);
	}
	cs->triangles = (t_triangle *)malloc(sizeof(t_triangle) * (size - 2));
	cs->triangles_count = 0;
}