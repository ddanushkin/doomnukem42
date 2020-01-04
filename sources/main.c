#include "doom_nukem.h"

void	reset_screen(register t_app *app)
{
	int						i;
	register t_depth_chunk	*depth_buffer;
	register t_depth_chunk	depth_chunk;
	register t_screen_chunk	*screen_buffer;
	register t_screen_chunk	screen_chunk;

	depth_buffer = app->depth_chunk_array;
	depth_chunk = app->depth_chunk;
	screen_buffer = app->screen_chunk_array;
	screen_chunk = app->screen_chunk;
	i = 0;
	while (i++ < SCREEN_H)
	{
		*depth_buffer++ = depth_chunk;
		*screen_buffer++ = screen_chunk;
	}
}

void 	print_to_screen(t_app *app, int x, int y, char *text)
{
	SDL_Color	*font_color;
	SDL_Surface	*font_surface;
	SDL_Rect	rect;
	Uint32 		c;

	c = 0xffffff;
	font_color = (SDL_Color *)(&c);
	font_surface = TTF_RenderText_Solid(app->font, text, *font_color);
	rect.x = x;
	rect.y = y;
	rect.w = font_surface->w;
	rect.h = font_surface->h;
	SDL_BlitSurface(font_surface, &font_surface->clip_rect, app->sdl->surface, &rect);
	SDL_FreeSurface(font_surface);
}

