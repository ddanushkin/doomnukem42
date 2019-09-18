#include "doom_nukem.h"

t_mat4x4	rotation_mat_z(float angle)
{
	t_mat4x4	mat;

	ft_bzero(&mat, sizeof(t_mat4x4));
	mat.m[0][0] = cosf(angle);
	mat.m[0][1] = sinf(angle);
	mat.m[1][0] = -sinf(angle);
	mat.m[1][1] = cosf(angle);
	mat.m[2][2] = 1.0f;
	mat.m[3][3] = 1.0f;
	return (mat);
}

t_mat4x4	rotation_mat_x(float angle)
{
	t_mat4x4	mat;

	ft_bzero(&mat, sizeof(t_mat4x4));
	mat.m[0][0] = 1.0f;
	mat.m[1][1] = cosf(angle);
	mat.m[1][2] = sinf(angle);
	mat.m[2][1] = -sinf(angle);
	mat.m[2][2] = cosf(angle);
	mat.m[3][3] = 1.0f;
	return (mat);
}

t_mat4x4	rotation_mat_y(float angle)
{
	t_mat4x4	mat;

	ft_bzero(&mat, sizeof(t_mat4x4));
	mat.m[0][0] = cosf(angle);
	mat.m[0][2] = sinf(angle);
	mat.m[1][1] = 1.0f;
	mat.m[2][0] = -sinf(angle);
	mat.m[2][2] = cosf(angle);
	mat.m[3][3] = 1.0f;
	return (mat);
}

void	project_triangle(t_triangle *tr, t_mat4x4 *proj_mat)
{
	tr->v[0] = matrix_multiply_vector(*proj_mat, tr->v[0]);
	tr->v[1] = matrix_multiply_vector(*proj_mat, tr->v[1]);
	tr->v[2] = matrix_multiply_vector(*proj_mat, tr->v[2]);
}

void	scale_triangle(t_triangle *tr)
{
	tr->v[0] = vector_div_by(tr->v[0], tr->v[0].w);
	tr->v[1] = vector_div_by(tr->v[1], tr->v[1].w);
	tr->v[2] = vector_div_by(tr->v[2], tr->v[2].w);
}

void	offset_triangle(t_triangle *tr, t_app *app)
{
	t_vector offset_view;

	offset_view = new_vector(1.0f, 1.0f, 0.0f);
	tr->v[0] = vector_sum(tr->v[0], offset_view);
	tr->v[0].x *= app->sdl->half_width;
	tr->v[0].y *= app->sdl->half_height;
	tr->v[1] = vector_sum(tr->v[1], offset_view);
	tr->v[1].x *= app->sdl->half_width;
	tr->v[1].y *= app->sdl->half_height;
	tr->v[2] = vector_sum(tr->v[2], offset_view);
	tr->v[2].x *= app->sdl->half_width;
	tr->v[2].y *= app->sdl->half_height;
}

void	calc_light(t_triangle *tr, t_vector normal)
{
	t_vector	light_dir;
	float		light_dp;

	set_vector(&light_dir, 0.0f, 0.0f, -1.0f);
	light_dir = vector_normalise(light_dir);
	light_dp = vector_dot_product(normal, light_dir);
	tr->color.r = (int)((float)tr->color.r * light_dp);
	tr->color.g = (int)((float)tr->color.g * light_dp);
	tr->color.b = (int)((float)tr->color.b * light_dp);

	//Test: Dark grey is minimal color
	tr->color.r = CLAMP(tr->color.r, 10, 255);
	tr->color.g = CLAMP(tr->color.g, 10, 255);
	tr->color.b = CLAMP(tr->color.b, 10, 255);

//	tr->color.r = CLAMP(tr->color.r, 0, 255);
//	tr->color.g = CLAMP(tr->color.g, 0, 255);
//	tr->color.b = CLAMP(tr->color.b, 0, 255);

}

t_vector	calc_normal(t_triangle tr)
{
	t_vector line1;
	t_vector line2;
	t_vector normal;

	line1.x = tr.v[1].x - tr.v[0].x;
	line1.y = tr.v[1].y - tr.v[0].y;
	line1.z = tr.v[1].z - tr.v[0].z;
	line2.x = tr.v[2].x - tr.v[0].x;
	line2.y = tr.v[2].y - tr.v[0].y;
	line2.z = tr.v[2].z - tr.v[0].z;
	normal.x = line1.y * line2.z - line1.z * line2.y;
	normal.y = line1.z * line2.x - line1.x * line2.z;
	normal.z = line1.x * line2.y - line1.y * line2.x;
	return (vector_normalise(normal));
}

void	draw_outline(t_app *app, t_triangle triangle)
{
	t_color		clr;

	clr.r = 255;
	clr.g = 255;
	clr.b = 255;

	triangle.v[0].x = CLAMP(triangle.v[0].x, 0, app->sdl->width);
	triangle.v[0].y = CLAMP(triangle.v[0].y, 0, app->sdl->height);
	triangle.v[1].x = CLAMP(triangle.v[1].x, 0, app->sdl->width);
	triangle.v[1].y = CLAMP(triangle.v[1].y, 0, app->sdl->height);
	triangle.v[2].x = CLAMP(triangle.v[2].x, 0, app->sdl->width);
	triangle.v[2].y = CLAMP(triangle.v[2].y, 0, app->sdl->height);

	draw_line(app, triangle.v[1], triangle.v[0], &clr);
	draw_line(app, triangle.v[2], triangle.v[0], &clr);
	draw_line(app, triangle.v[1], triangle.v[2], &clr);
}

t_triangle	check_triangle(t_app *app, t_triangle tr)
{
	t_vector	normal;
	t_vector	camera_ray;

	normal = calc_normal(tr);
	camera_ray = vector_sub(tr.v[0], app->camera.pos);
	if (vector_dot_product(normal, camera_ray) < 0.0f)
	{
		calc_light(&tr, normal);
		tr.visible = 1;
	}
	return (tr);
}

void	render_triangle(t_app *app, t_triangle tr)
{
	project_triangle(&tr, &app->projection_mat);
	scale_triangle(&tr);

	tr.v[0].x *= -1.0f;
	tr.v[1].x *= -1.0f;
	tr.v[2].x *= -1.0f;
	tr.v[0].y *= -1.0f;
	tr.v[1].y *= -1.0f;
	tr.v[2].y *= -1.0f;

	offset_triangle(&tr, app);
	fill_triangle(app, tr);
	if (PRINT_DEBUG)
		draw_outline(app, tr);
}