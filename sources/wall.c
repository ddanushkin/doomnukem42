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
	w.sprite = WALL_SPRITE;
	w.decor = 0;
	w.active = 1;
	w.flip = 1;
	w.is_exit = 0;
	w.ox = 0.0;
	w.oy = 0.0;
	w.anim_auto = 0;
	w.rotate = 0;
	w.collect = 0;
	w.is_card = 0;
	w.use = 0;
	w.ori = 0;
	return (w);
}

void 	wall_update_scale(t_wall *w)
{
	t_wus d;

	d.dx = fabs(w->v[0].x - w->v[3].x);
	d.dz = fabs(w->v[0].z - w->v[3].z);
	d.dy = fabs(w->v[0].y - w->v[3].y);
	d.v0 = sqrt(d.dx * d.dx + d.dy * d.dy + d.dz * d.dz);
	d.dx = fabs(w->v[2].x - w->v[1].x);
	d.dz = fabs(w->v[2].z - w->v[1].z);
	d.dy = fabs(w->v[2].y - w->v[1].y);
	d.v1 = sqrt(d.dx * d.dx + d.dy * d.dy + d.dz * d.dz);
	d.dx = fabs(w->v[0].x - w->v[2].x);
	d.dz = fabs(w->v[0].z - w->v[2].z);
	d.dy = fabs(w->v[0].y - w->v[2].y);
	d.h0 = sqrt(d.dx * d.dx + d.dy * d.dy + d.dz * d.dz);
	d.dx = fabs(w->v[3].x - w->v[1].x);
	d.dz = fabs(w->v[3].z - w->v[1].z);
	d.dy = fabs(w->v[3].y - w->v[1].y);
	d.h1 = sqrt(d.dx * d.dx + d.dy * d.dy + d.dz * d.dz);
	d.v = MAX(d.v0, d.v1);
	d.h = MIN(d.h0, d.h1);
	w->sy = d.v * 0.5;
	w->sx = d.h * 0.5;
}

void 	wall_update_tex(t_wall *w)
{
	double	dy0;
	double	dy1;

	dy0 = fabs(w->v[0].y - w->v[3].y);
	dy1 = fabs(w->v[2].y - w->v[1].y);
	wall_reset_tex(w);
	if (dy0 > dy1)
	{
		w->v[2].tex_y = calc_tex(w->v[0].y, w->v[2].y, w->v[3].y);
		w->v[1].tex_y = calc_tex(w->v[0].y, w->v[1].y, w->v[3].y);
	}
	else
	{
		w->v[0].tex_y = calc_tex(w->v[2].y, w->v[0].y, w->v[1].y);
		w->v[3].tex_y = calc_tex(w->v[2].y, w->v[3].y, w->v[1].y);
	}
}
