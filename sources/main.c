#include "doom_nukem.h"

void	reset_screen(t_app *app)
{
	int						i;
	t_depth_chunk	*depth_buffer;
	t_screen_chunk	*screen_buffer;

	depth_buffer = app->depth_chunk_array;
	screen_buffer = app->screen_chunk_array;
	i = 0;
	while (i++ < SCREEN_H)
	{
		*depth_buffer++ = app->depth_chunk;
		*screen_buffer++ = app->screen_chunk;
	}
}

int 	vertex_inside(t_v3d *v)
{
	return (fabs(v->x) <= fabs(v->w) &&
			fabs(v->y) <= fabs(v->w) &&
			fabs(v->z) <= fabs(v->w));
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

void	draw_scanline(t_app *app, t_edge *left, t_edge *right, int y)
{
	int		x_start;
	int		x_end;
	int		offset;

	x_start = (int)ceil(left->x);
	x_end = (int)ceil(right->x);

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
			double zx = 1.0 / tex_z * app->render_wall->scale_x * 256;
			double zy = 1.0 / tex_z * app->render_wall->scale_y * 256;
			uint32_t c;

			Uint32 img_x = (Uint32)(tex_x * zx) % 256;
			Uint32 img_y = (Uint32)(tex_y * zy) % 256;
			c = app->sprites[app->render_wall->sprite_index].pixels[((img_y << 8u) + img_x)];
//			uint8_t *color;
//			color = (uint8_t *)&c;
//			color[0] = color[0] / 2;
//			color[1] = color[1] / 2;
//			color[2] = color[2] / 2;
			if (c != TRANSPARENCY_COLOR)
			{
				app->depth_buffer[offset] = depth;
				set_pixel_uint32(app->sdl->surface, offset, c);
			}
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

int		inside_view(t_app *app, t_v3d v1, t_v3d v2, t_v3d v3)
{
	int		counter;
	double 	w1;
	double 	w2;
	double 	w3;

	w1 = fabs(v1.w);
	w2 = fabs(v2.w);
	w3 = fabs(v3.w);
	counter = 0;
	counter += fabs(v1.x) <= w1;
	counter += fabs(v1.y) <= w1;
	counter += v1.z <= app->camera->z_far;
	counter += v1.z >= app->camera->z_near;
	counter += fabs(v2.x) <= w2;
	counter += fabs(v2.y) <= w2;
	counter += v2.z <= app->camera->z_far;
	counter += v2.z >= app->camera->z_near;
	counter += fabs(v3.x) <= w3;
	counter += fabs(v3.y) <= w3;
	counter += v3.z <= app->camera->z_far;
	counter += v3.z >= app->camera->z_near;
	return (counter);
}

void 	fill_triangle(t_app *app, t_v3d v1, t_v3d v2, t_v3d v3)
{
	v1 = matrix_transform(app->camera->screen_space, v1);
	v2 = matrix_transform(app->camera->screen_space, v2);
	v3 = matrix_transform(app->camera->screen_space, v3);
	vertex_perspective_divide(&v1);
	vertex_perspective_divide(&v2);
	vertex_perspective_divide(&v3);
//	if (triangle_area(&v1, &v3, &v2) >= 0.0)
//		return;
	if (v3.y < v2.y)
		SWAP(v2, v3, t_v3d);
	if (v2.y < v1.y)
		SWAP(v1, v2, t_v3d);
	if (v3.y < v2.y)
		SWAP(v2, v3, t_v3d);
	scan_triangle(app, v1, v2, v3, triangle_area(&v1, &v3, &v2) >= 0.0);
	app->triangles_counter++;
}

void	polygon_add(t_polygon **poly, t_v3d v)
{
	t_polygon	*new;
	t_polygon	*last;

	if ((*poly) == NULL)
	{
		(*poly) = (t_polygon *)malloc(sizeof(t_polygon));
		(*poly)->v = v;
		(*poly)->is_convex = 0;
		(*poly)->is_ear = 0;
		(*poly)->angle = 0.0;
		(*poly)->next = *poly;
		(*poly)->prev = *poly;
	}
	else
	{
		last = (*poly)->prev;
		new = (t_polygon *)malloc(sizeof(t_polygon));
		new->v = v;
		new->is_convex = 0;
		new->is_ear = 0;
		new->angle = 0.0;
		new->next = (*poly);
		new->prev = last;
		last->next = new;
		(*poly)->prev = new;
	}
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

double	vector_dist(t_v3d v1, t_v3d v2)
{
	double	dx;
	double	dy;
	double	dz;

	dx = v2.x - v1.x;
	dy = v2.y - v1.y;
	dz = v2.z - v1.z;
	return (dx * dx) + (dy * dy) + (dz * dz);
}

void		ray_intersect(t_app *app, t_v3d v0, t_v3d v1, t_v3d v2)
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
	pvec = vector_cross_product(app->camera->dir, v0v2);
	det = vector_dot_product(v0v1, pvec);
	if (fabs(det) < 0.0)
		return ;
	invDet = 1 / det;
	tvec = vector_sub(app->camera->pos, v0);
	u = vector_dot_product(tvec, pvec) * invDet;
	if (u < 0.0 || u > 1.0)
		return ;
	qvec = vector_cross_product(tvec, v0v1);
	v = vector_dot_product(app->camera->dir, qvec) * invDet;
	if (v < 0.0 || u + v > 1.0)
		return ;
	t = vector_dot_product(v0v2, qvec) * invDet;
	if (t < app->hit_dist && t > 0.0)
	{
		app->hit_point = vector_sum(app->camera->pos, vector_mul_by(app->camera->dir, t));
		app->hit_dist = t;
		app->hit_wall = app->render_wall;
		app->hit = 1;
	}
}

int		render_triangle(t_app *app, t_v3d v1, t_v3d v2, t_v3d v3)
{
	int		inside_counter;

	inside_counter = inside_view(app, v1, v2, v3);
	if (!inside_counter)
		return 0;
	if (inside_counter < 12)
		clip_fill_triangle(app, v1, v2, v3);
	else
		fill_triangle(app, v1, v2, v3);
	return 1;
}

void 	render_floor(t_app *app, t_triangle *tr, t_wall *floor)
{
	t_v3d	v0;
	t_v3d	v1;
	t_v3d	v2;

	v0 = matrix_transform(app->camera->transform, tr->v[0]);
	v1 = matrix_transform(app->camera->transform, tr->v[1]);
	v2 = matrix_transform(app->camera->transform, tr->v[2]);
	app->hit = 0;
	app->render_wall = floor;
	if (render_triangle(app, v0, v1, v2) && !app->edge_selected)
	{
		ray_intersect(app, tr->v[0], tr->v[1], tr->v[2]);
		if (app->hit)
			app->hit_first = 1;
	}
}

void 	render_ceil(t_app *app, t_triangle *tr, t_wall *ceil)
{
	t_v3d	v0;
	t_v3d	v1;
	t_v3d	v2;

	v0 = matrix_transform(app->camera->transform, tr->v[0]);
	v1 = matrix_transform(app->camera->transform, tr->v[1]);
	v2 = matrix_transform(app->camera->transform, tr->v[2]);
	app->hit = 0;
	app->render_wall = ceil;
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
	while (i <= len)
	{
		set_pixel_uint32(app->sdl->surface, (start_y + i) * SCREEN_W + x, color);
		set_pixel_uint32(app->sdl->surface, y * SCREEN_W + (start_x + i), color);
		i++;
	}
}

void 	select_edge(t_app *app,  t_v3d v0, t_v3d v1)
{
	t_wall *w;

	w = app->hit_wall;
	app->edge_selected = 1;
	app->edit_wall.v[0] = v1;
	app->edit_wall.v[1] = v0;
	app->edit_wall.v[2] = v1;
	app->edit_wall.v[3] = v0;
	app->edit_wall.sprite_index = w->sprite_index;
	wall_reset_tex(&app->edit_wall);
	wall_update_scale(&app->edit_wall);
	app->inputs->mouse.left = 0;
}

void 	draw_edge(t_app *app, t_v3d edge)
{
	t_v3d v;

	v = matrix_transform(app->camera->transform, edge);
	if (vertex_inside(&v))
	{
		v = matrix_transform(app->camera->screen_space, v);
		vertex_perspective_divide(&v);
		draw_cross(app, (int)v.x, (int)v.y, app->hit_dist + 0.5, 0xff0000);
	}
}

void 	show_edge(t_app *app)
{
	t_v3d	v0;
	t_v3d	v1;

	v0 = app->hit_wall->v[app->hit_first ? 1 : 3];
	v1 = app->hit_wall->v[app->hit_first ? 2 : 0];
	if (v0.y != v1.y)
	{
		draw_edge(app, v0);
		draw_edge(app, v1);
		if (app->inputs->mouse.left)
			select_edge(app, v0, v1);
	}
}

void	render_map(t_app *app)
{
	int i;
	int	j;
	t_sector *s;

	i = 0;
	app->hit_dist = 10000.0;
	app->hit = 0;
	app->hit_wall = NULL;
	while (i < app->sectors_count)
	{
		j = 0;
		s = &app->sectors[i];
		while (j < s->walls_count)
			render_wall(app, &s->walls[j++]);
		if (s->ready &&s->triangles_count > 0)
		{
			int			t;
			t_triangle	ceil_triangle;

			t = 0;
			while (t < s->triangles_count)
			{
				render_floor(app, &s->triangles[t], &s->floor);
				ceil_triangle = s->triangles[t];
				ceil_triangle.v[0].y += 2.0;
				ceil_triangle.v[1].y += 2.0;
				ceil_triangle.v[2].y += 2.0;
				render_ceil(app, &ceil_triangle, &s->ceil);
				t++;
			}
		}
		i++;
	}
}

int 	compare_vertex(t_v3d *v1, t_v3d *v2)
{
	return (v1->x == v2->x && v1->y == v2->y && v1->z == v2->z);
}


int	find_linked_wall(t_sector *sector, t_v3d v, int skip)
{
	int		i;
	i = 0;

	while (i < sector->walls_count)
	{
		if (i != skip)
		{
			if(compare_vertex(&sector->walls[i].v[0], &v) ||
				compare_vertex(&sector->walls[i].v[2], &v))
				return i;
		}
		i++;
	}
	return (0);
}

void 	draw_polygon_line(t_app *app, t_v3d start, t_v3d end)
{
	t_v3d		tmp1;
	t_v3d		tmp2;

	tmp1 = matrix_transform(app->camera->transform, start);
	tmp2 = matrix_transform(app->camera->transform, end);
	if (vertex_inside(&tmp1) &&
		vertex_inside(&tmp2))
	{
		tmp1 = matrix_transform(app->camera->screen_space, tmp1);
		vertex_perspective_divide(&tmp1);
		tmp2 = matrix_transform(app->camera->screen_space, tmp2);
		vertex_perspective_divide(&tmp2);
		draw_line(app, &tmp1, &tmp2, 0xffffff);
	}
}

void 	draw_polygon(t_app *app, t_polygon *polygon)
{
	t_polygon	*head;

	head = polygon;
	while (polygon->next != head)
	{
		draw_polygon_line(app, polygon->v, polygon->next->v);
		polygon = polygon->next;
	}
	draw_polygon_line(app, polygon->v, polygon->next->v);
}

void 	draw_triangles(t_app *app, t_sector *cs)
{
	int i;
	t_triangle *tr;

	i = 0;
	while (i < cs->triangles_count)
	{
		tr = &cs->triangles[i];
		draw_polygon_line(app, tr->v[0], tr->v[1]);
		draw_polygon_line(app, tr->v[0], tr->v[2]);
		draw_polygon_line(app, tr->v[1], tr->v[2]);
		i++;
	}
}

double	poly_cross(t_v3d v1, t_v3d v2) {
	return (v1.x * v2.z - v1.z * v2.x);
}

double	get_orientation(t_v3d *polygon, int size)
{
	double		sum;
	int			i;

	i = 0;
	sum = 0.0;
	while (i < size)
	{
		sum += poly_cross(polygon[i], polygon[(i + 1) % size]);
		i++;
	}
	return (sum);
}

double 	calc_tex(double min, double cur, double max)
{
	return ((cur - min)/(max - min));
}

void 	get_floor_poly(t_sector *cs)
{
	int		i;
	t_v3d	*polygon;
	int		next_wall;

	polygon = (t_v3d *)malloc(sizeof(t_v3d) * cs->walls_count);

	polygon[0] = cs->walls[0].v[0];
	polygon[1] = cs->walls[0].v[2];

	cs->polygon_count = 2;
	i = 1;
	next_wall = 0;
	while (i < cs->walls_count - 1)
	{
		next_wall = find_linked_wall(cs, polygon[i], next_wall);
		if (next_wall != 0)
		{
			if (compare_vertex(&polygon[i], &cs->walls[next_wall].v[0]))
				polygon[i + 1] = cs->walls[next_wall].v[2];
			else if(compare_vertex(&polygon[i], &cs->walls[next_wall].v[2]))
				polygon[i + 1] = cs->walls[next_wall].v[0];
		}
		cs->polygon_count++;
		i++;
	}

	double or = get_orientation(&polygon[0], cs->polygon_count);

	if (or > 0)
	{
		i = 0;
		cs->polygon = NULL;
		while (i < cs->polygon_count)
		{
			polygon[i].tex_x = calc_tex(cs->box.x_min, polygon[i].x, cs->box.x_max);
			polygon[i].tex_y = calc_tex(cs->box.z_min, polygon[i].z, cs->box.z_max);
			polygon_add(&cs->polygon, polygon[i]);
			i++;
		}
	}
	else if (or < 0)
	{
		i = cs->polygon_count - 1;
		cs->polygon = NULL;
		while (i >= 0)
		{
			polygon[i].tex_x = calc_tex(cs->box.x_min, polygon[i].x, cs->box.x_max);
			polygon[i].tex_y = calc_tex(cs->box.z_min, polygon[i].z, cs->box.z_max);
			polygon_add(&cs->polygon, polygon[i]);
			i--;
		}
	}
	free(polygon);

	/*TODO: Fix malloc size!*/
	cs->triangles = (t_triangle *)malloc(sizeof(t_triangle) * 100);
	cs->triangles_count = 0;
	triangulate(cs);
}

void 	get_sector_min_max(t_sector *cs)
{
	int i;

	i = 0;
	cs->box.x_min = cs->walls[0].v[0].x;
	cs->box.x_max = cs->box.x_min;
	cs->box.z_min = cs->walls[0].v[0].z;
	cs->box.z_max = cs->box.z_min;
	while (i < cs->walls_count)
	{
		t_wall w;

		w = cs->walls[i];
		if (w.v[0].x < cs->box.x_min)
			cs->box.x_min = w.v[0].x;
		if (w.v[0].x > cs->box.x_max)
			cs->box.x_max = w.v[0].x;
		if (w.v[0].z < cs->box.z_min)
			cs->box.z_min = w.v[0].z;
		if (w.v[0].z > cs->box.z_max)
			cs->box.z_max = w.v[0].z;
		if (w.v[2].x < cs->box.x_min)
			cs->box.x_min = w.v[2].x;
		if (w.v[2].x > cs->box.x_max)
			cs->box.x_max = w.v[2].x;
		if (w.v[2].z < cs->box.z_min)
			cs->box.z_min = w.v[2].z;
		if (w.v[2].z > cs->box.z_max)
			cs->box.z_max = w.v[2].z;
		i++;
	}
}

void	start_the_game(t_app *app)
{
	SDL_SetRelativeMouseMode(SDL_TRUE);
	app->camera->pos = new_vector(1.0, 1.0, -10);
	app->camera->rot = new_vector(0.0, 0.0, 0.0);
	app->camera->fov = TO_RAD(90.0);
	app->camera->projection = matrix_perspective(
			app->camera->fov,
			(double)SCREEN_W / (double)SCREEN_H,
			0.05,
			1000.0);

	app->edge_selected = 0;
	app->grid_size = 0.5;

	app->timer->curr = SDL_GetPerformanceCounter();
	app->triangles_counter = 0;
	while (1)
	{
		if (!event_handling(app))
			break;
		get_delta_time(app->timer);
		if (app->inputs->keyboard[SDL_SCANCODE_ESCAPE])
			break ;
		process_inputs(app, app->timer->delta);

		if (app->input_g)
		{
			app->grid_size = app->grid_size == 2.0 ? 0.5 : 2.0;
			app->input_g = 0;
		}

		if (app->hit_wall)
		{
			if (app->input_minus && app->inputs->keyboard[SDL_SCANCODE_LCTRL] && app->hit_wall->scale_x > 0.25)
			{
				app->hit_wall->scale_x -= 0.25;
				app->input_minus = 0;
			}
			else if (app->input_plus && app->inputs->keyboard[SDL_SCANCODE_LCTRL] && app->hit_wall->scale_x < 10.0)
			{
				app->hit_wall->scale_x += 0.25;
				app->input_plus = 0;
			}
			else if (app->input_minus && app->inputs->keyboard[SDL_SCANCODE_LALT] && app->hit_wall->scale_y > 0.25)
			{
				app->hit_wall->scale_y -= 0.25;
				app->input_minus = 0;
			}
			else if (app->input_plus && app->inputs->keyboard[SDL_SCANCODE_LALT] && app->hit_wall->scale_y < 10.0)
			{
				app->hit_wall->scale_y += 0.25;
				app->input_plus = 0;
			}
			else if (app->input_minus && app->inputs->keyboard[SDL_SCANCODE_LSHIFT])
			{
				app->input_minus = 0;
				app->hit_wall->sprite_index--;
			}
			else if (app->input_plus && app->inputs->keyboard[SDL_SCANCODE_LSHIFT])
			{
				app->input_plus = 0;
				app->hit_wall->sprite_index++;
			}
			app->hit_wall->sprite_index = (app->hit_wall->sprite_index % 493 + 493) % 493;
		}

		update_camera(app->camera);
		reset_screen(app);

		render_map(app);

		if (app->hit_wall && !app->edge_selected)
			show_edge(app);

		if (app->edge_selected)
		{
			t_v3d tmp;

			tmp = vector_sum(app->camera->pos, vector_mul_by(get_forward(app->camera->rot), 2));
			tmp.x = round(tmp.x / app->grid_size) * app->grid_size;
			tmp.z = round(tmp.z / app->grid_size) * app->grid_size;
			app->edit_wall.v[app->hit_first ? 1 : 3].x = tmp.x;
			app->edit_wall.v[app->hit_first ? 1 : 3].z = tmp.z;
			app->edit_wall.v[app->hit_first ? 2 : 0].x = tmp.x;
			app->edit_wall.v[app->hit_first ? 2 : 0].z = tmp.z;
			wall_update_scale(&app->edit_wall);

			if (app->inputs->mouse.right)
			{
				app->edge_selected = 0;
				app->inputs->mouse.right = 0;
			}
			else if (app->inputs->mouse.left)
			{
				t_sector *cs;

				cs = &app->sectors[app->sectors_count - 1];
				cs->walls[cs->walls_count] = app->edit_wall;
				cs->walls_count++;
				app->hit_wall = NULL;
				app->hit = 0;
				app->hit_dist = 10000.0;
				app->edge_selected = 0;
				app->inputs->mouse.left = 0;
			}
			else
			{
				render_wall(app, &app->edit_wall);
				draw_grid(app);
			}
		}

		if (app->inputs->keyboard[SDL_SCANCODE_E] && app->sectors[app->sectors_count - 1].ready)
			app->sectors[app->sectors_count - 1].ready = 0;

		t_sector *cs;
		cs = &app->sectors[app->sectors_count - 1];

		if (!app->edge_selected && cs->walls_count >= 3 && !cs->ready)
		{
			if (app->inputs->keyboard[SDL_SCANCODE_Q])
			{
				get_sector_min_max(cs);
				get_floor_poly(cs);

				/*TODO: Set sector floor height*/
				cs->floor_height = 0.0;
				cs->floor = wall_new();

				cs->floor.v[0].x = cs->box.x_min;
				cs->floor.v[0].y = cs->floor_height;
				cs->floor.v[0].z = cs->box.z_min;
				cs->floor.v[1].x = cs->box.x_max;
				cs->floor.v[1].y = cs->floor_height;
				cs->floor.v[1].z = cs->box.z_max;
				cs->floor.v[2].x = cs->box.x_max;
				cs->floor.v[2].y = cs->floor_height;
				cs->floor.v[2].z = cs->box.z_min;
				cs->floor.v[3].x = cs->box.x_min;
				cs->floor.v[3].y = cs->floor_height;
				cs->floor.v[3].z = cs->box.z_max;

				cs->floor.scale_x = fabs(cs->box.x_min - cs->box.x_max) * 0.5;
				cs->floor.scale_y = fabs(cs->box.z_min - cs->box.z_max) * 0.5;
				cs->floor.sprite_index = 278;
				cs->ceil = cs->floor;

				cs->ceil.v[0].y += 2.0;
				cs->ceil.v[1].y += 2.0;
				cs->ceil.v[2].y += 2.0;
				cs->ceil.v[3].y += 2.0;

				cs->ceil.sprite_index = 399;
				cs->ready = 1;
			}
		}
		draw_cross(app, (int)app->sdl->half_width, (int)app->sdl->half_height, 8, 0xffffff);
		update_fps_text(app);
		update_walls_data(app);
		//mouse_update(app);
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
	obj_load("resources/plane.obj", &app->meshes[0]);
	obj_load("resources/cube.obj", &app->meshes[1]);

	/* TODO: Set number of meshes */
	app->sprites_count = 0;
	app->sprites = (t_sprite *)malloc(sizeof(t_sprite) * 493);
	for (int i = 0; i < 493; i++)
	{
		char	*file_name;
		char	file_path[100];
		file_path[0] = '\0';
		file_name = ft_itoa(i);
		ft_strcat(file_path, "resources/sprites/");
		ft_strcat(file_path, file_name);
		ft_strdel(&file_name);
		ft_strcat(file_path, ".bmp");
		bmp_load(app, file_path);
	}

	app->sectors_count = 1;
	app->sectors = (t_sector *)malloc(sizeof(t_sector) * 1000);
	app->sectors[0].walls_count = 0;
	app->sectors[0].walls = (t_wall *)malloc(sizeof(t_wall) * 1000);

	app->sectors[0].walls[0] = wall_new();
	app->sectors[0].walls[0].v[0].x = 0.0;
	app->sectors[0].walls[0].v[0].y = 0.0;
	app->sectors[0].walls[0].v[0].z = 0.0;
	app->sectors[0].walls[0].v[1].x = 2.0;
	app->sectors[0].walls[0].v[1].y = 2.0;
	app->sectors[0].walls[0].v[1].z = 0.0;
	app->sectors[0].walls[0].v[2].x = 2.0;
	app->sectors[0].walls[0].v[2].y = 0.0;
	app->sectors[0].walls[0].v[2].z = 0.0;
	app->sectors[0].walls[0].v[3].x = 0.0;
	app->sectors[0].walls[0].v[3].y = 2.0;
	app->sectors[0].walls[0].v[3].z = 0.0;

	app->sectors[0].ready = 0;

	app->sectors[0].walls_count++;

	start_the_game(app);
	quit_properly(app);

	return (0);
}

