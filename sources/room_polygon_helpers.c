#include "doom_nukem.h"

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
