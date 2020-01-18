#include "doom_nukem.h"

void 	render_billboard(t_app *app, t_wall *w)
{
	t_v3d	v0;
	t_v3d	v1;
	t_v3d	v2;
	t_v3d	v3;

//	t_v3d	right = get_right(app->camera->rot);
//	t_v3d	forward	= get_forward(app->camera->rot);
//	t_v3d	up = vector_cross_product(forward, right);

	t_v3d	right = new_vector(app->camera->view.m[0][0], app->camera->view.m[0][1], app->camera->view.m[0][2]);
	//t_v3d	up = new_vector(app->camera->view.m[1][0], app->camera->view.m[1][1], app->camera->view.m[1][2]);
	t_v3d	up = new_vector(0, 1, 0);

	t_v3d	position = new_vector(1.0, 1.0, 0.0);
	double	size = 2.0;

	v0 = vector_sum(position, vector_mul_by(right, -0.5 * size));
	v0 = vector_sum(v0, vector_mul_by(up, -0.5 * size));
	v1 = vector_sum(position, vector_mul_by(right, 0.5 * size));
	v1 = vector_sum(v1, vector_mul_by(up, 0.5 * size));
	v2 = vector_sum(position, vector_mul_by(right, -0.5 * size));
	v2 = vector_sum(v2, vector_mul_by(up, 0.5 * size));
	v3 = vector_sum(position, vector_mul_by(right, 0.5 * size));
	v3 = vector_sum(v3, vector_mul_by(up, -0.5 * size));

	printf("aright : [%f, %f, %f, %f]\n", right.x, right.y, right.z, right.w);
	printf("av0 : [%f, %f, %f, %f, %f, %f]\n", v0.x, v0.y, v0.z, v0.w, v0.tex_x, v0.tex_y);
	printf("av1 : [%f, %f, %f, %f, %f, %f]\n", v1.x, v1.y, v1.z, v1.w, v1.tex_x, v1.tex_y);
	printf("av2 : [%f, %f, %f, %f, %f, %f]\n", v2.x, v2.y, v2.z, v2.w, v2.tex_x, v2.tex_y);
	printf("av3 : [%f, %f, %f, %f, %f, %f]\n\n", v3.x, v3.y, v3.z, v3.w, v3.tex_x, v3.tex_y);

	v0 = matrix_transform(app->camera->transform, v0);
	v1 = matrix_transform(app->camera->transform, v1);
	v2 = matrix_transform(app->camera->transform, v2);
	v3 = matrix_transform(app->camera->transform, v3);

	printf("render_billboard transform\n");
	printf("[%f][%f][%f][%f]\n", app->camera->transform.m[0][0], app->camera->transform.m[0][1], app->camera->transform.m[0][2], app->camera->transform.m[0][3]);
	printf("[%f][%f][%f][%f]\n", app->camera->transform.m[1][0], app->camera->transform.m[1][1], app->camera->transform.m[1][2], app->camera->transform.m[1][3]);
	printf("[%f][%f][%f][%f]\n", app->camera->transform.m[2][0], app->camera->transform.m[2][1], app->camera->transform.m[2][2], app->camera->transform.m[2][3]);
	printf("[%f][%f][%f][%f]\n\n", app->camera->transform.m[3][0], app->camera->transform.m[3][1], app->camera->transform.m[3][2], app->camera->transform.m[3][3]);

	printf("bright : [%f, %f, %f, %f]\n", right.x, right.y, right.z, right.w);
	printf("bv0 : [%f, %f, %f, %f, %f, %f]\n", v0.x, v0.y, v0.z, v0.w, v0.tex_x, v0.tex_y);
	printf("bv1 : [%f, %f, %f, %f, %f, %f]\n", v1.x, v1.y, v1.z, v1.w, v1.tex_x, v1.tex_y);
	printf("bv2 : [%f, %f, %f, %f, %f, %f]\n", v2.x, v2.y, v2.z, v2.w, v2.tex_x, v2.tex_y);
	printf("bv3 : [%f, %f, %f, %f, %f, %f]\n\n", v3.x, v3.y, v3.z, v3.w, v3.tex_x, v3.tex_y);

	v0.tex_x = 0.0;
	v0.tex_y = 0.0;
	v1.tex_x = 1.0;
	v1.tex_y = 1.0;
	v2.tex_x = 1.0;
	v2.tex_y = 0.0;
	v3.tex_x = 0.0;
	v3.tex_y = 1.0;
	app->hit = 0;
	app->render_wall = w;
	render_triangle(app, v0, v1, v2);
	render_triangle(app, v0, v3, v1);
}

void 	render_floor_ceil(t_app *app, t_triangle *tr, t_wall *w)
{
	t_v3d	v0;
	t_v3d	v1;
	t_v3d	v2;

	v0 = matrix_transform(app->camera->transform, tr->v[0]);
	v1 = matrix_transform(app->camera->transform, tr->v[1]);
	v2 = matrix_transform(app->camera->transform, tr->v[2]);
	app->hit = 0;
	app->render_wall = w;
	if (render_triangle(app, v0, v1, v2) && !app->edge_selected)
	{
		ray_intersect(app, tr->v[0], tr->v[1], tr->v[2]);
		if (app->hit)
			app->hit_first = 1;
	}
}

void 	render_wall(t_app *app, t_wall *w)
{
	t_v3d	v0;
	t_v3d	v1;
	t_v3d	v2;
	t_v3d	v3;

	v0 = matrix_transform(app->camera->transform, w->v[0]);
	v1 = matrix_transform(app->camera->transform, w->v[1]);
	v2 = matrix_transform(app->camera->transform, w->v[2]);
	v3 = matrix_transform(app->camera->transform, w->v[3]);
	app->render_wall = w;
	app->hit = 0;
	if (render_triangle(app, v0, v1, v2) && !app->edge_selected)
	{
		ray_intersect(app, w->v[0], w->v[1], w->v[2]);
		if (app->hit)
			app->hit_first = 1;
	}
	if (render_triangle(app, v0, v3, v1) && !app->edge_selected && !app->hit)
	{
		ray_intersect(app, w->v[0], w->v[3], w->v[1]);
		if (app->hit)
			app->hit_first = 0;
	}
}

void 	render_sector(t_app *app, t_sector	*s)
{
	int			j;
	t_triangle	ceil_triangle;

	j = 0;
	while (j < s->walls_count)
	{
		if (s->walls[j].billboard)
			render_billboard(app, &s->walls[j++]);
		else
			render_wall(app, &s->walls[j++]);
	}
	if (s->ready && s->triangles_count > 0)
	{
		j = 0;
		while (j < s->triangles_count)
		{
			render_floor_ceil(app, &s->triangles[j], &s->floor);
			ceil_triangle = s->triangles[j];
			ceil_triangle.v[0].y += 2.0;
			ceil_triangle.v[1].y += 2.0;
			ceil_triangle.v[2].y += 2.0;
			render_floor_ceil(app, &ceil_triangle, &s->ceil);
			j++;
		}
	}
}

void	render_map(t_app *app)
{
	int			i;

	i = 0;
	app->hit_dist = 10000.0;
	app->hit = 0;
	app->hit_wall = NULL;
	while (i < app->sectors_count)
		render_sector(app, &app->sectors[i++]);
}