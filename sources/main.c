#include "doom_nukem.h"

void	clear_screen(t_app *app)
{
	image_clear(app->sdl->surface->pixels, 0, app->sdl->pixels_len);
}

void	clear_z_buffer(t_app *app)
{
	int i;
	int len;

	i = 0;
	len = SCREEN_W * SCREEN_H;
	while (i < len)
	{
		app->z_buf[i] = -INFINITY;
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
	t_gradient	gradient;

	gradient.one_over_dx  = 1.0 / ((mid.x - max.x) * (min.y - max.y) - (min.x - max.x) * (mid.y - max.y));
	gradient.one_over_dy = -gradient.one_over_dx;

	gradient.z[0] = 1.0 / min.w;
	gradient.z[1] = 1.0 / mid.w;
	gradient.z[2] = 1.0 / max.w;

	gradient.x[0] = min.tex_x * gradient.z[0];
	gradient.x[1] = mid.tex_x * gradient.z[1];
	gradient.x[2] = max.tex_x * gradient.z[2];

	gradient.y[0] = min.tex_y * gradient.z[0];
	gradient.y[1] = mid.tex_y * gradient.z[1];
	gradient.y[2] = max.tex_y * gradient.z[2];

	gradient.x_x_step = gradient_calc_x_step(gradient.x, min, mid, max, gradient.one_over_dx);
	gradient.x_y_step = gradient_calc_y_step(gradient.x, min, mid, max, gradient.one_over_dy);
	gradient.y_x_step = gradient_calc_x_step(gradient.y, min, mid, max, gradient.one_over_dx);
	gradient.y_y_step = gradient_calc_y_step(gradient.y, min, mid, max, gradient.one_over_dy);
	gradient.z_x_step = gradient_calc_x_step(gradient.z, min, mid, max, gradient.one_over_dx);
	gradient.z_y_step = gradient_calc_y_step(gradient.z, min, mid, max, gradient.one_over_dy);
	return (gradient);
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
	return (edge);
}

void	edge_step(t_edge *edge)
{
	edge->x += edge->x_step;
	edge->tex_x += edge->tex_x_step;
	edge->tex_y += edge->tex_y_step;
	edge->tex_z += edge->tex_z_step;
}

void	draw_scanline(t_app *app, t_edge *left, t_edge *right, int y, Uint32 c)
{
	int x;
	int x_min;
	int x_max;
	int offset;

	x_min = (int)ceil(left->x);
	x_max = (int)ceil(right->x);

	double	x_pre_step = x_min - left->x;
	double	x_dist = right->x - left->x;
	double	x_x_step = (right->tex_x - left->tex_x) / x_dist;
	double	y_x_step = (right->tex_y - left->tex_y) / x_dist;
	double	z_x_step = (right->tex_z - left->tex_z) / x_dist;
	double	tex_x = left->tex_x + x_x_step * x_pre_step;
	double	tex_y = left->tex_y + y_x_step * x_pre_step;
	double	tex_z = left->tex_z + z_x_step * x_pre_step;

	x = x_min;
	offset = y * SCREEN_W + x;
	while (x < x_max)
	{
		double z = 1.0 / tex_z * 255.5;
		int img_x = (int)(tex_x * z);
		int img_y = (int)(tex_y * z);

		set_pixel_uint32(app->sdl->surface, offset, sprite_get_color(&app->sprites[0], img_x, img_y));
		tex_x += x_x_step;
		tex_y += y_x_step;
		tex_z += z_x_step;
		x++;
		offset++;
	}
}

void	scan_edges(t_app *app, t_edge *a, t_edge *b, int handedness, Uint32 c)
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
	y_start = b->y_start;
	y_end = b->y_end;
	y = y_start;
	while (y < y_end)
	{
		draw_scanline(app, left, right, y, c);
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
	scan_edges(app, &top_to_bottom, &top_to_middle, handedness, 0x00FF0000);
	scan_edges(app, &top_to_bottom, &middle_to_bottom, handedness, 0x0000FF00);
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
	return (new_v);
}

void	render_pipeline(t_app *app, t_v3d vert1, t_v3d vert2, t_v3d vert3)
{
	t_v3d v1 = new_vector(vert1.x, vert1.y, vert1.z);
	double	tex_x1 = vert1.tex_x;
	double	tex_y1 = vert1.tex_y;

	t_v3d v2 = new_vector(vert2.x, vert2.y, vert2.z);
	double	tex_x2 = vert2.tex_x;
	double	tex_y2 = vert2.tex_y;

	t_v3d v3 = new_vector(vert3.x, vert3.y, vert3.z);
	double	tex_x3 = vert3.tex_x;
	double	tex_y3 = vert3.tex_y;

	t_mat4x4	projection_mat;
	projection_mat = matrix_perspective(
			1.22173,
			(double)SCREEN_W / (double)SCREEN_H,
			0.1,
			1000.0);

	t_mat4x4	translation_mat;
	translation_mat = matrix_translation(0.0, 0.0, 1.45);

	t_mat4x4	rotation_mat;
	rotation_mat = matrix_rotation(0.0, 0.0, 0.0);

	t_mat4x4	transform_mat;
	transform_mat = matrix_multiply(projection_mat, matrix_multiply(translation_mat, rotation_mat));

	v1 = matrix_transform(transform_mat, v1);
	v2 = matrix_transform(transform_mat, v2);
	v3 = matrix_transform(transform_mat, v3);

	t_mat4x4	screen_space_mat;
	screen_space_mat = matrix_screen_space();

	v1 = matrix_transform(screen_space_mat, v1);
	v2 = matrix_transform(screen_space_mat, v2);
	v3 = matrix_transform(screen_space_mat, v3);

	t_v3d min;
	t_v3d mid;
	t_v3d max;

	min = vertex_perspective_divide(v1);
	mid = vertex_perspective_divide(v2);
	max = vertex_perspective_divide(v3);

	min.tex_x = tex_x1;
	min.tex_y = tex_y1;
	mid.tex_x = tex_x2;
	mid.tex_y = tex_y2;
	max.tex_x = tex_x3;
	max.tex_y = tex_y3;

	if (max.y < mid.y)
		SWAP(mid, max, t_v3d);
	if (mid.y < min.y)
		SWAP(min, mid, t_v3d);
	if (max.y < mid.y)
		SWAP(mid, max, t_v3d);

	scan_triangle(
			app,
			min,
			mid,
			max,
			triangle_area_times_two(&min, &max, &mid) >= 0.0);
}

void	start_the_game(t_app *app)
{
	TTF_Init();
	TTF_Font *font_ptr = TTF_OpenFont("resources/calibrib.ttf", 16);
	SDL_Color font_color = {255, 255, 255};

	SDL_SetRelativeMouseMode(SDL_TRUE);
	app->camera->pos = new_vector(0.0, 0.0, 3.46);
	while (1)
	{
		get_ticks(app->timer);
		clear_screen(app);
		clear_z_buffer(app);
		mouse_update(app);
		if (!event_handling(app))
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

		render_pipeline(app, vert1, vert2, vert3);

		vert3.x = -1.0;
		vert3.y = 1.0;
		vert3.tex_x = 0.0;
		vert3.tex_y = 1.0;
		vert3.z = 0.0;
		vert3.w = 1.0;

		render_pipeline(app, vert1, vert2, vert3);

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
	app->z_buf = (double *)malloc(sizeof(double) * SCREEN_W * SCREEN_H);
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

