#include "doom_nukem.h"

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
		return 0;

	double	aa = v3d_dot(A, A);
	double	ab = v3d_dot(A, B);
	double	ac = v3d_dot(A, C);
	int		sep2 = (aa > rr) & (ab > aa) & (ac > aa);

	if (sep2)
		return 0;

	double bb = v3d_dot(B, B);
	double bc = v3d_dot(B, C);
	int sep3 = (bb > rr) & (ab > bb) & (bc > bb);

	if (sep3)
		return 0;

	double cc = v3d_dot(C, C);
	int sep4 = (cc > rr) & (ac > cc) & (bc > cc);

	if (sep4)
		return 0;

	t_v3d AB = v3d_sub(B, A);
	t_v3d BC = v3d_sub(C, B);
	t_v3d CA = v3d_sub(A, C);

	double d1 = ab - aa;
	double e1 = v3d_dot(AB, AB);

	t_v3d Q1 = v3d_sub(v3d_mul_by(A, e1), v3d_mul_by(AB, d1));
	t_v3d QC = v3d_sub(v3d_mul_by(C, e1), Q1);
	int sep5 = (v3d_dot(Q1, Q1) > rr * e1 * e1) & (v3d_dot(Q1, QC) > 0);

	if (sep5)
		return 0;

	double d2 = bc - bb;
	double e2 = v3d_dot(BC, BC);

	t_v3d Q2 = v3d_sub(v3d_mul_by(B, e2), v3d_mul_by(BC, d2));
	t_v3d QA = v3d_sub(v3d_mul_by(A, e2), Q2);
	int sep6 = (v3d_dot(Q2, Q2) > rr * e2 * e2) & (v3d_dot(Q2, QA) > 0);

	if (sep6)
		return 0;

	double d3 = ac - cc;
	double e3 = v3d_dot(CA, CA);

	t_v3d Q3 = v3d_sub(v3d_mul_by(C, e3), v3d_mul_by(CA, d3));
	t_v3d QB = v3d_sub(v3d_mul_by(B, e3), Q3);
	int sep7 = (v3d_dot(Q3, Q3) > rr * e3 * e3) & (v3d_dot(Q3, QB) > 0);

	if (sep7)
		return 0;

	return 1;
}

int		wall_check_collision(t_wall *w, t_v3d *pos, t_v3d *f, t_v3d n)
{
	int 	res;
	t_v3d	new_pos;

	new_pos = v3d_sum(*pos, *f);
	res = (is_separated(w->v[0], w->v[1], w->v[2], new_pos, 0.15) ||
			is_separated(w->v[0], w->v[3], w->v[1], new_pos, 0.15));
	if (res)
		*f = v3d_sub(*f, v3d_mul_by(n, v3d_dot(n, *f)));
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
	t_v3d	n;

	i = 0;
	count = 0;
	while (i < app->sectors_count)
	{
		j = 0;
		while (j < app->sectors[i].walls_count)
		{
			w = &app->sectors[i].walls[j];
			n = get_triangle_normal(w->v[3], w->v[0], w->v[2]);
			if ((w->active && (pos->y + 0.25) > MIN(w->v[0].y, w->v[2].y)))
				count += wall_check_collision(w, pos, &dir, n);
			j++;
		}
		i++;
	}
	*pos = v3d_sum(*pos, dir);
}