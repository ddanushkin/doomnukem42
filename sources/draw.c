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
	tr->light_dp = MAX(0.2, vector_dot_product(normal, vector_normalise(new_vector(0.0, 0.0, 1.0))));
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
	if (vector_dot_product(normal, camera_ray) > 0.0)
	{
		calc_light(tr, normal);
		tr->visible = 1;
	}
}

void TexturedTriangle(t_app *app, t_tex_tr *tr)
{
	if (tr->v[1].y < tr->v[0].y)
		SWAP(tr->v[0], tr->v[1], t_tex_v);
	if (tr->v[2].y < tr->v[0].y)
		SWAP(tr->v[0], tr->v[2], t_tex_v);
	if (tr->v[2].y < tr->v[1].y)
		SWAP(tr->v[1], tr->v[2], t_tex_v);

	t_tex_v dv1;
	double	dv1_abs_y;
	dv1.x = tr->v[1].x - tr->v[0].x;
	dv1.y = tr->v[1].y - tr->v[0].y;
	dv1.u = tr->v[1].u - tr->v[0].u;
	dv1.v = tr->v[1].v - tr->v[0].v;
	dv1.w = tr->v[1].w - tr->v[0].w;
	dv1_abs_y = 1.0 / (double)abs(dv1.y);

	t_tex_v dv2;
	double	dv2_abs_y;
	dv2.x = tr->v[2].x - tr->v[0].x;
	dv2.y = tr->v[2].y - tr->v[0].y;
	dv2.u = tr->v[2].u - tr->v[0].u;
	dv2.v = tr->v[2].v - tr->v[0].v;
	dv2.w = tr->v[2].w - tr->v[0].w;
	dv2_abs_y = 1.0 / (double)abs(dv2.y);

	double dax_step = 0;
	double dbx_step = 0;

	double du1_step = 0;
	double dv1_step = 0;
	double dw1_step = 0;

	double du2_step = 0;
	double dv2_step = 0;
	double dw2_step = 0;

	if (dv2.y)
	{
		dbx_step = dv2.x * dv2_abs_y;
		du2_step = dv2.u * dv2_abs_y;
		dv2_step = dv2.v * dv2_abs_y;
		dw2_step = dv2.w * dv2_abs_y;
	}

	if (dv1.y)
	{
		dax_step = dv1.x * dv1_abs_y;
		du1_step = dv1.u * dv1_abs_y;
		dv1_step = dv1.v * dv1_abs_y;
		dw1_step = dv1.w * dv1_abs_y;
		for (int i = tr->v[0].y; i <= tr->v[1].y; i++)
		{
			double i_minus_v0y = (double)(i - tr->v[0].y);
			int ax = tr->v[0].x + i_minus_v0y * dax_step;
			int bx = tr->v[0].x + i_minus_v0y * dbx_step;
			double tex_su = tr->v[0].u + i_minus_v0y * du1_step;
			double tex_sv = tr->v[0].v + i_minus_v0y * dv1_step;
			double tex_sw = tr->v[0].w + i_minus_v0y * dw1_step;
			double tex_eu = tr->v[0].u + i_minus_v0y * du2_step;
			double tex_ev = tr->v[0].v + i_minus_v0y * dv2_step;
			double tex_ew = tr->v[0].w + i_minus_v0y * dw2_step;

			if (ax > bx)
			{
				SWAP(ax, bx, int);
				SWAP(tex_su, tex_eu, double);
				SWAP(tex_sv, tex_ev, double);
				SWAP(tex_sw, tex_ew, double);
			}

			double tstep = 1.0 / (bx - ax);
			double t = 0.0;

			int offset = i * SCREEN_W + ax;
			for (int j = ax; j < bx; j++)
			{
				double tex_u, tex_v, tex_w;
				tex_u = (1.0 - t) * tex_su + t * tex_eu;
				tex_v = (1.0 - t) * tex_sv + t * tex_ev;
				tex_w = (1.0 - t) * tex_sw + t * tex_ew;
				if (tex_w > app->z_buf[offset])
				{
					app->z_buf[offset] = tex_w;
					tex_w = 1.0 / tex_w * 255.0;
					set_pixel_uint32(
							app->sdl->surface,
							offset,
							app->sprites[0].pixels[((int)(tex_v * tex_w) << 8) + (int)(tex_u * tex_w)]);
				}
				offset++;
				t += tstep;
			}

		}
	}

	dv1.x = tr->v[2].x - tr->v[1].x;
	dv1.y = tr->v[2].y - tr->v[1].y;
	dv1.u = tr->v[2].u - tr->v[1].u;
	dv1.v = tr->v[2].v - tr->v[1].v;
	dv1.w = tr->v[2].w - tr->v[1].w;
	dv1_abs_y = 1 / (double)abs(dv1.y);

	if (dv2.y)
		dbx_step = dv2.x * dv2_abs_y;

	if (dv1.y)
	{
		dax_step = dv1.x * dv1_abs_y;
		du1_step = dv1.u * dv1_abs_y;
		dv1_step = dv1.v * dv1_abs_y;
		dw1_step = dv1.w * dv1_abs_y;
		for (int i = tr->v[1].y; i <= tr->v[2].y; i++)
		{
			double i_minus_v0y = (double)(i - tr->v[0].y);
			double i_minus_v1y = (double)(i - tr->v[1].y);
			int ax = tr->v[1].x + (double)(i - tr->v[1].y) * dax_step;
			int bx = tr->v[0].x + i_minus_v0y * dbx_step;
			double tex_su = tr->v[1].u + i_minus_v1y * du1_step;
			double tex_sv = tr->v[1].v + i_minus_v1y * dv1_step;
			double tex_sw = tr->v[1].w + i_minus_v1y * dw1_step;
			double tex_eu = tr->v[0].u + i_minus_v0y * du2_step;
			double tex_ev = tr->v[0].v + i_minus_v0y * dv2_step;
			double tex_ew = tr->v[0].w + i_minus_v0y * dw2_step;

			if (ax > bx)
			{
				SWAP(ax, bx, int);
				SWAP(tex_su, tex_eu, double);
				SWAP(tex_sv, tex_ev, double);
				SWAP(tex_sw, tex_ew, double);
			}

			double tstep = 1.0 / (bx - ax);
			double t = 0.0;

			int offset = i * SCREEN_W + ax;
			for (int j = ax; j < bx; j++)
			{
				double tex_u, tex_v, tex_w;
				tex_u = (1.0 - t) * tex_su + t * tex_eu;
				tex_v = (1.0 - t) * tex_sv + t * tex_ev;
				tex_w = (1.0 - t) * tex_sw + t * tex_ew;
				if (tex_w > app->z_buf[offset])
				{
					app->z_buf[offset] = tex_w;
					tex_w = 1.0 / tex_w * 255.0;
					set_pixel_uint32(
							app->sdl->surface,
							offset,
							app->sprites[0].pixels[((int)(tex_v * tex_w) << 8) + (int)(tex_u * tex_w)]);
				}
				offset++;
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
		tr_lst->tr.light_dp = tr.light_dp;

		t_tex_tr tex_tr;

		tex_tr.v[0].x = (int)tr_lst->tr.v[0].x;
		tex_tr.v[1].x = (int)tr_lst->tr.v[1].x;
		tex_tr.v[2].x = (int)tr_lst->tr.v[2].x;
		tex_tr.v[0].y = (int)tr_lst->tr.v[0].y;
		tex_tr.v[1].y = (int)tr_lst->tr.v[1].y;
		tex_tr.v[2].y = (int)tr_lst->tr.v[2].y;
		tex_tr.v[0].u = tr_lst->tr.t[0].u;
		tex_tr.v[1].u = tr_lst->tr.t[1].u;
		tex_tr.v[2].u = tr_lst->tr.t[2].u;
		tex_tr.v[0].v = tr_lst->tr.t[0].v;
		tex_tr.v[1].v = tr_lst->tr.t[1].v;
		tex_tr.v[2].v = tr_lst->tr.t[2].v;
		tex_tr.v[0].w = tr_lst->tr.t[0].w;
		tex_tr.v[1].w = tr_lst->tr.t[1].w;
		tex_tr.v[2].w = tr_lst->tr.t[2].w;
		tex_tr.s = &app->sprites[0];

		TexturedTriangle(app, &tex_tr);
		//fill_triangle(app, tr_lst->tr);
		//draw_outline(app, tr_lst->tr);
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