void	update_fps_text(t_app *app)
{
	char	*fps_text;

	fps_text = ft_itoa(app->timer->fps);
	print_to_screen(app, 0, 0, fps_text);
	ft_strdel(&fps_text);
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

	edge.y_start = (int)(min.y + 1);
	edge.y_end = (int)(max.y + 1);

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
	int		x_start;
	int		x_end;
	int		offset;

	x_start = (int)(left->x);
	x_end = (int)(right->x);

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

	offset = y * SCREEN_W + x_start;
	while (x_start < x_end)
	{
		if (depth < app->depth_buffer[offset])
		{
			double scale = 1.0;
			double z = 1.0 / tex_z * scale * 256;
			uint8_t *color;
			uint32_t c;

			Uint32 img_x = (Uint32)(tex_x * z) % 256;
			Uint32 img_y = (Uint32)(tex_y * z) % 256;
			c = app->sprites[0].pixels[((img_y << 8u) + img_x)];
			if (app->tr_hitted)
			{
				color = (uint8_t *)&c;
				color[0] = color[0] / 2;
				color[1] = color[1] / 2;
				color[2] = color[2] / 2;
			}
			app->depth_buffer[offset] = depth;
			set_pixel_uint32(app->sdl->surface, offset, c);
		}
		tex_x += x_x_step;
		tex_y += y_x_step;
		tex_z += z_x_step;
		depth += depth_step;
		x_start++;
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
	y_start = b->y_start;
	y_end = b->y_end;
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

double	triangle_area(t_v3d *a, t_v3d *b, t_v3d *c)
{
	return ((b->x - a->x) * (c->y - a->y) - (c->x - a->x) * (b->y - a->y));
}

void vertex_perspective_divide(t_v3d *v)
{
	v->x /= v->w;
	v->y /= v->w;
	v->z /= v->w;
}

int		inside_view(t_v3d v1, t_v3d v2, t_v3d v3)
{
	return (fabs(v1.x) <= fabs(v1.w) &&
			fabs(v1.y) <= fabs(v1.w) &&
			fabs(v1.z) <= fabs(v1.w) &&
			fabs(v2.x) <= fabs(v2.w) &&
			fabs(v2.y) <= fabs(v2.w) &&
			fabs(v2.z) <= fabs(v2.w) &&
			fabs(v3.x) <= fabs(v3.w) &&
			fabs(v3.y) <= fabs(v3.w) &&
			fabs(v3.z) <= fabs(v3.w));
}

int		outside_view(t_v3d v1, t_v3d v2, t_v3d v3)
{
	return (fabs(v1.x) > fabs(v1.w) &&
			fabs(v1.y) > fabs(v1.w) &&
			fabs(v1.z) > fabs(v1.w) &&
			fabs(v2.x) > fabs(v2.w) &&
			fabs(v2.y) > fabs(v2.w) &&
			fabs(v2.z) > fabs(v2.w) &&
			fabs(v3.x) > fabs(v3.w) &&
			fabs(v3.y) > fabs(v3.w) &&
			fabs(v3.z) > fabs(v3.w));
}

void 	fill_triangle(t_app *app, t_v3d v1, t_v3d v2, t_v3d v3)
{
	v1 = matrix_transform(app->camera->screen_space, v1);
	v2 = matrix_transform(app->camera->screen_space, v2);
	v3 = matrix_transform(app->camera->screen_space, v3);
	vertex_perspective_divide(&v1);
	vertex_perspective_divide(&v2);
	vertex_perspective_divide(&v3);
	if (triangle_area(&v1, &v3, &v2) >= 0.0)
		return;
	if (v3.y < v2.y)
		SWAP(v2, v3, t_v3d);
	if (v2.y < v1.y)
		SWAP(v1, v2, t_v3d);
	if (v3.y < v2.y)
		SWAP(v2, v3, t_v3d);
	scan_triangle(app, v1, v2, v3, triangle_area(&v1, &v3, &v2) >= 0.0);
//	if (app->tr_hitted)
//	{
//		draw_line(app, &v1, &v2, 0xffffff);
//		draw_line(app, &v2, &v3, 0xffffff);
//		draw_line(app, &v3, &v1, 0xffffff);
//	}
}

void	vr_list_add(t_vr_list **list, t_v3d v)
{
	t_vr_list	*head;

	if ((*list) == NULL)
	{
		(*list) = (t_vr_list *)malloc(sizeof(t_vr_list));
		(*list)->v = v;
		(*list)->next = NULL;
	}
	else
	{
		head = *list;
		while ((*list)->next)
			(*list) = (*list)->next;
		(*list)->next = (t_vr_list *)malloc(sizeof(t_vr_list));
		(*list) = (*list)->next;
		(*list)->v = v;
		(*list)->next = NULL;
		*list = head;
	}
}

void 	vr_list_free(t_vr_list **list)
{
	t_vr_list *tmp;

	while ((*list) != NULL)
	{
		tmp = (*list);
		(*list) = (*list)->next;
		free(tmp);
	}
	(*list) = NULL;
}

t_vr_list	*vr_list_last(t_vr_list *head)
{
	if (head == NULL)
		return (NULL);
	while (head->next)
		head = head->next;
	return (head);
}

t_v3d	lerp(t_v3d start, t_v3d end, double amount)
{
	t_v3d result;

	result.x = (end.x - start.x) * amount + start.x;
	result.y = (end.y - start.y) * amount + start.y;
	result.z = (end.z - start.z) * amount + start.z;
	result.w = (end.w - start.w) * amount + start.w;
	result.tex_x = (end.tex_x - start.tex_x) * amount + start.tex_x;
	result.tex_y = (end.tex_y - start.tex_y) * amount + start.tex_y;
	return (result);
}

double	get_lerp_amount(t_clip_data *prev, t_clip_data *curr)
{
	double		tmp;

	tmp = prev->v.w - prev->value;
	return ((tmp) / ((tmp) - (curr->v.w - curr->value)));
}

void 	clip_axis(t_vr_list **start_list, double factor, t_vr_list **end_list, Uint8 index)
{
	t_vr_list	*cursor;
	t_clip_data	prev;
	t_clip_data	curr;

	cursor = (*start_list);
	prev.v = vr_list_last(*start_list)->v;
	prev.value = ((double *)(&prev.v))[index] * factor;
	prev.is_inside = prev.value <= prev.v.w;
	while (cursor)
	{
		curr.v = cursor->v;
		curr.value = ((double *)(&curr.v))[index] * factor;
		curr.is_inside = curr.value <= curr.v.w;
		if (curr.is_inside ^ prev.is_inside)
		{
			vr_list_add(end_list,
				lerp(prev.v, curr.v,
					get_lerp_amount(&prev, &curr)));
		}
		if (curr.is_inside)
			vr_list_add(end_list, curr.v);
		SWAP(prev, curr, t_clip_data);
		cursor = cursor->next;
	}
}

int 	clip_by_axis(t_vr_list **start_list, t_vr_list **end_list, Uint8 index)
{
	clip_axis(start_list, 1.0, end_list, index);
	vr_list_free(start_list);
	if ((*end_list) == NULL)
		return (0);
	clip_axis(end_list, -1.0, start_list, index);
	vr_list_free(end_list);
	return ((*start_list) != NULL);
}

void 	clip_fill_triangle(t_app *app, t_v3d v1, t_v3d v2, t_v3d v3)
{
	t_v3d		root;
	t_vr_list	*node;
	t_vr_list	*start_list;
	t_vr_list	*end_list;

	start_list = NULL;
	end_list = NULL;
	vr_list_add(&start_list, v1);
	vr_list_add(&start_list, v2);
	vr_list_add(&start_list, v3);
	if (clip_by_axis(&start_list, &end_list, 0) &&
		clip_by_axis(&start_list, &end_list, 1) &&
		clip_by_axis(&start_list, &end_list, 2))
	{
		root = start_list->v;
		node = start_list;
		start_list = start_list->next;
		free(node);
		while (start_list->next)
		{
			node = start_list;
			fill_triangle(app, root, node->v, node->next->v);
			start_list = start_list->next;
			free(node);
		}
		free(start_list);
	}
}

t_mat4x4 	get_transform_matrix(t_mat4x4 view_projection)
{
	return (matrix_multiply(
			view_projection,
			matrix_multiply(
				matrix_translation(0, 0, 0),
				matrix_rotation(0.0, 0.0, 0.0))));
}

int		ray_triangle_intersect(t_camera *camera, t_v3d v0, t_v3d v1, t_v3d v2)
{
	t_v3d	v0v1;
	t_v3d	v0v2;
	t_v3d	qvec;
	t_v3d	tvec;
	t_v3d	pvec;
	double	u;
	double	v;
	double	t;
	double	det;
	double	invDet;

	v0v1 = vector_sub(v1, v0);
	v0v2 = vector_sub(v2, v0);
	pvec = vector_cross_product(camera->dir, v0v2);
	det = vector_dot_product(v0v1, pvec);
	if (det < 0.0)
		return 0;
	if (fabs(det) < 0.0)
		return 0;
	invDet = 1 / det;
	tvec = vector_sub(camera->pos, v0);
	u = vector_dot_product(tvec, pvec) * invDet;
	if (u < 0.0 || u > 1.0)
		return 0;
	qvec = vector_cross_product(tvec, v0v1);
	v = vector_dot_product(camera->dir, qvec) * invDet;
	if (v < 0.0 || u + v > 1.0)
		return 0;
	t = vector_dot_product(v0v2, qvec) * invDet;
	return 1;
}

void	render_triangle(t_app *app, t_v3d v1, t_v3d v2, t_v3d v3)
{
	v1 = matrix_transform(app->camera->transform, v1);
	v2 = matrix_transform(app->camera->transform, v2);
	v3 = matrix_transform(app->camera->transform, v3);
	if (inside_view(v1, v2, v3))
	{
		fill_triangle(app, v1, v2, v3);
		return;
	}
	if (outside_view(v1, v2, v3))
		return;
	clip_fill_triangle(app, v1, v2, v3);
}

void 	render_wall(t_app *app, t_wall *w)
{
	app->tr_hitted = 0;
	if (w->is_null || w->v_count != 4)
		return ;
	if (ray_triangle_intersect(app->camera, w->v[0], w->v[1], w->v[2]))
		app->tr_hitted = 1;
	if (ray_triangle_intersect(app->camera, w->v[0], w->v[3], w->v[1]))
		app->tr_hitted = 1;
	render_triangle(app, w->v[0], w->v[1], w->v[2]);
	render_triangle(app, w->v[0], w->v[3], w->v[1]);
	if (app->tr_hitted)
		print_to_screen(app, SCREEN_W * 0.5, SCREEN_H * 0.5 - 15, "WALL");
}

void 	draw_cross(t_app *app, int x, int y, double size, Uint32 color)
{
	int	len;
	int	i;
	int start_x;
	int start_y;

	if (size <= 0)
		return ;
	i = 0;
	len = (int)(64.0 / size);
	len = CLAMP(len, 2, 64);
	start_x = x - (int)(len * 0.5);
	start_y = y - (int)(len * 0.5);
	while (i < len)
	{
		set_pixel_uint32(app->sdl->surface, (start_y + i) * SCREEN_W + x, color);
		set_pixel_uint32(app->sdl->surface, y * SCREEN_W + (start_x + i), color);
		i++;
	}
}

double	vector_dist(t_v3d v1, t_v3d v2)
{
	double	dx;
	double	dy;
	double	dz;

	dx = v2.x - v1.x;
	dy = v2.y - v1.y;
	dz = v2.z - v1.z;
	return sqrt((dx * dx) + (dy * dy) + (dz * dz));
}

void	start_the_game(t_app *app)
{
	SDL_SetRelativeMouseMode(SDL_TRUE);
	app->camera->pos = new_vector(0.0, 0.0, -10);
	app->camera->rot = new_vector(0.0, 0.0, 0.0);
	app->camera->fov = TO_RAD(90.0);
	app->camera->projection = matrix_perspective(
			app->camera->fov,
			(double)SCREEN_W / (double)SCREEN_H,
			0.05,
			1000.0);

	t_wall	wall;
	wall.is_null = 1;
	wall.v_count = 0;
	wall.v[0] = new_vector(0, 0, 0);
	wall.v[1] = wall.v[0];
	wall.v[2] = wall.v[0];
	wall.v[3] = wall.v[0];

	app->timer->curr = SDL_GetPerformanceCounter();
	while (1)
	{
		if (!event_handling(app))
			break;
		get_delta_time(app->timer);
		if (app->inputs->keyboard[SDL_SCANCODE_ESCAPE])
			break ;
		process_inputs(app, app->timer->delta);
		update_camera(app->camera);
		reset_screen(app);

		wall.v[0].x = -1.0;
		wall.v[0].y = -1.0;
		wall.v[0].z = 0.0;
		wall.v[0].w = 1.0;
		wall.v[0].tex_x = 0.0;
		wall.v[0].tex_y = 0.0;

		wall.v[1].x = 1.0;
		wall.v[1].y = 1.0;
		wall.v[1].z = 0.0;
		wall.v[1].w = 1.0;
		wall.v[1].tex_x = 1.0;
		wall.v[1].tex_y = 1.0;

		wall.v[2].x = 1.0;
		wall.v[2].y = -1.0;
		wall.v[2].z = 0.0;
		wall.v[2].w = 1.0;
		wall.v[2].tex_x = 1.0;
		wall.v[2].tex_y = 0.0;

		wall.v[3].x = -1.0;
		wall.v[3].y = 1.0;
		wall.v[3].z = 0.0;
		wall.v[3].w = 1.0;
		wall.v[3].tex_x = 0.0;
		wall.v[3].tex_y = 1.0;

		wall.is_null = 0;
		wall.v_count = 4;

//		for (int i = 0; i < 4; i++)
//		{
//			t_v3d tmp = wall.v[i];
//			tmp = matrix_transform(app->camera->transform, tmp);
//			double dist = vector_dist(app->camera->pos, tmp);
//			if (fabs(tmp.x) <= fabs(tmp.w) &&
//				fabs(tmp.y) <= fabs(tmp.w) &&
//				fabs(tmp.z) <= fabs(tmp.w))
//			{
//				tmp = matrix_transform(app->camera->screen_space, tmp);
//				vertex_perspective_divide(&tmp);
//				draw_cross(app, (int)tmp.x, (int)tmp.y, dist, 0xff0000);
//			}
//		}

		render_wall(app, &wall);
		draw_cross(app, (int)app->sdl->half_width, (int)app->sdl->half_height, 8, 0xffffff);
		update_fps_text(app);
		SDL_UpdateWindowSurface(app->sdl->window);
	}
	TTF_CloseFont(app->font);
	TTF_Quit();
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
	app->camera = (t_camera *)malloc(sizeof(t_camera));
	app->depth_buffer = (double *)malloc(sizeof(double) * SCREEN_W * SCREEN_H);
	init_app(app);

	/* TODO: Set number of meshes */
	int number_of_meshes = 2;
	app->meshes = (t_mesh *)malloc(sizeof(t_mesh) * number_of_meshes);
	//obj_load("resources/plane.obj", &app->meshes[0]);
	//obj_load("resources/cube.obj", &app->meshes[1]);

	/* TODO: Set number of meshes */
	int number_of_sprites = 1;
	app->sprites = (t_sprite *)malloc(sizeof(t_sprite) * number_of_sprites);
	bmp_load("resources/3.bmp", &app->sprites[0]);

	start_the_game(app);
	quit_properly(app);
	return (0);
}

