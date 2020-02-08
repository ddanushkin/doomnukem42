#include "doom_nukem.h"

void	wall_reset_tex(t_wall *w)
{
	w->v[0].tex_x = 0.0;
	w->v[0].tex_y = 0.0;
	w->v[1].tex_x = 1.0;
	w->v[1].tex_y = 1.0;
	w->v[2].tex_x = 1.0;
	w->v[2].tex_y = 0.0;
	w->v[3].tex_x = 0.0;
	w->v[3].tex_y = 1.0;
}

t_wall	wall_new()
{
	t_wall	w;

	w.v[0].w = 1.0;
	w.v[1].w = 1.0;
	w.v[2].w = 1.0;
	w.v[3].w = 1.0;
	wall_reset_tex(&w);
	w.sx = 1.0;
	w.sy = 1.0;
	w.sprite = 103;
	w.decor = 0;
	return (w);
}

void 	wall_update_scale(t_wall *w)
{
	double	dx;
	double	dz;

	dx = fabs(w->v[0].x - w->v[1].x);
	dz = fabs(w->v[0].z - w->v[1].z);
	w->sx = (dx + dz) * 0.5;
	w->sy = fabs(w->v[0].y - w->v[1].y) * 0.5;
}

void	update_walls_data(t_app *app)
{
	char	*fps_text;

	fps_text = ft_itoa(app->triangles_counter);
	print_to_screen(app, 0, 15, fps_text);
	ft_strdel(&fps_text);
	app->triangles_counter = 0;
}