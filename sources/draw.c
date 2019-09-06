#include "doom_nukem.h"

void	mat_vector_mult(t_vertex *in_vec, t_vertex *out_vec, t_mat4x4 *mat)
{
	float	w;

	out_vec->x = in_vec->x * mat->m[0][0] + in_vec->y * mat->m[1][0]
				 + in_vec->z * mat->m[2][0] + mat->m[3][0];
	out_vec->y = in_vec->x * mat->m[0][1] + in_vec->y * mat->m[1][1]
				 + in_vec->z * mat->m[2][1] + mat->m[3][1];
	out_vec->z = in_vec->x * mat->m[0][2] + in_vec->y * mat->m[1][2]
				 + in_vec->z * mat->m[2][2] + mat->m[3][2];
	w = in_vec->x * mat->m[0][3] + in_vec->y * mat->m[1][3]
		+ in_vec->z * mat->m[2][3] + mat->m[3][3];
	if (w != 0.0f)
	{
		out_vec->x /= w;
		out_vec->y /= w;
		out_vec->z /= w;
	}
}

void	update_rotation_mat_z(t_app *app, float angle)
{
	ft_bzero(&app->rotation_mat_z, sizeof(t_mat4x4));
	app->rotation_mat_z.m[0][0] = cosf(angle);
	app->rotation_mat_z.m[0][1] = sinf(angle);
	app->rotation_mat_z.m[1][0] = -sinf(angle);
	app->rotation_mat_z.m[1][1] = cosf(angle);
	app->rotation_mat_z.m[2][2] = 1;
	app->rotation_mat_z.m[3][3] = 1;
}

void	update_rotation_mat_x(t_app *app, float angle)
{
	ft_bzero(&app->rotation_mat_x, sizeof(t_mat4x4));
	app->rotation_mat_x.m[0][0] = 1;
	app->rotation_mat_x.m[1][1] = cosf(angle);
	app->rotation_mat_x.m[1][2] = sinf(angle);
	app->rotation_mat_x.m[2][1] = -sinf(angle);
	app->rotation_mat_x.m[2][2] = cosf(angle);
	app->rotation_mat_x.m[3][3] = 1;
}

void	rotate_triangle(t_triangle *in, t_triangle *out, t_mat4x4 *rot_mat)
{
	ft_bzero(out, sizeof(t_triangle));
	out->v[0] = (t_vertex *)malloc(sizeof(t_vertex));
	out->v[1] = (t_vertex *)malloc(sizeof(t_vertex));
	out->v[2] = (t_vertex *)malloc(sizeof(t_vertex));
	mat_vector_mult(in->v[0], out->v[0], rot_mat);
	mat_vector_mult(in->v[1], out->v[1], rot_mat);
	mat_vector_mult(in->v[2], out->v[2], rot_mat);
}

void	project_triangle(t_triangle *in, t_triangle *out, t_mat4x4 *proj_mat)
{
	ft_bzero(out, sizeof(t_triangle));
	out->v[0] = (t_vertex *)malloc(sizeof(t_vertex));
	out->v[1] = (t_vertex *)malloc(sizeof(t_vertex));
	out->v[2] = (t_vertex *)malloc(sizeof(t_vertex));
	mat_vector_mult(in->v[0], out->v[0], proj_mat);
	mat_vector_mult(in->v[1], out->v[1], proj_mat);
	mat_vector_mult(in->v[2], out->v[2], proj_mat);
}

void	translate_triangle(t_triangle *in, t_triangle *out)
{
	ft_bzero(out, sizeof(t_triangle));
	out->v[0] = (t_vertex *)malloc(sizeof(t_vertex));
	out->v[1] = (t_vertex *)malloc(sizeof(t_vertex));
	out->v[2] = (t_vertex *)malloc(sizeof(t_vertex));
	set_vertex(out->v[0], in->v[0]->x, in->v[0]->y, in->v[0]->z + 3.0f);
	set_vertex(out->v[1], in->v[1]->x, in->v[1]->y, in->v[1]->z + 3.0f);
	set_vertex(out->v[2], in->v[2]->x, in->v[2]->y, in->v[2]->z + 3.0f);
}

void	scale_vector(t_app *app, t_vertex *vector)
{
	vector->x += 1.0f;
	vector->y += 1.0f;
	vector->x *= 0.5f * (float)SCREEN_W;
	vector->y *= 0.5f * (float)SCREEN_H;
}

void	scale_triangle(t_app *app, t_triangle *triangle)
{
	scale_vector(app, triangle->v[0]);
	scale_vector(app, triangle->v[1]);
	scale_vector(app, triangle->v[2]);
}

void	draw_triangle(t_app *app, t_triangle triangle)
{
	t_triangle	projected;
	t_triangle	rotated_z;
	t_triangle	rotated_x;
	t_triangle	translated;

	t_vertex normal, line1, line2;

	rotate_triangle(&triangle, &rotated_z, &app->rotation_mat_z);
	rotate_triangle(&rotated_z, &rotated_x, &app->rotation_mat_x);
	free(rotated_z.v[0]);
	free(rotated_z.v[1]);
	free(rotated_z.v[2]);

	translate_triangle(&rotated_x, &translated);
	free(rotated_x.v[0]);
	free(rotated_x.v[1]);
	free(rotated_x.v[2]);

	line1.x = translated.v[1]->x - translated.v[0]->x;
	line1.y = translated.v[1]->y - translated.v[0]->y;
	line1.z = translated.v[1]->z - translated.v[0]->z;

	line2.x = translated.v[2]->x - translated.v[0]->x;
	line2.y = translated.v[2]->y - translated.v[0]->y;
	line2.z = translated.v[2]->z - translated.v[0]->z;

	normal.x = line1.y * line2.z - line1.z * line2.y;
	normal.y = line1.z * line2.x - line1.x * line2.z;
	normal.z = line1.x * line2.y - line1.y * line2.x;

	float length = sqrtf(normal.x * normal.x + normal.y * normal.y + normal.z * normal.z);

	normal.x /= length;
	normal.y /= length;
	normal.z /= length;

	if (1)
	{
		project_triangle(&translated, &projected, &app->projection_mat);
		projected.color = triangle.color;
		scale_triangle(app, &projected);
		fill_triangle(app, projected);
		t_color white;
		white.b = 255;
		white.r = 255;
		white.g = 255;
		draw_line(app, *projected.v[1], *projected.v[0], &white);
		draw_line(app, *projected.v[2], *projected.v[0], &white);
		draw_line(app, *projected.v[1], *projected.v[2], &white);
	}
	else
	{
		printf("huevo\n");
	}
	free(translated.v[0]);
	free(translated.v[1]);
	free(translated.v[2]);
}