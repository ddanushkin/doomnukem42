#include "doom_nukem.h"

t_v3d	calc_normal2(t_v3d v1, t_v3d v2, t_v3d v3)
{
	return (v3d_normalise(v3d_cross(v3d_sub(v3, v1), v3d_sub(v2, v1))));
}

t_v3d 	calc_normal(t_v3d v0, t_v3d v1, t_v3d v2)
{
	return (v3d_normalise(v3d_cross(v3d_sub(v0, v1), v3d_sub(v0, v2))));
}

double plane_dist(t_v3d n, t_v3d p)
{
	return -((n.x * p.x) + (n.y * p.y) + (n.z * n.z));
}

int		check_sphere(t_v3d pos, t_v3d n, t_v3d p, double r, double *d)
{
	*d = n.x * pos.x + n.y * pos.y
		+ n.z * pos.z + (double)plane_dist(n, p);
	if (fabs(*d) < r)
		return (1);
	return (0);
}

double	calc_angle(t_v3d v1, t_v3d v2)
{
	double	a;

	a = acos(v3d_dot(v1, v2) / v3d_length(v1) * v3d_length(v2));
	if (isnan(a))
		return 0;
	return a;
}

int inside_polygon(t_v3d v0, t_v3d v1, t_v3d v2, t_v3d v3)
{
	double	a;

	a = 0.0;
	a += calc_angle(v3d_sub(v1, v0), v3d_sub(v2, v0));
	if (a >= 2.0 * M_PI)
		return (1);
	a += calc_angle(v3d_sub(v2, v0), v3d_sub(v3, v0));
	if (a >= 2.0 * M_PI)
		return (1);
	a += calc_angle(v3d_sub(v3, v0), v3d_sub(v1, v0));
	if (a >= 2.0 * M_PI)
		return (1);
	return (0);
}

t_v3d	closest_point(t_v3d a, t_v3d b, t_v3d p)
{
	t_v3d	v1;
	t_v3d	v2;
	t_v3d	dv;
	double	d;
	double	t;

	v1 = v3d_sub(p, a);
	v2 = v3d_sub(b, a);
	v2 = v3d_normalise(v2);
	dv = v3d_sub(a, b);
	d = v3d_length(dv);
	t = v3d_dot(v2, v1);
	if (t <= 0.0)
		return a;
	if (t >= d)
		return b;
	return (v3d_sum(a, v3d_mul_by(v2, t)));
}

int		edge_coll(t_v3d pos, double r, t_v3d v1, t_v3d v2, t_v3d v3)
{
	if (v3d_length(v3d_sub(closest_point(v1, v2, pos), pos)) < r)
		return (1);
	if (v3d_length(v3d_sub(closest_point(v2, v3, pos), pos)) < r)
		return (1);
	if (v3d_length(v3d_sub(closest_point(v3, v1, pos), pos)) < r)
		return (1);
	return (0);
}

int		tr_coll(t_v3d pos, double radius, t_v3d v1, t_v3d v2, t_v3d v3)
{
	double	d;
	t_v3d	n;

	n = calc_normal(v1, v2, v3);
	d = 0.0;
	if (check_sphere(pos, n, v1, radius, &d) &&
		(inside_polygon(v3d_sub(pos, v3d_mul_by(n, d)), v1, v2, v3) ||
		edge_coll(pos, radius, v1, v2, v3)))
		return 1;
	return 0;
}
