#include "doom_nukem.h"

void 		coll_t1_t2(t_coll_data	*c, t_v3d* v)
{
	double	min;
	double	max;
	int 	div;

	div = 0;
	c->y = 0.0;
	max = MAX(v[3].y, v[1].y);
	min = MIN(v[3].y, v[1].y);
	c->t1 = (-c->b - sqrt(c->d)) / (2.0 * c->a);
	c->t2 = (-c->b + sqrt(c->d)) / (2.0 * c->a);
	if (c->t1 < 1 && c->t1 > 0)
	{
		c->y += max * (1.0 - c->t1) + c->t1 * min;
		c->coll = 1;
	}
	if (c->y != 0.0)
		div = 1;
	if (c->t2 < 1 && c->t2 > 0)
	{
		c->y += max * (1.0 - c->t2) + c->t2 * min;
		c->coll = 1;
		if (div)
			c->y *= 0.5;
	}
}

t_coll_data	is_colliding(t_v3d c0, double radius, t_v3d* v)
{
	t_v3d		dp;
	t_coll_data	c;

	c.coll = 0;
	c0.y = MIN(v[0].y, v[2].y) + fabs(v[0].y - v[2].y) * 0.5;
	dp = v3d_sub(v[2], v[0]);
	c.a = v3d_dot(dp, dp);
	c.b = 2.0 *
		(v3d_dot(v[0], dp) - dp.x * c0.x - dp.y * c0.y - dp.z * c0.z);
	c.c = v[0].x * v[0].x -
		2 * v[0].x * c0.x + c0.x * c0.x + v[0].y * v[0].y -
		2 * v[0].y * c0.y + c0.y * c0.y + v[0].z * v[0].z -
		2 * v[0].z * c0.z + c0.z * c0.z - radius * radius;
	c.d = c.b * c.b - 4 * c.a * c.c;
	if (c.d < 0)
		return (c);
	coll_t1_t2(&c, v);
	return (c);
}

t_v3d 	get_triangle_normal(t_v3d v0, t_v3d v1, t_v3d v2)
{
	return (vector_normalise(v3d_cross(v3d_sub(v0, v1), v3d_sub(v0, v2))));
}

int is_separated(
		t_v3d A,
		t_v3d B,
		t_v3d C,
		t_v3d P,
		double r)
{
	A = v3d_sub(A, P);
	B = v3d_sub(B, P);
	C = v3d_sub(C, P);

	double	rr = r * r;
	t_v3d	V =  v3d_cross(v3d_sub(B, A), v3d_sub(C, A));
	double	d = v3d_dot(A, V);
	double	e = v3d_dot(V, V);
	int sep1 = d * d > rr * e;

	if (sep1)
		return 1;

	double	aa = v3d_dot(A, A);
	double	ab = v3d_dot(A, B);
	double	ac = v3d_dot(A, C);
	int		sep2 = (aa > rr) & (ab > aa) & (ac > aa);

	if (sep2)
		return 1;

	double bb = v3d_dot(B, B);
	double bc = v3d_dot(B, C);
	int sep3 = (bb > rr) & (ab > bb) & (bc > bb);

	if (sep3)
		return 1;

	double cc = v3d_dot(C, C);
	int sep4 = (cc > rr) & (ac > cc) & (bc > cc);

	if (sep4)
		return 1;

	t_v3d AB = v3d_sub(B, A);
	t_v3d BC = v3d_sub(C, B);
	t_v3d CA = v3d_sub(A, C);

	double d1 = ab - aa;
	double e1 = v3d_dot(AB, AB);

	t_v3d Q1 = v3d_sub(v3d_mul_by(A, e1), v3d_mul_by(AB, d1));
	t_v3d QC = v3d_sub(v3d_mul_by(C, e1), Q1);
	int sep5 = (v3d_dot(Q1, Q1) > rr * e1 * e1) & (v3d_dot(Q1, QC) > 0);

	if (sep5)
		return 1;

	double d2 = bc - bb;
	double e2 = v3d_dot(BC, BC);

	t_v3d Q2 = v3d_sub(v3d_mul_by(B, e2), v3d_mul_by(BC, d2));
	t_v3d QA = v3d_sub(v3d_mul_by(A, e2), Q2);
	int sep6 = (v3d_dot(Q2, Q2) > rr * e2 * e2) & (v3d_dot(Q2, QA) > 0);

	if (sep6)
		return 1;

	double d3 = ac - cc;
	double e3 = v3d_dot(CA, CA);

	t_v3d Q3 = v3d_sub(v3d_mul_by(C, e3), v3d_mul_by(CA, d3));
	t_v3d QB = v3d_sub(v3d_mul_by(B, e3), Q3);
	int sep7 = (v3d_dot(Q3, Q3) > rr * e3 * e3) & (v3d_dot(Q3, QB) > 0);

	if (sep7)
		return 1;

	return 0;
}

int		wall_check_collision(t_wall *w, t_v3d *pos, t_v3d *f)
{
	int 		res;
	t_v3d		normal;
	res = (!is_separated(w->v[3], w->v[0], w->v[2], *pos, 0.35) ||
			!is_separated(w->v[2], w->v[1], w->v[3], *pos, 0.35));
	if (res)
	{
		normal = get_triangle_normal(w->v[1], w->v[2], w->v[0]);
		normal = v3d_mul_by(normal, v3d_dot(*f, normal));
		*f = v3d_sub(*f, normal);
		*pos = v3d_sub(*pos, normal);
	}
	return (res);
}

void 	stop_collision(t_v3d *pos, t_v3d prev)
{
	*pos = prev;
}

void 	check_collision(t_app *app, t_v3d *pos, t_v3d dir)
{
	int		i;
	int		j;
	t_wall	*w;
	int		count;

	i = 0;
	count = 0;
	*pos = v3d_sum(*pos, dir);
	while (i < app->sectors_count)
	{
		j = 0;
		while (j < app->sectors[i].walls_count)
		{
			w = &app->sectors[i].walls[j++];
			if ((w->active && (pos->y + 0.25) > MIN(w->v[0].y, w->v[2].y)))
				count += wall_check_collision(w, pos, &dir);
//			if (count >= 2)
//				return (stop_collision(pos, prev));
		}
		i++;
	}
}