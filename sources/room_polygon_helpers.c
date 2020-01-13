#include "doom_nukem.h"

int 	compare_vertex(t_v3d *v1, t_v3d *v2)
{
	return (v1->x == v2->x && v1->y == v2->y && v1->z == v2->z);
}

int		find_linked_wall(t_sector *sector, t_v3d v, int skip)
{
	int		i;
	i = 0;

	while (i < sector->walls_count)
	{
		if (i != skip)
		{
			if(compare_vertex(&sector->walls[i].v[0], &v) ||
			   compare_vertex(&sector->walls[i].v[2], &v))
				return i;
		}
		i++;
	}
	return (0);
}

double	get_orientation(t_v3d *polygon, int size)
{
	int		i;
	double	sum;
	t_v3d	v1;
	t_v3d	v2;

	i = 0;
	sum = 0.0;
	while (i < size)
	{
		v1 = polygon[i];
		v2 = polygon[(i + 1) % size];
		sum += v1.x * v2.z - v1.z * v2.x;
		i++;
	}
	return (sum);
}