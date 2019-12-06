#include "doom_nukem.h"

void	clear_screen(t_app *app)
{
	image_clear(app->sdl->surface->pixels, 0, app->sdl->pixels_len);
}

void	clear_depth_buffer(t_app *app)
{
	int i;
	int len;

	i = 0;
	len = SCREEN_W * SCREEN_H;
	while (i < len)
	{
		app->depth_buffer[i] = INFINITY;
		i++;
	}
}

void	update_fps_data(t_app *app, TTF_Font *font_ptr, SDL_Color font_color)
{
	char fps_text[50];
	itoa(app->timer->fps, fps_text, 10);
	SDL_Surface *font_surface = TTF_RenderText_Solid(font_ptr, fps_text, font_color);
	SDL_BlitSurface(font_surface, NULL, app->sdl->surface, NULL);
	SDL_FreeSurface(font_surface);
}

double 		gradient_calc_x_step(double coords[3], t_v3d min, t_v3d mid, t_v3d max, double one_over_dx)
{
	return (((coords[1] - coords[2]) * (min.y - max.y) - (coords[0] - coords[2]) * (mid.y - max.y)) * one_over_dx);
}

double 		gradient_calc_y_step(double coords[3], t_v3d min, t_v3d mid, t_v3d max, double one_over_dy)
{
	return (((coords[1] - coords[2]) * (min.x - max.x) - (coords[0] - coords[2]) * (mid.x - max.x)) * one_over_dy);
}

t_gradient	gradient_new(t_v3d min, t_v3d mid, t_v3d max)
{
	t_gradient	g;

	g.z[0] = 1.0 / min.w;
	g.z[1] = 1.0 / mid.w;
	g.z[2] = 1.0 / max.w;
	g.x[0] = min.tex_x * g.z[0];
	g.x[1] = mid.tex_x * g.z[1];
	g.x[2] = max.tex_x * g.z[2];
	g.y[0] = min.tex_y * g.z[0];
	g.y[1] = mid.tex_y * g.z[1];
	g.y[2] = max.tex_y * g.z[2];
	g.depth[0] = min.z;
	g.depth[1] = mid.z;
	g.depth[2] = max.z;
	g.one_over_dx  = 1.0 / ((mid.x - max.x) * (min.y - max.y) - (min.x - max.x) * (mid.y - max.y));
	g.one_over_dy = -g.one_over_dx;
	g.x_x_step = gradient_calc_x_step(g.x, min, mid, max, g.one_over_dx);
	g.x_y_step = gradient_calc_y_step(g.x, min, mid, max, g.one_over_dy);
	g.y_x_step = gradient_calc_x_step(g.y, min, mid, max, g.one_over_dx);
	g.y_y_step = gradient_calc_y_step(g.y, min, mid, max, g.one_over_dy);
	g.z_x_step = gradient_calc_x_step(g.z, min, mid, max, g.one_over_dx);
	g.z_y_step = gradient_calc_y_step(g.z, min, mid, max, g.one_over_dy);
	g.depth_x_step = gradient_calc_x_step(g.depth, min, mid, max, g.one_over_dx);
	g.depth_y_step = gradient_calc_y_step(g.depth, min, mid, max, g.one_over_dy);
	return (g);
}

t_edge	edge_new(t_gradient	g, t_v3d min, t_v3d max, int index)
{
	t_edge		edge;
	double		y_dist;
	double		x_dist;
	double		y_pre_step;
	double		x_pre_step;

	edge.y_start = (int)ceil(min.y);
	edge.y_end = (int)ceil(max.y);

	y_dist = max.y - min.y;
	x_dist = max.x - min.x;

	y_pre_step = edge.y_start - min.y;
	edge.x_step = x_dist / y_dist;
	edge.x = min.x + y_pre_step * edge.x_step;
	x_pre_step = edge.x - min.x;

	edge.tex_x = g.x[index] + g.x_x_step * x_pre_step + g.x_y_step * y_pre_step;
	edge.tex_x_step = g.x_y_step + g.x_x_step * edge.x_step;

	edge.tex_y = g.y[index] + g.y_x_step * x_pre_step + g.y_y_step * y_pre_step;
	edge.tex_y_step = g.y_y_step + g.y_x_step * edge.x_step;

	edge.tex_z = g.z[index] + g.z_x_step * x_pre_step + g.z_y_step * y_pre_step;
	edge.tex_z_step = g.z_y_step + g.z_x_step * edge.x_step;

	edge.depth = g.depth[index] + g.depth_x_step * x_pre_step + g.depth_y_step * y_pre_step;
	edge.depth_step = g.depth_y_step + g.depth_x_step * edge.x_step;
	return (edge);
}

void	edge_step(t_edge *edge)
{
	edge->x += edge->x_step;
	edge->tex_x += edge->tex_x_step;
	edge->tex_y += edge->tex_y_step;
	edge->tex_z += edge->tex_z_step;
	edge->depth += edge->depth_step;
}

