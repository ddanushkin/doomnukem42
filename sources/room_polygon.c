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
	/*TODO: Fix malloc size!*/
	cs->triangles = (t_triangle *)malloc(sizeof(t_triangle) * (size - 2));
	cs->triangles_count = 0;
}

void 	get_floor_poly(t_sector *cs)
{
	int		i;
	t_v3d	*polygon;
	int		next_wall;

	polygon = (t_v3d *)malloc(sizeof(t_v3d) * cs->walls_count);
	polygon[0] = cs->walls[0].v[0];
	polygon[1] = cs->walls[0].v[2];
	i = 1;
	next_wall = 0;
	while (i < cs->walls_count - 1)
	{
		next_wall = find_linked_wall(cs, polygon[i], next_wall);
		if (next_wall != 0)
		{
			if (compare_vertex(&polygon[i], &cs->walls[next_wall].v[0]))
				polygon[i + 1] = cs->walls[next_wall].v[2];
			else if(compare_vertex(&polygon[i], &cs->walls[next_wall].v[2]))
				polygon[i + 1] = cs->walls[next_wall].v[0];
		}
		i++;
	}
	polygon_to_list(cs, polygon, i + 1);
	triangulate(cs);
	free(polygon);
}