/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pixel.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lglover <lglover@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/11 13:03:39 by lglover           #+#    #+#             */
/*   Updated: 2020/03/11 16:25:49 by lglover          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"

int		point_in_screen(t_v3d p)
{
	return ((int)p.x >= 0 && (int)p.x < SCREEN_W &&
			(int)p.y >= 0 && (int)p.y < SCREEN_H);
}

void	pixel_set(SDL_Surface *surface, int offset, uint32_t c)
{
	if (offset < 0 || offset >= SCREEN_W * SCREEN_H)
		return ;
	((uint32_t *)surface->pixels)[offset] = c;
}
