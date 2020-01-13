#include "doom_nukem.h"

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