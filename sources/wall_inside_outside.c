/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wall_inside_outside.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lglover <lglover@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/11 16:29:30 by lglover           #+#    #+#             */
/*   Updated: 2020/03/11 17:11:01 by lglover          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"

uint32_t	wall_inside(t_v3d *v0, t_v3d *v1, t_v3d *v2, t_v3d *v3)
{
	return (
	(vertex_inside(v0) << 24u) +
	(vertex_inside(v1) << 16u) +
	(vertex_inside(v2) << 8u) +
	vertex_inside(v3));
}

uint8_t		wall_outside_x(t_v3d *v0, t_v3d *v1, t_v3d *v2, t_v3d *v3)
{
	return ((
	v0->x > v0->w &&
	v1->x > v1->w &&
	v2->x > v2->w &&
	v3->x > v3->w) || (
	v0->x < -v0->w &&
	v1->x < -v1->w &&
	v2->x < -v2->w &&
	v3->x < -v3->w));
}

uint8_t		wall_outside_y(t_v3d *v0, t_v3d *v1, t_v3d *v2, t_v3d *v3)
{
	return ((
	v0->y > v0->w &&
	v1->y > v1->w &&
	v2->y > v2->w &&
	v3->y > v3->w) || (
	v0->y < -v0->w &&
	v1->y < -v1->w &&
	v2->y < -v2->w &&
	v3->y < -v3->w));
}

uint8_t		wall_outside_z(t_v3d *v0, t_v3d *v1, t_v3d *v2, t_v3d *v3)
{
	return ((
	v0->z > v0->w &&
	v1->z > v1->w &&
	v2->z > v2->w &&
	v3->z > v3->w) || (
	v0->z < -v0->w &&
	v1->z < -v1->w &&
	v2->z < -v2->w &&
	v3->z < -v3->w));
}

uint8_t		wall_outside(t_v3d *v0, t_v3d *v1, t_v3d *v2, t_v3d *v3)
{
	return (
	wall_outside_x(v0, v1, v2, v3) ||
	wall_outside_y(v0, v1, v2, v3) ||
	wall_outside_z(v0, v1, v2, v3));
}
