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

void	calc_light(t_app *app, t_triangle *tr, t_v3d normal)
{
	t_v3d	light_dir;
	double	light_dp;

	set_vector(&light_dir, 0.0, 1.0, -1.0);
	light_dir = vector_normalise(light_dir);
	light_dp = MAX(0.1, vector_dot_product(normal, light_dir));
	tr->color.r = (int)((double)tr->color.r * light_dp);
	tr->color.g = (int)((double)tr->color.g * light_dp);
	tr->color.b = (int)((double)tr->color.b * light_dp);
	tr->color.r = CLAMP(tr->color.r, 0, 255);
	tr->color.g = CLAMP(tr->color.g, 0, 255);
	tr->color.b = CLAMP(tr->color.b, 0, 255);
}

t_v3d	calc_normal(t_triangle tr)
{
	t_v3d	tmp_v1;
	t_v3d	tmp_v2;

	tmp_v1 = vector_sub(tr.v[1], tr.v[0]);
	tmp_v2 = vector_sub(tr.v[2], tr.v[0]);
	return (vector_normalise(vector_cross_product(tmp_v1, tmp_v2)));
}

void	draw_outline(t_app *app, t_triangle triangle)
{
	t_color c;

	c = color_sub(triangle.color, 10);
	draw_line(app, &triangle.v[1], &triangle.v[0], c);
	draw_line(app, &triangle.v[2], &triangle.v[0], c);
	draw_line(app, &triangle.v[1], &triangle.v[2], c);
}

void	check_triangle(t_app *app, t_triangle *tr)
{
	t_v3d	normal;
	t_v3d	camera_ray;

	normal = calc_normal(*tr);
	camera_ray = vector_sub(tr->v[0], app->camera->pos);
	if (vector_dot_product(normal, camera_ray) < 0.0)
	{
		calc_light(app, tr, normal);
		tr->visible = 1;
	}
}

void TexturedTriangle(
		t_app *app,
		int x1, int y1, double u1, double v1, double w1,
		int x2, int y2, double u2, double v2, double w2,
		int x3, int y3, double u3, double v3, double w3)
{
	if (y2 < y1)
	{
		SWAP(y1, y2, int);
		SWAP(x1, x2, int);
		SWAP(u1, u2, double);
		SWAP(v1, v2, double);
		SWAP(w1, w2, double);
	}

	if (y3 < y1)
	{
		SWAP(y1, y3, int);
		SWAP(x1, x3, int);
		SWAP(u1, u3, double);
		SWAP(v1, v3, double);
		SWAP(w1, w3, double);
	}

	if (y3 < y2)
	{
		SWAP(y2, y3, int);
		SWAP(x2, x3, int);
		SWAP(u2, u3, double);
		SWAP(v2, v3, double);
		SWAP(w2, w3, double);
	}

	int dy1 = y2 - y1;
	int dx1 = x2 - x1;
	double dv1 = v2 - v1;
	double du1 = u2 - u1;
	double dw1 = w2 - w1;

	int dy2 = y3 - y1;
	int dx2 = x3 - x1;
	double dv2 = v3 - v1;
	double du2 = u3 - u1;
	double dw2 = w3 - w1;

	double tex_u, tex_v, tex_w;

	double dax_step = 0, dbx_step = 0,
			du1_step = 0, dv1_step = 0,
			du2_step = 0, dv2_step = 0,
			dw1_step = 0, dw2_step = 0;

	if (dy1) dax_step = dx1 / (double)abs(dy1);
	if (dy2) dbx_step = dx2 / (double)abs(dy2);

	if (dy1) du1_step = du1 / (double)abs(dy1);
	if (dy1) dv1_step = dv1 / (double)abs(dy1);
	if (dy1) dw1_step = dw1 / (double)abs(dy1);

	if (dy2) du2_step = du2 / (double)abs(dy2);
	if (dy2) dv2_step = dv2 / (double)abs(dy2);
	if (dy2) dw2_step = dw2 / (double)abs(dy2);

	if (dy1)
	{
		for (int i = y1; i <= y2; i++)
		{
			int ax = x1 + (double)(i - y1) * dax_step;
			int bx = x1 + (double)(i - y1) * dbx_step;

			double tex_su = u1 + (double)(i - y1) * du1_step;
			double tex_sv = v1 + (double)(i - y1) * dv1_step;
			double tex_sw = w1 + (double)(i - y1) * dw1_step;

			double tex_eu = u1 + (double)(i - y1) * du2_step;
			double tex_ev = v1 + (double)(i - y1) * dv2_step;
			double tex_ew = w1 + (double)(i - y1) * dw2_step;

			if (ax > bx)
			{
				SWAP(ax, bx, int);
				SWAP(tex_su, tex_eu, double);
				SWAP(tex_sv, tex_ev, double);
				SWAP(tex_sw, tex_ew, double);
			}

			double tstep = 1.0 / (bx - ax);
			double t = 0.0;

			for (int j = ax; j < bx; j++)
			{
				tex_u = (1.0 - t) * tex_su + t * tex_eu;
				tex_v = (1.0 - t) * tex_sv + t * tex_ev;
				tex_w = (1.0 - t) * tex_sw + t * tex_ew;
				/* TODO: Fix DepthBuffer (app->z_buf) check */
				if (1)
				{
					set_pixel(app->sdl->surface, j, i, sprite_get_color_by_uv(&app->sprites[0], tex_u / tex_w, tex_v / tex_w));
					app->z_buf[i * SCREEN_W + j] = tex_w;
				}
				t += tstep;
			}

		}
	}

	dy1 = y3 - y2;
	dx1 = x3 - x2;
	dv1 = v3 - v2;
	du1 = u3 - u2;
	dw1 = w3 - w2;

	if (dy1) dax_step = dx1 / (double)abs(dy1);
	if (dy2) dbx_step = dx2 / (double)abs(dy2);

	du1_step = 0, dv1_step = 0;
	if (dy1) du1_step = du1 / (double)abs(dy1);
	if (dy1) dv1_step = dv1 / (double)abs(dy1);
	if (dy1) dw1_step = dw1 / (double)abs(dy1);

	if (dy1)
	{
		for (int i = y2; i <= y3; i++)
		{
			int ax = x2 + (double)(i - y2) * dax_step;
			int bx = x1 + (double)(i - y1) * dbx_step;

			double tex_su = u2 + (double)(i - y2) * du1_step;
			double tex_sv = v2 + (double)(i - y2) * dv1_step;
			double tex_sw = w2 + (double)(i - y2) * dw1_step;

			double tex_eu = u1 + (double)(i - y1) * du2_step;
			double tex_ev = v1 + (double)(i - y1) * dv2_step;
			double tex_ew = w1 + (double)(i - y1) * dw2_step;

			if (ax > bx)
			{
				SWAP(ax, bx, int);
				SWAP(tex_su, tex_eu, double);
				SWAP(tex_sv, tex_ev, double);
				SWAP(tex_sw, tex_ew, double);
			}

			double tstep = 1.0 / (bx - ax);
			double t = 0.0;

			for (int j = ax; j < bx; j++)
			{
				tex_u = (1.0 - t) * tex_su + t * tex_eu;
				tex_v = (1.0 - t) * tex_sv + t * tex_ev;
				tex_w = (1.0 - t) * tex_sw + t * tex_ew;
				/* TODO: Fix DepthBuffer (app->z_buf) check */
				if (1)
				{
					set_pixel(app->sdl->surface, j, i, sprite_get_color_by_uv(&app->sprites[0], tex_u / tex_w, tex_v / tex_w));
					app->z_buf[i * SCREEN_W + j] = tex_w;
				}
				t += tstep;
			}
		}
	}
}

