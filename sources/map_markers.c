/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_markers.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lglover <lglover@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/11 14:31:08 by lglover           #+#    #+#             */
/*   Updated: 2020/03/11 14:32:54 by lglover          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"

void	draw_exit(t_app *app)
{
	int			i;
	int			j;
	t_v3d		p;
	t_sector	*s;

	i = 0;
	while (i < app->sectors_count)
	{
		s = &app->sectors[i];
		j = 0;
		while (j < s->decor_count)
		{
			if (s->decor[j].is_exit)
			{
				p = new_vector(s->decor[j].v[0].x, 0.0, s->decor[j].v[0].z);
				p = point_3d_to_2d(app, p);
				if (point_in_screen(p))
					return (pts(app, p.x, p.y, "EXIT\0"));
			}
			j++;
		}
		i++;
	}
}

void	draw_start(t_app *app)
{
	t_v3d	p;

	if (app->md.start_set)
	{
		p = new_vector(app->md.start_pos.x, 0.0, app->md.start_pos.z);
		p = point_3d_to_2d(app, p);
		if (point_in_screen(p))
			pts(app, p.x, p.y, "START\0");
	}
	if (app->md.card_set)
	{
		p = new_vector(app->md.card_pos.x, 0.0, app->md.card_pos.z);
		p = point_3d_to_2d(app, p);
		if (point_in_screen(p))
			pts(app, p.x, p.y, "CARD\0");
	}
}
