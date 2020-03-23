/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sector_close_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lglover <lglover@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/23 17:28:45 by lglover           #+#    #+#             */
/*   Updated: 2020/03/23 17:28:45 by lglover          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"

void	sector_new(t_app *app, t_sector *s)
{
	s->floor = wall_new();
	s->floor.sprite = FLOOR_SPRITE;
	s->ceil = s->floor;
	s->floor.flip = 0;
	s->ceil.sprite = CEIL_SPRITE;
	s->floor_y = 0.0;
	s->ceil_y = 2.0;
	s->delta_y = fabs(s->ceil_y - s->floor_y);
	s->walls_count = 0;
	s->npcs_count = 0;
	s->decor_count = 0;
	s->decor_next = 0;
	s->shade = 0;
	s->inside = 0;
	s->door_anim = 0;
	s->door = 0;
	s->door_dir = -1.0;
	s->lava = 0;
	s->id = app->sectors_count;
}

void	sector_copy_v_1(t_sector *s, t_v3d *p, int len)
{
	int		i;

	i = 0;
	while (i < len)
	{
		s->fpts[i] = p[i];
		s->fpts[i].y = s->floor_y;
		s->fpts[i].i = i;
		s->cpts[i] = p[i];
		s->cpts[i].y = s->ceil_y;
		s->cpts[i].i = i;
		i++;
	}
	s->pts_count = len;
}

void	sector_copy_v_2(t_sector *s, t_v3d *p, int len)
{
	int		i;
	int		j;

	i = len - 1;
	j = 0;
	while (i >= 0)
	{
		s->fpts[j] = p[i];
		s->fpts[j].y = s->floor_y;
		s->fpts[j].i = i;
		s->cpts[j] = p[i];
		s->cpts[j].y = s->ceil_y;
		s->cpts[j].i = i;
		i--;
		j++;
	}
	s->pts_count = len;
}
