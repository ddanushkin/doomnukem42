#include "doom_nukem.h"

void	mat_vector_mult(t_vertex *vt, t_mat4x4 *mat)
{
	t_vertex	tmp;
	float		w;

	tmp = *vt;
	vt->x = tmp.x * mat->m[0][0] + tmp.y * mat->m[1][0]
				 + tmp.z * mat->m[2][0] + mat->m[3][0];
	vt->y = tmp.x * mat->m[0][1] + tmp.y * mat->m[1][1]
				 + tmp.z * mat->m[2][1] + mat->m[3][1];
	vt->z = tmp.x * mat->m[0][2] + tmp.y * mat->m[1][2]
				 + tmp.z * mat->m[2][2] + mat->m[3][2];
	w = tmp.x * mat->m[0][3] + tmp.y * mat->m[1][3]
		+ tmp.z * mat->m[2][3] + mat->m[3][3];
	if (w != 0.0f)
	{
		vt->x /= w;
		vt->y /= w;
		vt->z /= w;
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

void	rotate_triangle(t_triangle *tr, t_mat4x4 *rot_mat)
{
	mat_vector_mult(&tr->v[0], rot_mat);
	mat_vector_mult(&tr->v[1], rot_mat);
	mat_vector_mult(&tr->v[2], rot_mat);
}

void	project_triangle(t_triangle *tr, t_mat4x4 *proj_mat)
{
	mat_vector_mult(&tr->v[0], proj_mat);
	mat_vector_mult(&tr->v[1], proj_mat);
	mat_vector_mult(&tr->v[2], proj_mat);
}

void	translate_triangle(t_triangle *tr, t_app *app)
{
	set_vertex(&tr->v[0], tr->v[0].x + app->camera.pos.x, tr->v[0].y, tr->v[0].z + app->camera.pos.y);
	set_vertex(&tr->v[1], tr->v[1].x + app->camera.pos.x, tr->v[1].y, tr->v[1].z + app->camera.pos.y);
	set_vertex(&tr->v[2], tr->v[2].x + app->camera.pos.x, tr->v[2].y, tr->v[2].z + app->camera.pos.y);
}

void	offset_triangle(t_triangle *tr, float offset)
{
	tr->v[0].z += offset;
	tr->v[1].z += offset;
	tr->v[2].z += offset;
}

void	scale_vector(t_vertex *vector)
{
	vector->x += 1.0f;
	vector->y += 1.0f;
	vector->x *= 0.5f * (float)SCREEN_W;
	vector->y *= 0.5f * (float)SCREEN_H;
}

void	scale_triangle(t_triangle *triangle)
{
	scale_vector(&triangle->v[0]);
	scale_vector(&triangle->v[1]);
	scale_vector(&triangle->v[2]);
}

void	calc_light(t_triangle *tr, t_vertex normal)
{
	t_vertex	light_dir;
	float		light_dp;

	set_vertex(&light_dir, 0.0f, 0.0f, -1.0f);
	light_dir = normalise_vector(light_dir);
	light_dp = dot_product(normal, light_dir);
	tr->color.r = (int)((float)tr->color.r * light_dp);
	tr->color.g = (int)((float)tr->color.g * light_dp);
	tr->color.b = (int)((float)tr->color.b * light_dp);
	tr->color.r = CLAMP(tr->color.r, 0, 255);
	tr->color.g = CLAMP(tr->color.g, 0, 255);
	tr->color.b = CLAMP(tr->color.b, 0, 255);

}

t_vertex	calc_normal(t_triangle tr)
{
	t_vertex line1;
	t_vertex line2;
	t_vertex normal;

	line1.x = tr.v[1].x - tr.v[0].x;
	line1.y = tr.v[1].y - tr.v[0].y;
	line1.z = tr.v[1].z - tr.v[0].z;
	line2.x = tr.v[2].x - tr.v[0].x;
	line2.y = tr.v[2].y - tr.v[0].y;
	line2.z = tr.v[2].z - tr.v[0].z;
	normal.x = line1.y * line2.z - line1.z * line2.y;
	normal.y = line1.z * line2.x - line1.x * line2.z;
	normal.z = line1.x * line2.y - line1.y * line2.x;
	return (normalise_vector(normal));
}

void	draw_outline(t_app *app, t_triangle triangle)
{
	t_color		clr;

	clr.r = 0;
	clr.g = 0;
	clr.b = 0;
	draw_line(app, triangle.v[1], triangle.v[0], &clr);
	draw_line(app, triangle.v[2], triangle.v[0], &clr);
	draw_line(app, triangle.v[1], triangle.v[2], &clr);
}

int 	triangle_is_visible(t_app *app, t_triangle tr, t_vertex normal)
{
	t_vertex	tmp;

	tmp = vector_subtract(tr.v[1], app->camera.pos);
	return (cross_product(&normal, &tmp) < 0.0f);
}

t_triangle	check_triangle(t_app *app, t_triangle tr)
{
	t_vertex	normal;

	rotate_triangle(&tr, &app->rotation_mat_z);
	rotate_triangle(&tr, &app->rotation_mat_x);
	translate_triangle(&tr, app);
	offset_triangle(&tr, 300.0f);
	normal = calc_normal(tr);
	if (triangle_is_visible(app, tr, normal))
	{
		calc_light(&tr, normal);
		tr.visible = 1;
	}
	return (tr);
}

void	render_triangle(t_app *app, t_triangle triangle)
{
	project_triangle(&triangle, &app->projection_mat);
	scale_triangle(&triangle);
	fill_triangle(app, triangle);
	if (PRINT_DEBUG)
		draw_outline(app, triangle);
}