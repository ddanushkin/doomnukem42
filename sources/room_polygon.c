#include "doom_nukem.h"

double 	calc_tex(double min, double cur, double max)
{
	return ((cur - min)/(max - min));
}

void	push_vertex(t_sector *cs, t_polygon **polygon, t_v3d *v)
{
	v->tex_x = calc_tex(cs->x_min, v->x, cs->x_max);
	v->tex_y = calc_tex(cs->z_min, v->z, cs->z_max);
	polygon_add(polygon, *v);
}

t_polygon	*points_to_list(t_sector *cs, t_v3d *points, int size)
{
	double		or;
	int			i;
	t_polygon	*polygon;

	polygon = NULL;
	or = get_orientation(&points[0], size);
	if (or > 0)
	{
		i = 0;
		while (i < size)
			push_vertex(cs, &polygon, &points[i++]);
	}
	else if (or < 0)
	{
		i = size - 1;
		while (i >= 0)
			push_vertex(cs, &polygon, &points[i--]);
	}
	cs->triangles_count = 0;
	return (polygon);
}