void	render_triangle(t_app *app, t_triangle tr)
{
	t_tr_list	*tr_lst;
	t_tr_list	*tmp_next;

	tr.v[0] = matrix_multiply_vector(app->camera->view_mat, tr.v[0]);
	tr.v[1] = matrix_multiply_vector(app->camera->view_mat, tr.v[1]);
	tr.v[2] = matrix_multiply_vector(app->camera->view_mat, tr.v[2]);
	if (tr.v[0].z < 0.1 && tr.v[1].z < 0.1 && tr.v[2].z < 0.1)
		return;
	if (tr.v[0].z < 0.1)
		tr.v[0].z = 0.1;
	if (tr.v[1].z < 0.1)
		tr.v[1].z = 0.1;
	if (tr.v[2].z < 0.1)
		tr.v[2].z = 0.1;

	project_triangle(&tr, &app->projection_mat);

	tr.t[0].u /= tr.v[0].w;
	tr.t[0].v /= tr.v[0].w;
	tr.t[0].w = 1.0 / tr.v[0].w;

	tr.t[1].u /= tr.v[1].w;
	tr.t[1].v /= tr.v[1].w;
	tr.t[1].w = 1.0 / tr.v[1].w;

	tr.t[2].u /= tr.v[2].w;
	tr.t[2].v /= tr.v[2].w;
	tr.t[2].w = 1.0 / tr.v[2].w;

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
		tr_lst->tr.color = tr.color;
		TexturedTriangle(app,
						 tr_lst->tr.v[0].x, tr_lst->tr.v[0].y, tr_lst->tr.t[0].u, tr_lst->tr.t[0].v, tr_lst->tr.t[0].w,
						 tr_lst->tr.v[1].x, tr_lst->tr.v[1].y, tr_lst->tr.t[1].u, tr_lst->tr.t[1].v, tr_lst->tr.t[1].w,
						 tr_lst->tr.v[2].x, tr_lst->tr.v[2].y, tr_lst->tr.t[2].u, tr_lst->tr.t[2].v, tr_lst->tr.t[2].w);
		//fill_triangle(app, tr_lst->tr);
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

	cross_color = color_new(r, g, b);
	cross_hl_s = new_vector(app->sdl->half_width - size, app->sdl->half_height, 0.0);
	cross_hl_e = new_vector(app->sdl->half_width + size, app->sdl->half_height, 0.0);
	cross_vl_s = new_vector(app->sdl->half_width, app->sdl->half_height - size, 0.0);
	cross_vl_e = new_vector(app->sdl->half_width, app->sdl->half_height + size, 0.0);
	draw_line(app, &cross_hl_s, &cross_hl_e, cross_color);
	draw_line(app, &cross_vl_s, &cross_vl_e, cross_color);
}