void	draw_scanline(t_app *app, t_edge *left, t_edge *right, int y)
{
	int x;
	int x_start;
	int x_end;
	int offset;

	x_start = MAX((int)ceil(left->x), 0);
	x_end = MIN((int)ceil(right->x), SCREEN_W - 1);

	double	x_pre_step = x_start - left->x;
	double	x_dist = right->x - left->x;
	double	x_x_step = (right->tex_x - left->tex_x) / x_dist;
	double	y_x_step = (right->tex_y - left->tex_y) / x_dist;
	double	z_x_step = (right->tex_z - left->tex_z) / x_dist;
	double 	depth_step = (right->depth - left->depth) / x_dist;
	double	tex_x = left->tex_x + x_x_step * x_pre_step;
	double	tex_y = left->tex_y + y_x_step * x_pre_step;
	double	tex_z = left->tex_z + z_x_step * x_pre_step;
	double	depth = left->depth + depth_step * x_pre_step;

	x = x_start;
	offset = y * SCREEN_W + x;
	while (x < x_end)
	{
		if (depth < app->depth_buffer[offset])
		{
			double z = 1.0 / tex_z * 255.5;
			int img_x = (int)(tex_x * z);
			int img_y = (int)(tex_y * z);

			app->depth_buffer[offset] = depth;
			set_pixel_uint32(app->sdl->surface, offset, sprite_get_color(&app->sprites[0], img_x, img_y));
		}
		tex_x += x_x_step;
		tex_y += y_x_step;
		tex_z += z_x_step;
		depth += depth_step;
		x++;
		offset++;
	}
}

void	scan_edges(t_app *app, t_edge *a, t_edge *b, int handedness)
{
	t_edge	*left;
	t_edge	*right;
	int		y_start;
	int		y_end;
	int		y;

	left = a;
	right = b;
	if (handedness)
		SWAP(left, right, t_edge *);
	y_start = MAX(b->y_start, 0);
	y_end = MIN(b->y_end, SCREEN_H - 1);
	y = y_start;
	while (y < y_end)
	{
		draw_scanline(app, left, right, y);
		edge_step(left);
		edge_step(right);
		y++;
	}
}

void	scan_triangle(t_app *app, t_v3d min, t_v3d mid, t_v3d max, int handedness)
{
	t_edge		top_to_bottom;
	t_edge		top_to_middle;
	t_edge		middle_to_bottom;
	t_gradient	gradient;

	gradient = gradient_new(min, mid, max);
	top_to_bottom = edge_new(gradient, min, max, 0);
	top_to_middle = edge_new(gradient, min, mid, 0);
	middle_to_bottom = edge_new(gradient, mid, max, 1);
	scan_edges(app, &top_to_bottom, &top_to_middle, handedness);
	scan_edges(app, &top_to_bottom, &middle_to_bottom, handedness);
}

double	triangle_area_times_two(t_v3d *a, t_v3d *b, t_v3d *c)
{
	return ((b->x - a->x) * (c->y - a->y) - (c->x - a->x) * (b->y - a->y));
}

t_v3d vertex_perspective_divide(t_v3d v)
{
	t_v3d new_v;

	new_v.x = v.x / v.w;
	new_v.y = v.y / v.w;
	new_v.z = v.z / v.w;
	new_v.w = v.w;
	new_v.tex_x = v.tex_x;
	new_v.tex_y = v.tex_y;
	return (new_v);
}

int		inside_view_frustum(t_v3d v)
{
//	TODO: fix z check
//	return (fabs(v.x) <= fabs(v.w) &&
//			fabs(v.y) <= fabs(v.w) &&
//			fabs(v.z) <= fabs(v.w));
	return (fabs(v.x) <= fabs(v.w) &&
			fabs(v.y) <= fabs(v.w));
}

void 	fill_triangle(t_app *app, t_v3d v1, t_v3d v2, t_v3d v3)
{
	t_mat4x4	screen_space_mat;

	screen_space_mat = matrix_screen_space();

	v1 = matrix_transform(screen_space_mat, v1);
	v2 = matrix_transform(screen_space_mat, v2);
	v3 = matrix_transform(screen_space_mat, v3);

	v1 = vertex_perspective_divide(v1);
	v2 = vertex_perspective_divide(v2);
	v3 = vertex_perspective_divide(v3);

	/* Triangle is facing camera check */
	if (triangle_area_times_two(&v1, &v3, &v2) >= 0.0)
		return;

	if (v3.y < v2.y)
		SWAP(v2, v3, t_v3d);
	if (v2.y < v1.y)
		SWAP(v1, v2, t_v3d);
	if (v3.y < v2.y)
		SWAP(v2, v3, t_v3d);

	scan_triangle(
			app,
			v1,
			v2,
			v3,
			triangle_area_times_two(&v1, &v3, &v2) >= 0.0);}


