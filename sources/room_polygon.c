#include "doom_nukem.h"

double 	calc_tex(double min, double cur, double max)
{
	return ((cur - min)/(max - min));
}

void	push_vertex(t_sector *cs, t_polygon **polygon, int i)
{
	t_v3d *v;

	v = &cs->fpts[i];
	v->tex_x = calc_tex(cs->x_min, v->x, cs->x_max);
	v->tex_y = calc_tex(cs->z_min, v->z, cs->z_max);
	polygon_add(polygon, *v);
}

t_polygon	*points_to_list(t_sector *s)
{
	double		or;
	int			i;
	t_polygon	*polygon;

	polygon = NULL;
	or = get_orientation(&s->fpts[0], s->pts_count);
	if (or > 0)
	{
		i = 0;
		while (i < s->pts_count)
			push_vertex(s, &polygon, i++);
	}
	else if (or < 0)
	{
		i = s->pts_count - 1;
		while (i >= 0)
			push_vertex(s, &polygon, i--);
	}
	s->trs_count = 0;
	return (polygon);
}