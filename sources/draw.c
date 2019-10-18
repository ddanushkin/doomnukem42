#include "doom_nukem.h"

t_mat4x4	rotation_mat_z(double angle)
{
	t_mat4x4	mat;

	ft_bzero(&mat, sizeof(t_mat4x4));
	mat.m[0][0] = cos(angle);
	mat.m[0][1] = sin(angle);
	mat.m[1][0] = -sin(angle);
	mat.m[1][1] = cos(angle);
	mat.m[2][2] = 1.0;
	mat.m[3][3] = 1.0;
	return (mat);
}

t_mat4x4	rotation_mat_x(double angle)
{
	t_mat4x4	mat;

	ft_bzero(&mat, sizeof(t_mat4x4));
	mat.m[0][0] = 1.0;
	mat.m[1][1] = cos(angle);
	mat.m[1][2] = sin(angle);
	mat.m[2][1] = -sin(angle);
	mat.m[2][2] = cos(angle);
	mat.m[3][3] = 1.0;
	return (mat);
}

t_mat4x4	rotation_mat_y(double angle)
{
	t_mat4x4	mat;

	ft_bzero(&mat, sizeof(t_mat4x4));
	mat.m[0][0] = cos(angle);
	mat.m[0][2] = sin(angle);
	mat.m[1][1] = 1.0;
	mat.m[2][0] = -sin(angle);
	mat.m[2][2] = cos(angle);
	mat.m[3][3] = 1.0;
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
	t_v3d offset_view;

	offset_view = new_vector(1.0, 1.0, 0.0);
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

void	calc_light(t_triangle *tr, t_v3d normal)
{
	t_v3d	light_dir;
	double		light_dp;

	set_vector(&light_dir, 0.0, 0.0, -1.0);
	light_dir = vector_normalise(light_dir);
	light_dp = vector_dot_product(normal, light_dir);
	tr->color.r = (int)((double)tr->color.r * light_dp);
	tr->color.g = (int)((double)tr->color.g * light_dp);
	tr->color.b = (int)((double)tr->color.b * light_dp);

	tr->color.r = CLAMP(tr->color.r, 10, 255);
	tr->color.g = CLAMP(tr->color.g, 10, 255);
	tr->color.b = CLAMP(tr->color.b, 10, 255);
}

t_v3d	calc_normal(t_triangle tr)
{
	t_v3d line1;
	t_v3d line2;
	t_v3d normal;

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
	clr.g = 0;
	clr.b = 255;

	draw_line(app, &triangle.v[1], &triangle.v[0], clr);
	draw_line(app, &triangle.v[2], &triangle.v[0], clr);
	draw_line(app, &triangle.v[1], &triangle.v[2], clr);
}

void	check_triangle(t_app *app, t_triangle *tr)
{
	t_v3d	normal;
	t_v3d	camera_ray;

	normal = calc_normal(*tr);
	camera_ray = vector_sub(tr->v[0], app->camera->pos);
	if (vector_dot_product(normal, camera_ray) < 0.0)
	{
		calc_light(tr, normal);
		tr->visible = 1;
	}
}

void	render_triangle(t_app *app, t_triangle tr)
{
	t_tr_list	*tr_lst;
	t_tr_list	*tmp_next;

	if (tr.v[0].z < 0.1 && tr.v[1].z < 0.1 && tr.v[2].z < 0.1)
		return;
	if (tr.v[0].z < 0.1)
		tr.v[0].z = 0.1;
	if (tr.v[1].z < 0.1)
		tr.v[1].z = 0.1;
	if (tr.v[2].z < 0.1)
		tr.v[2].z = 0.1;
	project_triangle(&tr, &app->projection_mat);
	scale_triangle(&tr);
	tr.v[0] = vector_mul_by(tr.v[0], -1);
	tr.v[0].z *= -1;
	tr.v[1] = vector_mul_by(tr.v[1], -1);
	tr.v[1].z *= -1;
	tr.v[2] = vector_mul_by(tr.v[2], -1);
	tr.v[2].z *= -1;
	offset_triangle(&tr, app);

	tr_lst = (t_tr_list *)malloc(sizeof(t_tr_list));
	tr_lst->tr = tr;
	tr_lst->next = NULL;
	clip_triangles(&tr_lst);
	while (tr_lst != NULL)
	{
		fill_triangle(app, tr_lst->tr);
		draw_outline(app, tr_lst->tr);
		tmp_next = tr_lst->next;
		free(tr_lst);
		tr_lst = tmp_next;
	}
}

void	draw_cross(t_app *app, double size, int r, int g, int b)
{
	t_v3d	cross_hl_s;
	t_v3d	cross_hl_e;
	t_v3d	cross_vl_s;
	t_v3d	cross_vl_e;
	t_color	cross_color;

	if (size <= 0.0)
		size = 5.0;

	set_color(&cross_color, r, g, b);
	cross_hl_s = new_vector(app->sdl->half_width - size, app->sdl->half_height, 0.0);
	cross_hl_e = new_vector(app->sdl->half_width + size, app->sdl->half_height, 0.0);
	cross_vl_s = new_vector(app->sdl->half_width, app->sdl->half_height - size, 0.0);
	cross_vl_e = new_vector(app->sdl->half_width, app->sdl->half_height + size, 0.0);
	draw_line(app, &cross_hl_s, &cross_hl_e, cross_color);
	draw_line(app, &cross_vl_s, &cross_vl_e, cross_color);
}