void	render_pipeline(t_app *app, t_v3d v1, t_v3d v2, t_v3d v3, t_mat4x4 view_projection)
{
	t_mat4x4	translation_mat;
	t_mat4x4	rotation_mat;
	t_mat4x4	transform_mat;

	//Mesh move, rotate
	translation_mat = matrix_translation(0, 0, 10);
	rotation_mat = matrix_rotation(0.0, 0.0, 0.0);
	transform_mat = matrix_multiply(view_projection, matrix_multiply(translation_mat, rotation_mat));

	v1 = matrix_transform(transform_mat, v1);
	v2 = matrix_transform(transform_mat, v2);
	v3 = matrix_transform(transform_mat, v3);
	
	if (inside_view_frustum(v1) && inside_view_frustum(v2) && inside_view_frustum(v3))
	{
		fill_triangle(app, v1, v2, v3);
		return;
	}
	else
	{
		//Clipping
		fill_triangle(app, v1, v2, v3);
	}

}

void	start_the_game(t_app *app)
{
	TTF_Init();
	TTF_Font *font_ptr = TTF_OpenFont("resources/calibrib.ttf", 16);
	SDL_Color font_color = {255, 255, 255};

	SDL_SetRelativeMouseMode(SDL_TRUE);
	app->camera->pos = new_vector(0.0, 0.0, 3.46);
	app->camera->rot = new_vector(0.0, 0.0, 0.0);
	app->camera->projection = matrix_perspective(
			1.22173,
			(double)SCREEN_W / (double)SCREEN_H,
			0.1,
			1000.0);
	while (1)
	{
		get_ticks(app->timer);
		clear_screen(app);
		clear_depth_buffer(app);
		mouse_update(app);
		if (!event_handling(app, app->timer->delta))
			break;

		t_v3d vert1;
		t_v3d vert2;
		t_v3d vert3;

		vert1.x = -1.0;
		vert1.y = -1.0;
		vert1.tex_x = 0.0;
		vert1.tex_y = 0.0;
		vert1.z = 0.0;
		vert1.w = 1.0;

		vert2.x = 1.0;
		vert2.y = 1.0;
		vert2.tex_x = 1.0;
		vert2.tex_y = 1.0;
		vert2.z = 0.0;
		vert2.w = 1.0;

		vert3.x = 1.0;
		vert3.y = -1.0;
		vert3.tex_x = 1.0;
		vert3.tex_y = 0.0;
		vert3.z = 0.0;
		vert3.w = 1.0;

		//TODO: Clipping

		render_pipeline(app, vert1, vert2, vert3, app->camera->view_projection);

		vert3.x = -1.0;
		vert3.y = 1.0;
		vert3.tex_x = 0.0;
		vert3.tex_y = 1.0;
		vert3.z = 0.0;
		vert3.w = 1.0;

		render_pipeline(app, vert1, vert3, vert2, app->camera->view_projection);

		draw_cross(app, 7.0, 255, 0, 200);
		get_delta_time(app->timer);
		update_fps_data(app, font_ptr, font_color);
		SDL_UpdateWindowSurface(app->sdl->window);
	}
	TTF_CloseFont(font_ptr);
	SDL_Quit();
	SDL_DestroyWindow(app->sdl->window);
}

int	check_resources(void)
{
	int		fd;
	char 	hash[33];

	system("tar c ./resources/ | md5 > resources_hash");
	fd = open("resources_hash", O_RDONLY);
	if (fd < 3)
		exit_with_status(STATUS_FILE_NOT_FOUND, "resources_hash");
	hash[32] = '\0';
	read(fd, hash, 32);
	close(fd);
	printf("%s\n", hash);
	return (ft_strequ(hash, RESOURCES_MD5));
}

int		main(int argv, char**argc)
{
	t_app	*app;

	//if (!check_resources())
	//	exit_with_status(STATUS_BAD_RESOURCES, NULL);
	app = (t_app *)malloc(sizeof(t_app));
	app->sdl = (t_sdl *)malloc(sizeof(t_sdl));
	app->inputs = (t_inputs *)malloc(sizeof(t_inputs));
	app->timer = (t_timer *)malloc(sizeof(t_timer));
	app->world = (t_world *)malloc(sizeof(t_world));
	app->camera = (t_camera *)malloc(sizeof(t_camera));
	app->depth_buffer = (double *)malloc(sizeof(double) * SCREEN_W * SCREEN_H);
	init_app(app);

	/* TODO: Set number of meshes */
	int number_of_meshes = 2;
	app->meshes = (t_mesh *)malloc(sizeof(t_mesh) * number_of_meshes);
	obj_load("resources/plane.obj", &app->meshes[0]);
	obj_load("resources/cube.obj", &app->meshes[1]);

	/* TODO: Set number of meshes */
	int number_of_sprites = 1;
	app->sprites = (t_sprite *)malloc(sizeof(t_sprite) * number_of_sprites);
	bmp_load("resources/3.bmp", &app->sprites[0]);

	start_the_game(app);
	quit_properly(app);
	return (0);
}

